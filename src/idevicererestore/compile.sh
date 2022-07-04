#!/bin/bash

if [[ $OSTYPE == "linux"* ]]; then
    platform="linux"
    echo "* Platform: Linux"
    sudo apt update
    sudo apt install -y git

elif [[ $OSTYPE == "msys" ]]; then
    platform="win"
    echo "* Platform: Windows MSYS2"
    pacman -Syu --needed --noconfirm git

else
    echo "[Error] Unsupported platform for compile script"
    exit 1
fi

git clone https://github.com/LukeeGD/idevicerestore
rm -f idevicerestore/src/*
cp *.am *.c *.h idevicerestore/src
cd idevicerestore
./compile.sh
exit $?
