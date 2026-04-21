from quant_finance._core import math as _math


def add(a: float, b: float) -> float:
    """Return the sum of two numbers.

    Args:
        a: The first number.
        b: The second number.

    Returns:
        The sum of a and b.
    """
    result: float = _math.add(a, b)
    return result


def subtract(a: float, b: float) -> float:
    """Return the result of subtracting one number from another.

    Args:
        a: The number to subtract from.
        b: The number to subtract.

    Returns:
        The result of a minus b.
    """
    result: float = _math.subtract(a, b)
    return result


def multiply(a: float, b: float) -> float:
    """Return the product of two numbers.

    Args:
        a: The first number.
        b: The second number.

    Returns:
        The product of a and b.
    """
    result: float = _math.multiply(a, b)
    return result


def divide(a: float, b: float) -> float:
    """Return the result of dividing one number by another.

    Args:
        a: The numerator.
        b: The denominator.

    Returns:
        The result of dividing a by b.

    Raises:
        ZeroDivisionError: If b is zero.
    """
    result: float = _math.divide(a, b)
    return result
