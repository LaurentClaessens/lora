"""Some getters"""

from typing import TYPE_CHECKING

from src.paths_keeper import PathsKeeper

if TYPE_CHECKING:
    from src.options import Options


def get_options() -> 'Options':
    """Return the current options object."""
    from src.options import Options
    return Options.get_instance()


def get_paths() -> PathsKeeper:
    """return the path keeper."""
    options = get_options()
    return options.paths
