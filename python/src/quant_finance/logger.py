from __future__ import annotations

import enum

from quant_finance._core import logger as _logger


class Level(enum.IntEnum):
    """Severity levels for the :class:`Logger`.

    The numeric values mirror the C++ levels so that integer comparisons
    between the Python and C++ representations are valid.
    """

    TRACE = 0
    """Fine-grained diagnostic messages."""
    DEBUG = 1
    """Coarse-grained diagnostic messages."""
    INFO = 2
    """Informational runtime messages."""
    WARN = 3
    """Potentially harmful situations."""
    ERR = 4
    """Error events that may still allow continued execution."""
    CRITICAL = 5
    """Severe errors that will likely abort the program."""
    OFF = 6
    """Disables all logging output."""


class Logger:
    """Thread-safe singleton logger used to control the log level of the
    C++ library.

    Wraps the C++ ``qf::logger`` singleton and exposes an interface for
    querying and setting the minimum severity level.  Messages below the
    active level are silently discarded by the C++ layer.  Only one instance
    exists per process; obtain it via :meth:`Logger.get`.

    The class is not intended to be instantiated directly.

    Example::
        Logger.get().set_level(Level.INFO)
    """

    _instance: Logger | None = None

    def __init__(self) -> None:
        self._logger = _logger.logger.get()

    @classmethod
    def get(cls) -> Logger:
        """Return the process-wide :class:`Logger` singleton.

        The instance is created on the first call and reused on every
        subsequent call.

        :returns: The singleton :class:`Logger` instance.
        :rtype: Logger
        """
        if cls._instance is None:
            cls._instance = cls()
        return cls._instance

    def set_level(self, level: Level) -> None:
        """Set the minimum severity level for emitted messages.

        Messages below *level* are silently discarded.

        :param level: The desired minimum :class:`Level`.
        :type level: Level
        """
        self._logger.set_level(Logger._to_cpp_level(level))

    def get_level(self) -> Level:
        """Return the currently active severity level.

        :returns: The current :class:`Level`.
        :rtype: Level
        """
        return Level(int(self._logger.get_level()))

    @staticmethod
    def _to_cpp_level(level: Level) -> _logger.level:
        """Convert a Python :class:`Level` to the underlying C++ ``level`` enum."""
        return _logger.level(int(level))
