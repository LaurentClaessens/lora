#!/bin/bash

set -u

MAIN_DIR=$(pwd)
VENV_DIR="$MAIN_DIR/venv"
BIN_DIR="$VENV_DIR/bin"
PYTHON_VERSION=3.10.6
PYTHON3="$HOME/.pyenv/versions/$PYTHON_VERSION/bin/python3"



function install_pyenv()
{
	if [ -e "$PYTHON3" ]; then
  	echo "The pyenv binary $PYTHON3 exists."
		return
  fi

	git clone https://github.com/pyenv/pyenv.git ~/.pyenv
	cd ~/.pyenv
	git pull
	yes n | ~/.pyenv/bin/pyenv install -v $PYTHON_VERSION
}

function create_venv()
{
	echo "Creating the virtual environment"
	"$PYTHON3" -m venv "$VENV_DIR"

	cd "$BIN_DIR" || exit 1
	./python3 -m pip install --upgrade pip
}

function pip_install()
{
	cd "$BIN_DIR" || exit 1
	./pip3 install -r "$MAIN_DIR/requirements.txt"
	# ./pip3 freeze
}


install_pyenv
create_venv
pip_install
