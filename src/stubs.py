from typing import TYPE_CHECKING
from typing import Union

from pathlib import Path


if TYPE_CHECKING:
    from src.purge_job import PurgeJob
    from src.backup_job import BackupJob

PathOrStr = Union[Path, str]
LoraJob = Union['BackupJob', 'PurgeJob']
