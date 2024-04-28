"""Some project-related utilities."""

from pathlib import Path

from src.getters import get_options
from src.options import Options


def is_excluded_dir(my_path: Path) -> bool:
    """Say if a directory is excluded from the backup."""
    options = get_options()
    paths = options.paths
    _ = paths, my_path

    if my_path.name == "venv":
        exp_python = my_path / "bin" / "python"
        if exp_python.is_file():
            return True
    if my_path in paths.excluded_dirs:
        return True
    return False


class CloseThreads:
    """Colored output"""

    def __init__(self, options: Options):
        """Initialize."""
        self.options = options

    def __exit__(self, *args):
        """Reset all the colors"""
        _ = args
        self.options.close_threads()

    def __enter__(self):
        """Enter the context manager."""
        self.options.jobs_manager.start()


def all_files(directory: Path):
    """Yiels all the files to be checked."""
    for elem in directory.iterdir():
        if elem.is_file():
            yield elem
        if elem.is_dir():
            if is_excluded_dir(elem):
                continue
            yield from all_files(elem)
