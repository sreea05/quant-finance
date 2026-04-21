"""Unit tests for the quant_finance package."""

import pytest

import quant_finance.math


def test_add() -> None:
    assert quant_finance.math.add(1.0, 2.0) == 3.0
    assert quant_finance.math.add(-1.0, 1.0) == 0.0
    assert quant_finance.math.add(0.0, 0.0) == 0.0


def test_subtract() -> None:
    assert quant_finance.math.subtract(5.0, 3.0) == 2.0
    assert quant_finance.math.subtract(0.0, 5.0) == -5.0
    assert quant_finance.math.subtract(3.0, 3.0) == 0.0


def test_multiply() -> None:
    assert quant_finance.math.multiply(3.0, 4.0) == 12.0
    assert quant_finance.math.multiply(-2.0, 3.0) == -6.0
    assert quant_finance.math.multiply(0.0, 100.0) == 0.0


def test_divide() -> None:
    assert quant_finance.math.divide(10.0, 2.0) == 5.0
    assert quant_finance.math.divide(7.0, 2.0) == 3.5
    assert quant_finance.math.divide(-6.0, 3.0) == -2.0


def test_divide_by_zero() -> None:
    with pytest.raises(ValueError):
        quant_finance.math.divide(1.0, 0.0)
    with pytest.raises(ValueError):
        quant_finance.math.divide(-5.0, 0.0)
