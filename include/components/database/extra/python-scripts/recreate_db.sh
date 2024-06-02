#!/bin/bash

python db-drop.py
python db-init.py
python db-init-metadata.py
python db-faker.py