#!/bin/bash

VENV_NAME=filter_venv

if [ -d $VENV_NAME ]; then rm -rf $VENV_NAME; fi

python3 -m venv $VENV_NAME
source $VENV_NAME/bin/activate
python -m pip install -r requirements.txt
deactivate

echo "Venv created under: $VENV_NAME"
