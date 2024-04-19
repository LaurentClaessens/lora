"""Wrap around a list of files to be backuped."""

from src.backup_job import BackupJob


class State:

    def __init__(self):
        """Initialize."""
        self.backup_jobs:  list[BackupJob] = []
