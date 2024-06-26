#!/bin/bash

arg="ipsw"
cmake=/usr/bin/cmake
export JNUM="-j$(nproc)"

for i in "$@"; do
    if [[ $i == "help" ]]; then
        echo "Usage: $0 <all> <help> <undo>"
        echo "    <all>: All xpwn binaries"
        echo "    <help>: Display this help prompt"
        echo "    <undo>: Undo preparation (macOS only)"
        exit 0
    elif [[ $i == "all" ]]; then
        echo "* Build all"
        arg=
    fi
done

prepare() {
    if [[ $OSTYPE == "darwin"* ]]; then
        platform="macos"
        echo "* Platform: macOS"
        port=/opt/local/bin/port
        lib=/opt/local/lib
        cmake=/opt/local/bin/cmake

        if [[ $1 == undo ]]; then
            sudo mv ${lib}2/* ${lib}
            sudo rm -rf ${lib}2
            exit 0
        elif [[ ! -d ${lib}2 ]]; then
            if [[ ! -e $port ]]; then
                echo "MacPorts not installed!"
                exit 1
            fi
            sudo $port install -N zlib +universal
            sudo $port install -N openssl +universal
            sudo $port install -N bzip2 +universal
            sudo $port install -N libpng +universal
            sudo $port install -N cmake
            sudo mkdir ${lib}2
            sudo mv $lib/libbz2.dylib $lib/libcrypto.dylib $lib/libz.dylib $lib/libpng*.dylib $lib/libssl*.dylib ${lib}2
        fi

    elif [[ $OSTYPE == "linux"* ]]; then
        sslver="1.1.1s"
        platform="linux"
        echo "* Platform: Linux"
        . /etc/os-release
        if [[ $ID == "arch" || $ID_LIKE == "arch" ]]; then
            return
        elif [[ ! -f "/etc/lsb-release" && ! -f "/etc/debian_version" ]]; then
            echo "[Error] Ubuntu/Debian only"
            exit 1
        fi
        export BEGIN_LDFLAGS="-Wl,--allow-multiple-definition"
        export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:/usr/lib/x86_64-linux-gnu/pkgconfig

        if [[ ! -e /usr/local/lib/libbz2.a || ! -e /usr/local/lib/libz.a ||
            ! -e /usr/local/lib/libcrypto.a || ! -e /usr/local/lib/libssl.a ]]; then
        #if [[ ! -e /usr/local/lib/libbz2.a || ! -e /usr/local/lib/libz.a ]]; then
            sudo apt update
            sudo apt remove -y libssl-dev
            sudo apt install -y pkg-config libtool automake g++ cmake git libusb-1.0-0-dev libreadline-dev libpng-dev git autopoint aria2 ca-certificates

            mkdir tmp
            cd tmp
            git clone https://github.com/madler/zlib
            aria2c https://sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz
            aria2c https://www.openssl.org/source/openssl-$sslver.tar.gz

            tar -zxvf bzip2-1.0.8.tar.gz
            cd bzip2-1.0.8
            make $JNUM LDFLAGS="$BEGIN_LDFLAGS"
            sudo make install
            cd ..

            cd zlib
            ./configure --static
            make $JNUM LDFLAGS="$BEGIN_LDFLAGS"
            sudo make install
            cd ..

            #: '
            tar -zxvf openssl-$sslver.tar.gz
            cd openssl-$sslver
            if [[ $(uname -m) == "a"* && $(getconf LONG_BIT) == 64 ]]; then
                ./Configure no-ssl3-method linux-aarch64 "-Wa,--noexecstack -fPIC"
            elif [[ $(uname -m) == "a"* ]]; then
                ./Configure no-ssl3-method linux-generic32 "-Wa,--noexecstack -fPIC"
            else
                ./Configure no-ssl3-method enable-ec_nistp_64_gcc_128 linux-x86_64 "-Wa,--noexecstack -fPIC"
            fi
            make $JNUM depend
            make $JNUM
            sudo make install_sw install_ssldirs
            sudo rm -rf /usr/local/lib/libcrypto.so* /usr/local/lib/libssl.so*
            cd ..
            #'

            cd ..
            rm -rf tmp
        fi

    elif [[ $OSTYPE == "msys" ]]; then
        platform="win"
        echo "* Platform: Windows MSYS2"

        if [[ ! -e /usr/lib/libpng.a ]]; then
            echo "* Note that if your msys-runtime is outdated, MSYS2 prompt may close after updating."
            echo "* If this happens, reopen the MSYS2 prompt and run the script again"
            pacman -Syu --noconfirm --needed cmake git libbz2-devel make msys2-devel openssl-devel patch zip zlib-devel
            mkdir tmp
            cd tmp
            git clone https://github.com/glennrp/libpng
            cd libpng
            ./configure
            make $JNUM
            make install
            cd ../..
            rm -rf tmp
        fi

    else
        echo "[Error] Unsupported platform"
        exit 1
    fi
}

build() {
    if [[ $platform == "win" ]]; then
        cd ipsw-patch
        patch main.c < main.patch
        cd ..
    fi

    rm -rf bin new
    mkdir bin new
    cd new
    $cmake ..
    make $JNUM $arg

    if [[ $1 == "all" ]]; then
        cp common/libcommon.a ../bin
        cp dmg/dmg ../bin
        cp hdutil/hdutil ../bin
        cp hfs/hfsplus ../bin
        cp ipsw-patch/imagetool ../bin
        cp ipsw-patch/ipsw ../bin
        cp ipsw-patch/ticket ../bin
        cp ipsw-patch/validate ../bin
        cp ipsw-patch/xpwntool ../bin
        cp ipsw-patch/libxpwn.a ../bin
        if [[ $platform != "win" ]]; then
            cd ..
            rm -rf new
            echo "Done! Builds at bin/"
            exit 0
        fi
    else
        cp ipsw-patch/ipsw ../bin/ipsw_$platform
    fi
    cd ..

    if [[ $platform == "win" ]]; then
        rm -rf new/*
        cd ipsw-patch
        patch -R main.c < main.patch
        cd ../new
        $cmake ..
        make $JNUM ipsw
        cp ipsw-patch/ipsw ../bin/ipsw2
        cd ..
    fi

    rm -rf new
    echo "Done! Build at bin/ipsw_$platform"
}

prepare $1
build $1
