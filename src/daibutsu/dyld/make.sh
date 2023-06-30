#!/bin/bash
# Tested with Xcode 5.1.1

if [[ ! $(which ldid) ]]; then
    git clone https://github.com/xerub/ldid
    pushd ldid
    ./make.sh
    sudo mkdir -p /usr/local/bin
    sudo cp ldid /usr/local/bin
    popd
    rm -rf ldid
fi

xcrun -sdk iphoneos gcc -arch armv7 haxx_overwrite.c -o haxx_overwrite
ldid -S haxx_overwrite
