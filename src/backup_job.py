"""A file to be backuped"""


import os
import shutil
from pathlib import Path

from src.getters import get_paths
from src.getters import get_options
from src.exceptions import MtimeError
from src.utilities import human_timestamp
from src.utilities import dprint
from src.utilities import ciao
_ = ciao


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

        # round the time because it seems that the
        # copied mtime is rounded.
        # source:  1690263067.3967152
        # backup : 1690263067.0
        src_mtime = int(os.path.getmtime(self.source))
        dest_mtime = int(os.path.getmtime(self.destination))
        if src_mtime > dest_mtime:
            dprint(f"ok pour {self.source}")
            dprint("parce que :")
            dprint("  src : ", src_mtime, human_timestamp(src_mtime))
            dprint("  dst : ", dest_mtime, human_timestamp(dest_mtime))
            return True
        if src_mtime != dest_mtime:
            text = f"Problem with mtime of "\
                   f"{self.source} Vs {self.destination}"
            raise MtimeError(text)
        src_size = os.path.getsize(self.source)
        dst_size = os.path.getsize(self.destination)
        if src_size != dst_size:
            return True
        return False

    def purge_dest(self):
        """Move the destination file to its purge."""
        purge = self.paths.get_mod_purge(self.source)
        dprint(f"  mod : {purge}")
        purge.parent.mkdir(parents=True, exist_ok=True)
        shutil.move(self.destination, purge)

    def run(self):
        """Run the job: makes the copy."""
        qsize = self.options.jobs_manager.qsize()
        dprint(f"run bak job ({qsize})")
        dprint(f"  src : {self.source}")
        dprint(f"  dst : {self.destination}")
        if "ZZ" in str(self.source):
            self.options.close_threads()
            ciao()
        if self.destination.is_file():
            self.purge_dest()
        dprint("   bakup")
        self.destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(self.source, self.destination)
