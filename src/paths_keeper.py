"""
Expose a class to keep the directories and file paths.

This class contains the naming convention for the files and
directories.
"""

from typing import TYPE_CHECKING
from pathlib import Path

import dirmanage

if TYPE_CHECKING:
    from src.options import Options


class PathsKeeper:
    """Wrapper around a dictionary of directories."""

    def __init__(self, options: 'Options'):
        """Initialize."""
        self.options = options
        self.dir_dict: dict[str, Path] = {}
        self.set_defaults()
        self.excluded_dirs = self.create_excluded_dirs()

    def set_defaults(self):
        """Define the default conventional names."""
        config = self.options.config
        self["main"] = dirmanage.base_dir
        self["starting"] = Path(config["starting"])
        self["main_backup_dir"] = Path(config["main_backup_dir"])
        self["bakatot_dir"] = self.build_bakatot()

    def build_bakatot(self):
        """The directory in which we make the copy."""
        config = self.options.config
        name = config["name"]
        dirname = f"bakatot.{name}"
        return self["main_backup_dir"] / dirname

    def create_excluded_dirs(self):
        """The list of directories to be excluded from the backup."""
        ex_dirs = []
        config = self.options.config
        name: str
        for name in config["exclude"]:
            ex_dir = self["starting"] / name
            ex_dirs.append(ex_dir)
        return ex_dirs

    def __setitem__(self, key, value):
        """
        Add the key/value in the dictionary.

        The `value` is assumed to be `posixpath` and
        is resolved.
        """
        full_path = Path(value).resolve()
        self.dir_dict[key] = full_path

    def __getitem__(self, key) -> Path:
        """Retrieve a directory from its name."""
        return self.dir_dict[key]
