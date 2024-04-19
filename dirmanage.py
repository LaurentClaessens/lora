"""Manage the subdirectory issue."""


import os
import sys
from pathlib import Path

init_dir = Path(os.getcwd()).resolve()
base_dir = init_dir
sys.path.append(os.getcwd())
