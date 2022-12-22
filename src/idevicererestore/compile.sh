#!/bin/bash
set -e

if [[ $OSTYPE == "linux"* ]]; then
    platform="linux"
    echo "* Platform: Linux"
    sudo apt update
    sudo apt install -y git

elif [[ $OSTYPE == "msys" ]]; then
    platform="win"
    echo "* Platform: Windows MSYS2"
    pacman -Syu --needed --noconfirm git patch
    patch idevicerestore.c < idevicerestore.patch
    patch restore.c < restore.patch

else
    echo "[Error] Unsupported platform for compile script"
    exit 1
fi

git clone https://github.com/LukeZGD/idevicerestore
rm -f idevicerestore/src/*
cp *.am *.c *.h idevicerestore/src
cd idevicerestore
./compile.sh
exit $?
