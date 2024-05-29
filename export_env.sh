#!/bin/bash

ENV_FILE=".env"

if [ ! -f "$ENV_FILE" ]; then
  echo "Файл $ENV_FILE не найден"
  exit 1
fi

export $(grep -v '^#' "$ENV_FILE" | xargs)

./build/main