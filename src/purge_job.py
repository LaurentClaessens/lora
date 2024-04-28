"""A file to be backuped"""


import shutil
from pathlib import Path

from src.getters import get_paths
from src.getters import get_options
from src.utilities import dprint
_ = shutil
_ = dprint


class PurgeJob:

    def __init__(self, bak_file: Path):
        """Initialize."""
        self.options = get_options()
        self.paths = get_paths()
        self.source = bak_file
        self.destination = self.paths.get_del_purge(self.source)

    def has_to_to(self):
        """Say if self is a job that has to be done."""
        return True

    def run(self):
        """Run the job: makes the copy."""
        raise NotImplemented
