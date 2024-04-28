"""A file to be backuped"""


import shutil
from pathlib import Path

from src.getters import get_paths
from src.getters import get_options
from src.utilities_b import manage_crashs
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
        local_file = self.paths.bak_to_local(self.source)
        if not local_file.is_file():
            print("ok purge pour")
            print("  bak", self.source)
            print("  loc", local_file)
            print("  dst", self.destination)
            print("je dirais que oui")
        return False

    @manage_crashs
    def run(self):
        """Run the job: makes the copy."""
        qsize = self.options.jobs_manager.qsize()
        dprint(f"run purge job ({qsize})")
        dprint(f" src: {self.source}")
        dprint(f" dst: {self.destination}")
        self.destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.move(self.source, self.destination)
