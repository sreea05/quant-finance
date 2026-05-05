"""Unit tests for quant_finance.logger."""

import pytest

from quant_finance.logger import Level, Logger


def test_get_returns_same_instance() -> None:
    assert Logger.get() is Logger.get()


def test_get_level_returns_level_enum() -> None:
    assert isinstance(Logger.get().get_level(), Level)


@pytest.mark.parametrize("level", list(Level))
def test_set_and_get_level_roundtrip(level: Level) -> None:
    Logger.get().set_level(level)
    assert Logger.get().get_level() == level


@pytest.mark.parametrize("level", list(Level))
def test_cpp_level_integer_matches(level: Level) -> None:
    assert int(Logger._to_cpp_level(level)) == int(level)
