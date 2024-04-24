"""A file to be backuped"""


import os
import shutil
from pathlib import Path

from src.getters import get_paths
from src.getters import get_options
from src.exceptions import MtimeError
from src.utilities import dprint
_ = shutil


class BackupJob:

    def __init__(self, local_file: Path):
        """Initialize."""
        self.options = get_options()
        self.paths = get_paths()
        self.source = local_file
        self.destination = self.paths.get_bak_path(self.source)

    def has_to_to(self):
        """
        Say if self is a job that has to be done.

        It has to be done if
        - the destination does not exists
        - the destination and the course are not identical.
        """
        if not self.destination.is_file():
            return True
        src_mtime = os.path.getmtime(self.source)
        dest_mtime = os.path.getmtime(self.destination)
        if src_mtime > dest_mtime:
            return True
        if src_mtime != dest_mtime:
            raise MtimeError(
                "Problem with mtime of {self.source} Vs {self.destination}")
        src_size = os.path.getsize(self.source)
        dst_size = os.path.getsize(self.destination)
        if src_size != dst_size:
            return True
        return False

    def purge_dest(self):
        """Move the destination file to its purge."""
        purge = self.paths.get_mod_purge(self.source)
        dprint(f"  mod : {purge}")
        #shutil.move(self.destination, purge)

    def run(self):
        """Run the job: makes the copy."""
        qsize = self.options.jobs_manager.qsize()
        dprint(f"run bak job ({qsize})")
        dprint(f"  src : {self.source}")
        dprint(f"  dst : {self.destination}")
        if self.destination.is_file():
            self.purge_dest()
        dprint("   bakup")
        #shutil.copy2(self.source, self.destination)
