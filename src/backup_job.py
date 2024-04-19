"""A file to be backuped"""


from pathlib import Path

from src.getters import get_paths


class BackupJob:

    def __init__(self, local_file: Path):
        self.source = local_file
        self.destination = self.build_dest()

    def build_dest(self):
        """Return the destination path."""
        paths = get_paths()
        starting = paths["starting"]
        rel_path = self.source.relative_to(starting)
        return paths["bakatot_dir"] / rel_path
