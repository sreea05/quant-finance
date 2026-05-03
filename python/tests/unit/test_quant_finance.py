"""Unit tests for the quant_finance package."""

import numpy as np

import quant_finance.math


def test_scale_matrix_basic() -> None:
    mat = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]], dtype=float, order="F")
    expected = np.array([[5, 10, 15], [20, 25, 30], [35, 40, 45]], dtype=float, order="F")
    result = quant_finance.math.scale_matrix(mat, 5)
    np.testing.assert_array_almost_equal(result, expected)


def test_scale_matrix_by_zero() -> None:
    mat = np.array([[1, 2], [3, 4]], dtype=float, order="F")
    result = quant_finance.math.scale_matrix(mat, 0)
    np.testing.assert_array_almost_equal(result, np.zeros((2, 2)))


def test_scale_matrix_by_one() -> None:
    mat = np.array([[1.5, 2.5], [3.5, 4.5]], dtype=float, order="F")
    result = quant_finance.math.scale_matrix(mat, 1)
    np.testing.assert_array_almost_equal(result, mat)
