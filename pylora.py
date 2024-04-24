#!venv/bin/python3


from pathlib import Path

import dirmanage
from src.get_args import get_pylora_args
from src.options import Options
from src.utilities_b import is_excluded_dir
from src.utilities_b import CloseThreads
from src.utilities_b import all_files
from src.backup_job import BackupJob
from src.utilities import ciao
_ = dirmanage
_ = ciao


def do_work(options: Options):

    tr_path = Path("/home/moky/.pyenv")

    print(is_excluded_dir(tr_path), "         ")

    tst = dirmanage.base_dir / "src" / "utilities.py"

    job = BackupJob(tst)

    print(job.destination)
    for loc_file in all_files(starting):
        job = BackupJob(loc_file)
        options.jobs_manager.append(job)


args = get_pylora_args()
options = Options(args)
starting = options.paths["starting"]
with CloseThreads(options):
    do_work(options)
