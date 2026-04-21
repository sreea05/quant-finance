import pytest

import quant_finance.math


def test_chained_arithmetic() -> None:
    # (10 + 5) * 3 / 5 - 2 = 7.0
    result = quant_finance.math.add(10.0, 5.0)
    result = quant_finance.math.multiply(result, 3.0)
    result = quant_finance.math.divide(result, 5.0)
    result = quant_finance.math.subtract(result, 2.0)
    assert result == 7.0


def test_divide_by_zero_in_chain() -> None:
    result = quant_finance.math.subtract(5.0, 5.0)
    with pytest.raises(ValueError) as _:
        quant_finance.math.divide(10.0, result)
