"""Some project-related utilities."""

from pathlib import Path

from src.getters import get_options


def is_excluded_dir(my_path: Path):
    """Say if a directory is excluded from the backup."""
    options = get_options()
    paths = options.paths
    _ = paths, my_path

    if my_path.name == "venv":
        exp_python = my_path / "bin" / "python"
        if exp_python.is_file():
            return True
