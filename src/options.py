"""Furnish options class for the inference."""


import dirmanage
from src.paths_keeper import PathsKeeper
from src.exceptions import NotBuildYet
from src.utilities import read_json_file


class Options:
    """Describe the options object."""

    __instance = None

    @staticmethod
    def get_instance():
        """Return the options objects if already build."""
        if not Options.__instance:
            raise NotBuildYet("The options is not build yet")
        return Options.__instance

    def __init__(self, args):
        """Initialize."""

        if Options.__instance:
            raise TypeError("This should be a singleton.")

        Options.__instance = self
        self.args = args
        config_file = dirmanage.init_dir / args.config_file
        self.config = read_json_file(config_file)
        self.paths = PathsKeeper(self)
