#!/bin/bash
set -x

if [[ ! $(which ldid) ]]; then
    echo "[Error] No ldid. Get from https://github.com/ProcursusTeam/ldid/releases"
    exit 1
fi

if [[ $1 == "live" ]]; then
    live="-DLIVE"
fi

xcrun -sdk iphoneos gcc -arch armv7 haxx_overwrite.c -o haxx_overwrite $live
ldid -S haxx_overwrite
