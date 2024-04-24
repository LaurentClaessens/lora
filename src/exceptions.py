"""Specific exceptions."""


class PyLoraError(Exception):
    """Base exception for my project."""


class NotBuildYet(PyLoraError):
    """The object is not build yet."""


class MtimeError(PyLoraError):
    """Strange Mtime for a file."""
