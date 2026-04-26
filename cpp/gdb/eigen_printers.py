from __future__ import annotations

import re
from collections.abc import Iterable
from typing import Any

import gdb

MAX_PRINT = 12
FLOAT_FMT = "{:.4g}"


# ============================================================
# LOW-LEVEL PRINTER (robust)
# ============================================================


class _EigenCore:
    def __init__(self, val: gdb.Value) -> None:
        self.val = val

        t = val.type
        if t.code == gdb.TYPE_CODE_REF:
            t = t.target()

        self.gdb_type = t.unqualified().strip_typedefs()
        tag = self.gdb_type.tag

        if not tag:
            raise RuntimeError("Not an Eigen type (no debug info?)")

        m = re.search(r"<(.*)>", tag)
        if not m:
            raise RuntimeError("Not an Eigen matrix")

        params = [x.strip() for x in m.group(1).split(",")]

        # dimensions
        self.rows = int(val["m_storage"]["m_rows"]) if params[1].startswith("-") else int(params[1])
        self.cols = int(val["m_storage"]["m_cols"]) if params[2].startswith("-") else int(params[2])

        # layout
        opts = int(params[3]) if len(params) > 3 else 0
        self.row_major = bool(opts & 1)

        self.inner = self.gdb_type.template_argument(0)

        # data pointer (robust)
        data = val["m_storage"]["m_data"]

        try:
            if data.type.code == gdb.TYPE_CODE_STRUCT:
                for f in data.type.fields():
                    if f.name == "array":
                        data = data["array"]
        except Exception:
            pass

        try:
            self.data = data.cast(self.inner.pointer())
        except Exception:
            self.data = data

    def at(self, r: int, c: int) -> gdb.Value:
        if not (0 <= r < self.rows and 0 <= c < self.cols):
            raise IndexError(f"Index ({r},{c}) out of bounds {self.rows}x{self.cols}")

        idx = r * self.cols + c if self.row_major else c * self.rows + r
        return (self.data + idx).dereference()

    def fmt(self, v: gdb.Value) -> str:
        try:
            f = float(v)
            return "0" if abs(f) < 1e-12 else FLOAT_FMT.format(f)
        except Exception:
            return str(v)

    def grid(self, rr: Iterable[int], cc: Iterable[int]) -> list[list[str]]:
        return [[self.fmt(self.at(r, c)) for c in cc] for r in rr]

    def pretty(self, max_r: int = MAX_PRINT, max_c: int = MAX_PRINT) -> str:
        rr = range(min(self.rows, max_r))
        cc = range(min(self.cols, max_c))

        lines = ["[ " + "  ".join(row) + " ]" for row in self.grid(rr, cc)]

        if self.rows > max_r or self.cols > max_c:
            lines.append(f"... truncated {self.rows}x{self.cols} ...")

        layout = "row-major" if self.row_major else "col-major"
        return f"{self.rows}x{self.cols} Eigen Matrix ({layout})\n" + "\n".join(lines)

    def norm(self) -> float:
        total = sum(float(self.at(r, c)) ** 2 for r in range(self.rows) for c in range(self.cols))
        return total**0.5

    def diag(self) -> list[str]:
        return [self.fmt(self.at(i, i)) for i in range(min(self.rows, self.cols))]

    def stats(self) -> tuple[float, float, float, float]:
        vals = [float(self.at(r, c)) for r in range(self.rows) for c in range(self.cols)]
        if not vals:
            return 0.0, 0.0, 0.0, 0.0
        mn = min(vals)
        mx = max(vals)
        mean = sum(vals) / len(vals)
        frob = sum(v * v for v in vals) ** 0.5
        return mn, mx, mean, frob


# ============================================================
# FRIENDLY API
# ============================================================


class EigenMatrix:
    """
    Friendly wrapper for Eigen matrices in GDB.

    Example:
        m = M("A")
        m[1,2]
        m.row(1)
        m[:,1]
        print(m)
    """

    def __init__(self, expr: str) -> None:
        try:
            val = gdb.parse_and_eval(expr)
        except Exception as e:
            raise RuntimeError(f"Invalid expression: {expr}") from e

        self._core = _EigenCore(val)
        self.rows = self._core.rows
        self.cols = self._core.cols

    # -------------------------
    # indexing
    # -------------------------

    def __getitem__(self, key: tuple[int | slice, int | slice]) -> str | list[list[str]]:
        if not isinstance(key, tuple):
            raise TypeError("Use m[row, col]")

        r, c = key

        def to_range(x: int | slice, maxv: int) -> range | list[int]:
            if isinstance(x, slice):
                return range(*x.indices(maxv))
            return [x]

        rr = to_range(r, self.rows)
        cc = to_range(c, self.cols)

        data = self._core.grid(rr, cc)

        # return scalar if single element
        if len(rr) == 1 and len(cc) == 1:
            return data[0][0]

        return data

    # -------------------------
    # helpers
    # -------------------------

    def row(self, r: int) -> str | list[list[str]]:
        return self[r, :]

    def col(self, c: int) -> str | list[list[str]]:
        return self[:, c]

    def head(self, n: int = 5) -> str | list[list[str]]:
        return self[:n, :n]

    def shape(self) -> tuple[int, int]:
        return (self.rows, self.cols)

    def tolist(self) -> str | list[list[str]]:
        return self[:, :]

    # -------------------------
    # printing
    # -------------------------

    def show(self) -> None:
        print(self._core.pretty())

    def __str__(self) -> str:
        return self._core.pretty()

    def __repr__(self) -> str:
        return f"<EigenMatrix {self.rows}x{self.cols}>"


# shortcut (nice UX)
def M(expr: str) -> EigenMatrix:
    return EigenMatrix(expr)


# ============================================================
# GDB PRETTY PRINTER  (invoked automatically by `p`)
# ============================================================


class EigenMatrixPrinter:
    """Registered in gdb.pretty_printers so that `p mat` works natively."""

    def __init__(self, val: gdb.Value) -> None:
        self._core = _EigenCore(val)

    def to_string(self) -> str:
        return self._core.pretty()

    def display_hint(self) -> str:
        return "string"


def _eigen_lookup(val: gdb.Value) -> EigenMatrixPrinter | None:
    t = val.type
    if t.code == gdb.TYPE_CODE_REF:
        t = t.target()
    t = t.unqualified().strip_typedefs()
    tag = getattr(t, "tag", None)
    if tag and re.match(r"Eigen::(Matrix|Array)", tag):
        try:
            return EigenMatrixPrinter(val)
        except Exception:
            return None
    return None


gdb.pretty_printers.append(_eigen_lookup)


# ============================================================
# SLICE HELPERS  (NumPy-style: VAR[1:3, 0:2])
# ============================================================


def _parse_slice_str(s: str) -> int | slice:
    """'1:3' -> slice(1,3)  |  ':' -> slice(None)  |  '2' -> 2"""
    s = s.strip()
    if ":" in s:
        lo, hi = s.split(":", 1)
        return slice(int(lo) if lo.strip() else None, int(hi) if hi.strip() else None)
    return int(s)


def _parse_eigen_arg(arg: str) -> tuple[str, int | slice, int | slice]:
    """Parse 'var' or 'var[row_spec, col_spec]' into (name, row_key, col_key).

    Examples::

        'A'           -> ('A', slice(None), slice(None))
        'A[0, :]'     -> ('A', 0, slice(None))
        'A[1:3, 0:2]' -> ('A', slice(1,3), slice(0,2))
        'A[:, 2]'     -> ('A', slice(None), 2)
    """
    m = re.match(r"^(.*?)\[([^\[\]]+)\]$", arg.strip())
    if not m:
        return arg.strip(), slice(None), slice(None)
    name = m.group(1).strip()
    parts = [p.strip() for p in m.group(2).split(",")]
    if len(parts) == 1:
        return name, _parse_slice_str(parts[0]), slice(None)
    if len(parts) == 2:
        return name, _parse_slice_str(parts[0]), _parse_slice_str(parts[1])
    raise ValueError(f"Expected [row, col], got: [{m.group(2)}]")


def _display_result(data: str | list[Any]) -> None:
    """Print a scalar string or 2-D list of strings."""
    if isinstance(data, str):
        print(data)
    elif isinstance(data, list):
        for row in data:
            print("  ".join(row) if isinstance(row, list) else str(row))


# ============================================================
# GDB COMMANDS
# ============================================================
# pmat VAR              — full matrix
# pmat VAR[r, c]        — NumPy-style slice  (e.g. A[1:3, :]  A[0, :]  A[:, 2])
# eshape VAR            — dimensions
# pnorm  VAR            — Frobenius / L2 norm
# pdiag  VAR            — main diagonal
# pstat  VAR            — min  max  mean  norm
# ============================================================


class Pmat(gdb.Command):
    """pmat VAR[row, col]  — pretty-print with optional NumPy-style slicing."""

    def __init__(self) -> None:
        super().__init__("pmat", gdb.COMMAND_USER)

    def invoke(self, arg: str, from_tty: bool) -> None:
        arg = arg.strip()
        if not arg:
            print("Usage: pmat VAR  or  pmat VAR[row, col]")
            print("  Slicing examples: A[0, :]  A[:, 2]  A[1:3, 0:2]")
            return
        try:
            name, rs, cs = _parse_eigen_arg(arg)
            m = M(name)
            if rs == slice(None) and cs == slice(None):
                print(m)
            else:
                _display_result(m[rs, cs])
        except Exception as e:
            print(f"pmat: {e}")


class Eshape(gdb.Command):
    """eshape VAR  — print dimensions (rows x cols) of an Eigen matrix."""

    def __init__(self) -> None:
        super().__init__("eshape", gdb.COMMAND_USER)

    def invoke(self, arg: str, from_tty: bool) -> None:
        arg = arg.strip()
        if not arg:
            print("Usage: eshape VAR")
            return
        try:
            m = M(arg)
            print(f"{m.rows} x {m.cols}")
        except Exception as e:
            print(f"eshape: {e}")


class Pnorm(gdb.Command):
    """pnorm VAR  — Frobenius norm (L2 norm for vectors)."""

    def __init__(self) -> None:
        super().__init__("pnorm", gdb.COMMAND_USER)

    def invoke(self, arg: str, from_tty: bool) -> None:
        arg = arg.strip()
        if not arg:
            print("Usage: pnorm VAR")
            return
        try:
            print(f"{M(arg)._core.norm():.6g}")
        except Exception as e:
            print(f"pnorm: {e}")


class Pdiag(gdb.Command):
    """pdiag VAR  — print the main diagonal."""

    def __init__(self) -> None:
        super().__init__("pdiag", gdb.COMMAND_USER)

    def invoke(self, arg: str, from_tty: bool) -> None:
        arg = arg.strip()
        if not arg:
            print("Usage: pdiag VAR")
            return
        try:
            print("  ".join(M(arg)._core.diag()))
        except Exception as e:
            print(f"pdiag: {e}")


class Pstat(gdb.Command):
    """pstat VAR  — print min, max, mean, and Frobenius norm in one line."""

    def __init__(self) -> None:
        super().__init__("pstat", gdb.COMMAND_USER)

    def invoke(self, arg: str, from_tty: bool) -> None:
        arg = arg.strip()
        if not arg:
            print("Usage: pstat VAR")
            return
        try:
            mn, mx, mean, frob = M(arg)._core.stats()
            print(f"min={mn:.4g}  max={mx:.4g}  mean={mean:.4g}  norm={frob:.4g}")
        except Exception as e:
            print(f"pstat: {e}")


# ---- register -------------------------------------------------------
Pmat()
Eshape()
Pnorm()
Pdiag()
Pstat()

print(
    "Eigen pretty-printers loaded.\n"
    "  p VAR                  auto pretty-print (native GDB)\n"
    "  pmat VAR               full matrix\n"
    "  pmat VAR[r, c]         NumPy-style slice  e.g. A[1:3, :]  A[0, :]  A[:, 2]\n"
    "  eshape VAR             dimensions\n"
    "  pnorm VAR              Frobenius / L2 norm\n"
    "  pdiag VAR              main diagonal\n"
    "  pstat VAR              min  max  mean  norm"
)
