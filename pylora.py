#!venv/bin/python3

import dirmanage
from src.get_args import get_pylora_args
from src.options import Options
from src.utilities_b import is_excluded_dir
from src.backup_job import BackupJob
_ = dirmanage


args = get_pylora_args()
options = Options(args)

tr_path = dirmanage.init_dir / "venv"

print(is_excluded_dir(tr_path))

tst = dirmanage.base_dir / "src" / "utilities.py"

job = BackupJob(tst)

print(job.destination)
