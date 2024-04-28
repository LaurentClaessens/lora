#!venv/bin/python3


import dirmanage
from src.get_args import get_pylora_args
from src.options import Options
from src.utilities_b import CloseThreads
from src.utilities_b import all_files
from src.backup_job import BackupJob
from src.purge_job import PurgeJob
from src.utilities import ciao
from src.utilities import dprint
_ = dirmanage
_ = ciao


def do_backup(options: Options):
    """Make the backup."""
    starting = options.paths["starting"]
    for loc_file in all_files(starting):
        job = BackupJob(loc_file)
        options.jobs_manager.append(job)


def do_purge(options: Options):
    """Make the purge of the backup directory."""
    bak_dir = options.paths["bakatot_dir"]
    for bak_file in all_files(bak_dir):
        job = PurgeJob(bak_file)
        options.jobs_manager.append(job)


def do_work(options: Options):
    """Make backup, then purge."""
    do_backup(options)
    # do_purge(options)


def ask_for_mount(options):
    """Ask the user to mount the backup partition."""
    bak_uuid = options.config["bak_uuid"]
    dev_dir = f"/dev/disk/by-uuid/{bak_uuid}"
    mount_dir = options.paths["main_backup_dir"]
    print(f"sudo mount {dev_dir} {mount_dir}")
    answer = input("ok ?")
    if answer != "y":
        print("I do nothing")
        ciao()


args = get_pylora_args()
options = Options(args)
ask_for_mount(options)
with CloseThreads(options):
    do_work(options)

dprint("remettre la position 911108151")
