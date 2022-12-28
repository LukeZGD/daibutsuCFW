#!/bin/bash
set -e

export JNUM="-j$(nproc)"

if [[ $OSTYPE == "linux"* ]]; then
    platform="linux"
    echo "* Platform: Linux"
    sudo apt update
    sudo apt install -y git

    export ALT_CONF_ARGS="--disable-dependency-tracking --disable-silent-rules --prefix=/usr/local"
    if [[ $(uname -m) == "a"* ]]; then
        export CC_ARGS="CC=/usr/bin/gcc CXX=/usr/bin/g++ LD=/usr/bin/ld RANLIB=/usr/bin/ranlib AR=/usr/bin/ar"
    else
        export CC_ARGS="CC=/usr/bin/clang-14 CXX=/usr/bin/clang++-14 LD=/usr/bin/ld64.lld-14 RANLIB=/usr/bin/ranlib AR=/usr/bin/ar"
        export LD_ARGS="-Wl,--allow-multiple-definition -L/usr/lib/x86_64-linux-gnu -lzstd -llzma -lbz2"
    fi

    if [[ $(uname -m) == "a"* && $(getconf LONG_BIT) == 64 ]]; then
        export LD_ARGS="-Wl,--allow-multiple-definition -L/usr/lib/aarch64-linux-gnu -lzstd -llzma -lbz2"
    elif [[ $(uname -m) == "a"* ]]; then
        export LD_ARGS="-Wl,--allow-multiple-definition -L/usr/lib/arm-linux-gnueabihf -lzstd -llzma -lbz2"
    fi


elif [[ $OSTYPE == "msys" ]]; then
    platform="win"
    echo "* Platform: Windows MSYS2"
    pacman -Syu --needed --noconfirm git patch

    export STATIC_FLAG="--enable-static --disable-shared"
    export BEGIN_LDFLAGS="-Wl,--allow-multiple-definition -all-static"

else
    echo "[Error] Unsupported platform for compile script"
    exit 1
fi

git clone https://github.com/LukeZGD/idevicerestore
rm -f idevicerestore/src/*
cp *.am *.c *.h *.patch idevicerestore/src
cd idevicerestore
./compile.sh ssl3
mv bin/idevicerestore_$platform bin/idevicererestore_$platform

if [[ $1 == "patch" ]]; then
    cd src
    patch idevicerestore.c < idevicerestore.patch
    patch restore.c < restore.patch
    cd ..
    make clean
    if [[ $platform == "linux" ]]; then
        ./autogen.sh $ALT_CONF_ARGS $CC_ARGS LDFLAGS="$LD_ARGS" LIBS="-ldl"
        make $JNUM
        cp src/idevicerestore bin/idevicererestore_${platform}_patch
    elif [[ $platform == "win" ]]; then
        ./autogen.sh $STATIC_FLAG
        export curl_LIBS="$(curl-config --static-libs)"
        make $JNUM install CFLAGS="-DCURL_STATICLIB" LDFLAGS="$BEGIN_LDFLAGS" LIBS="-llzma -lbz2 -lbcrypt"
        cp /mingw64/bin/idevicerestore bin/idevicererestore_${platform}_patch
    fi
fi

exit $?
