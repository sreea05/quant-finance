"""Math utilities for the quant_finance package.

Thin Python wrappers around the compiled C++ extension module
``quant_finance._core.math``.
"""

import numpy as np

from quant_finance._core import math as _math


def scale_matrix(mat: np.ndarray, scale: float) -> np.ndarray:
    """Scale every element of a matrix by a scalar factor.

    :param mat: The input 2-D array to scale.
    :param scale: The scalar value by which each element of *mat* is multiplied.
    :return: A new array whose elements equal those of *mat* multiplied by *scale*.
    :rtype: np.ndarray
    """
    return _math.scale_matrix(mat, scale)
