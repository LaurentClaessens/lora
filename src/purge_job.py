"""A file to be purged because deleted."""


from pathlib import Path


class PurgeJob:

    def __init__(self, bak_file: Path):
        self.bak_file = bak_file

    def has_to_to(self):
        raise NotImplemented

    def run(self):
        raise NotImplemented
