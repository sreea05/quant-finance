import numpy as np

import quant_finance.example


def test_chain_scale_operations() -> None:
    mat = np.array([[1, 2], [3, 4]], dtype=float, order="F")
    scaled_twice = quant_finance.example.scale_matrix(quant_finance.example.scale_matrix(mat, 2), 3)
    scaled_once = quant_finance.example.scale_matrix(mat, 6)
    np.testing.assert_array_almost_equal(scaled_twice, scaled_once)


def test_scale_by_zero_produces_zero_matrix() -> None:
    mat = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]], dtype=float, order="F")
    result = quant_finance.example.scale_matrix(mat, 0)
    np.testing.assert_array_almost_equal(result, np.zeros((3, 3)))


def test_scale_by_one_preserves_matrix() -> None:
    mat = np.array([[1.5, 2.5, 3.5], [4.5, 5.5, 6.5]], dtype=float, order="F")
    result = quant_finance.example.scale_matrix(mat, 1)
    np.testing.assert_array_almost_equal(result, mat)
