#!/bin/bash

python3.11 db-drop.py
python3.11 db-init.py
python3.11 db-init-metadata.py
python3.11 db-faker.py