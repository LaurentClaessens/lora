"""Some utilities."""

import io
import os
import pickle
import random
import inspect
import datetime
import hashlib
import json
import random
import socket
import string
import sys
import time
import contextlib
from typing import Optional
from typing import Any
from pathlib import Path
from contextlib import contextmanager
import json

from colorama import Fore, Back, Style

from src.stubs import PathOrStr


class ColorOutput:
    """Colored output"""

    def __init__(self, fg: Optional[str] = None, bg: Optional[str] = None):
        """Initialize."""
        self.fg = fg
        self.bg = bg

    def __exit__(self, *args):
        """Reset all the colors"""
        _ = args
        print(Style.RESET_ALL, end="\r")

    def __enter__(self):
        """Initiate the requested color."""
        fg_correspondance = {
            "black": Fore.BLACK,
            "red": Fore.RED,
            "green": Fore.GREEN,
            "yellow": Fore.YELLOW,
            "blue": Fore.BLUE,
            "magenta": Fore.MAGENTA,
            "cyan": Fore.CYAN,
            "white": Fore.WHITE
        }
        bg_correspondance = {
            "black": Back.BLACK,
            "red": Back.RED,
            "green": Back.GREEN,
            "yellow": Back.YELLOW,
            "blue": Back.BLUE,
            "magenta": Back.MAGENTA,
            "cyan": Back.CYAN,
            "white": Back.WHITE}

        if self.fg:
            print(fg_correspondance[self.fg], end="\r")
        if self.bg:
            print(bg_correspondance[self.bg], end="\r")

    def __call__(self, fun):
        """Turn the color outpus to a context manager"""
        def wrapper(*args, **kwargs):
            """Wrap the function."""
            with self:
                return fun(*args, **kwargs)
        return wrapper


def is_port_in_use(port: int) -> bool:
    """Say if something is listening on the given port."""
    HOST = "localhost"
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.settimeout(1)
        try:
            s.connect((HOST, port))
        except socket.error:
            return False
        else:
            return True


def human_filesize(filepath):
    """Return a human readable size of the requested file."""
    size = filepath.stat().st_size
    suffix = "B"
    for unit in ["", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei", "Zi"]:
        if abs(size) < 1024.0:
            return f"{size:3.1f}{unit}{suffix}"
        size /= 1024.0
    return f"{size:.1f}Yi{suffix}"


def human_timestamp(now=None):
    """Return a human readable timestamp."""
    if now is None:
        now = time.time()
    local_time = time.localtime(now)
    return time.strftime("%Z - %A  %Y/%B/%d, %H:%M:%S", local_time)


def filename_timestamp(now=None):
    """Return a human readable timestamp, but ok for filename."""
    if now is None:
        now = time.time()
    local_time = time.localtime(now)
    stamp = time.strftime("%Z-%A_%Y-%B-%d_%Hh%Mm%Ss", local_time)
    return stamp


class Timer:
    """Furnish a context manager for timing the functions."""

    def __init__(self, message,
                 quiet=False, welcome=False,
                 collect=False):
        """Initialize with the main message."""
        self.message = message
        self.init_time = 0
        self.end_time = 0
        self.elapsed = 0
        self.quiet = quiet
        self.welcome = welcome
        self.collect = collect

    def __enter__(self):
        """Will write a message when something is really written."""
        if self.welcome:
            print(f"-- timer -- (start) {self.message} -- "
                  f"{human_timestamp()}")
        self.init_time = time.time()
        return self

    def current(self):
        """Return the elapsed time from the enter."""
        now = time.time()
        return now - self.init_time

    def __call__(self, fun):
        """Make this class a decorator as well."""
        def new_fun(*args, **kwargs):
            with Timer(self.message, welcome=self.welcome, collect=self.collect):
                return fun(*args, **kwargs)
        return new_fun

    def __exit__(self, *args):
        """Compute the elapsed time and print a message."""
        _ = args
        self.elapsed = self.current()
        if not self.quiet:
            print(f"-- timer -- (finish) {self.message} -- {self.elapsed}")


def random_string(length):
    """Return a random string of letters of the given length."""
    rn_list = [random.choice(string.ascii_letters) for _ in range(1, length)]
    return "".join(rn_list)


def ensure_encoded(text, encoding='utf8'):
    """
    Return the encoded text.

    - If it is already 'byte', leave as it.
    - If not, encode.
    """
    try:
        answer = text.encode(encoding)
    except AttributeError:
        answer = text
    return answer


def get_text_hash(text):
    """Return a hash of the given text."""
    sha1 = hashlib.sha1()
    text = ensure_encoded(text, 'utf8')
    sha1.update(text)
    return sha1.hexdigest()


def get_bytes_hash(byte_str: bytes):
    """Return a hash of the given text."""
    result = hashlib.sha1(byte_str)
    hash_value = result.hexdigest()
    return hash_value


def get_file_hash(filename):
    """
    Return a hash of the given file.

    @return {string}
        The hex digest of the content of the file.
    """
    with open(filename, 'rb') as my_file:
        content = my_file.read()
    return get_text_hash(content)


def json_serial(obj):
    """Serialize the datetime."""
    if isinstance(obj, datetime.datetime):
        timestamp = obj.timestamp()
        return human_timestamp(timestamp)
    with contextlib.suppress(AttributeError):
        return obj.to_json()
    if isinstance(obj, Path):
        return str(obj)
    if isinstance(obj, set):
        return list(obj)
    return str(obj)


def read_binary_file(filepath):
    """
    Read the file in memory.

    @return {io.BytesIO}
    """
    data = io.BytesIO()
    with open(filepath, 'rb') as zip_file:
        data.write(zip_file.read())
    return data


def json_to_str(json_dict, pretty=False, ensure_ascii=True):
    """Return a string representation of the given json."""
    if pretty:
        return json.dumps(json_dict,
                          sort_keys=True,
                          indent=4,
                          default=json_serial,
                          ensure_ascii=ensure_ascii)
    return json.dumps(json_dict, default=json_serial, ensure_ascii=ensure_ascii)


def print_json(json_dict, pretty=True, color=None):
    """Print the given json."""
    text = json_to_str(json_dict, pretty, ensure_ascii=False)
    with ColorOutput(color):
        print(text)


def write_json_file(json_dict,
                    filename: PathOrStr,
                    pretty=False,
                    parents=False):
    """Write the dictionary in the given file."""
    filename = Path(filename)
    if parents:
        parent = filename.parent
        parent.mkdir(parents=True, exist_ok=True)
    my_str = json_to_str(json_dict, pretty=pretty)

    filename.write_text(my_str)


def read_json_file(json_path: PathOrStr, default=None, key_type: Optional[type] = None) -> Any:
    """
    Read a json file and return it as dictionsry, list, etc.

    If `key_type` is given, the keys will be converted to the
    requested type.
    """
    json_path = Path(json_path)
    if not json_path.is_file():
        if default is None:
            raise ValueError(f"You try to read {json_path}. "
                             f"The file does not exist and you "
                             f"furnished no default.")
        return default
    with open(json_path, 'r') as json_data:
        try:
            answer = json.load(json_data)
        except json.decoder.JSONDecodeError as err:
            print("JSONDecodeError:", err)
            message = f"Json error in {json_path}:\n {err}"
            raise ValueError(message) from err

    if key_type:
        with Timer(f"convert keys to {key_type}", welcome=True):
            answer = {key_type(key): answer[key] for key in answer}
    return answer


def write_pickle_file(obj, pickle_path: Path):
    """Write the object in a pickle file."""
    pickle_path.write_bytes(pickle.dumps(obj))


def read_pickle_file(pickle_path: Path) -> Any:
    """Read the object from the pickle file."""
    return pickle.loads(pickle_path.read_bytes())


def is_port_free(port: int) -> bool:
    """Say if the given port is free."""
    sock = None
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        result = sock.connect_ex(('localhost', port))
        return result != 0
    except socket.error:
        # An error occurred, so the port is likely free
        return True
    finally:
        if sock:
            sock.close()


def get_free_port(portinuse: Optional[list[int]]):
    """Return a free port."""
    portsinuse: list[int] = portinuse or []
    found_port = False
    num = 0
    while not found_port:
        num += 1
        port = random.randint(49152, 65535)
        if is_port_free(port):
            if port not in portsinuse:
                return port
        if num > 100:
            raise ValueError("No free port found")


def dprint(*args, **kwargs):
    """Print with color for debug purposes."""
    color = kwargs.pop('color', None)
    if not color:
        print(*args, **kwargs)
    if color:
        with ColorOutput(color):
            print(*args, **kwargs)


def ciao(message=None, color=None):
    """For debug only."""
    if color is None:
        color = "yellow"
    if message:
        with ColorOutput("yellow"):
            print("\n", message, "\n")
    x = random.random()
    if x > 3:
        return "pas possible"

    current_frame = inspect.stack()[1]
    current_file = Path(current_frame[1]).resolve()
    current_line = current_frame[2]
    print(f"{current_file}, line {current_line} --> ciao !")
    sys.exit(1)


def deprecation(message=None):
    """Indicate that I think that a function is deprecated."""
    import traceback
    print("Ceci est déprécié.")
    with ColorOutput("magenta"):
        traceback.print_stack(file=sys.stdout)
    ciao(message, color="cyan")


@contextmanager
def ChdirContext(new_path: Path):
    original_path = os.getcwd()
    os.chdir(new_path)
    try:
        yield
    finally:
        os.chdir(original_path)
