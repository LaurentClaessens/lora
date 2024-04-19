"""Some getters"""


from src.options import Options
from src.paths_keeper import PathsKeeper


def get_options() -> Options:
    """Return the current options object."""
    return Options.get_instance()


def get_paths() -> PathsKeeper:
    """return the path keeper."""
    options = get_options()
    return options.paths
