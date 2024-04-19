"""
The functions which are getting the CLI arguments.
"""


import argparse


def add_standard_arguments(parser):
    """
    Add the standard arguments (prod/project_name, etc.).

    Also set the default values.
    """
    parser.add_argument('--config_file', type=str, required=True)
    return parser


def get_pylora_args():
    """Return the options object based on the command line arguments."""
    parser = argparse.ArgumentParser()
    # parser.add_argument('--port', type=int, default=None)
    add_standard_arguments(parser)
    args = parser.parse_args()
    return args
