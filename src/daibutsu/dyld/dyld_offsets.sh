#!/bin/bash
#set -x
# needs dmg, hfsplus, haxx
devices=(iPhone4,1 iPhone5,1 iPhone5,2 iPhone5,3 iPhone5,4 iPad2,1 iPad2,2 iPad2,3 iPad2,4 iPad2,5 iPad2,6 iPad2,7 iPad3,1 iPad3,2 iPad3,3 iPad3,4 iPad3,5 iPad3,6 iPod5,1)
rv=0
#rm if.c of.c
for device in "${devices[@]}"; do
    echo "$device"
    json=$(curl "https://firmware-keys.ipsw.me/device/$device")
    len=$(echo "$json" | jq length)
    builds=()
    i=0
    while (( i < len )); do
        builds+=($(echo "$json" | jq -r ".[$i].buildid"))
        ((i++))
    done
    for build in "${builds[@]}"; do
        isIOS9=
        case $build in
            "12"* ) :;;
            "13A"* | "13B"* ) isIOS9=1;;
            * ) continue;;
        esac
        echo "$build"
        ipsw="$(curl https://api.ipsw.me/v2.1/$device/$build/filename)"
        ipsw="${ipsw%?????}"
        if [[ ! -e $ipsw.ipsw ]]; then
            url="$(curl https://api.ipsw.me/v2.1/$device/$build/url)"
            curl -LO "$url"
        fi
        try=("https://github.com/LukeZGD/Legacy-iOS-Kit-Keys/raw/master/$device/$build/index.html"
             "https://api.m1sta.xyz/wikiproxy/$device/$build"
             "http://127.0.0.1:8888/firmware/$device/$build")
        device_fw_key=
        for i in "${try[@]}"; do
            device_fw_key="$(curl -L $i)"
            if [[ -n $device_fw_key ]]; then
                break
            fi
        done
        RootName=$(echo "$device_fw_key" | jq -j '.keys[] | select(.image == "RootFS") | .filename')
        RootKey=$(echo "$device_fw_key" | jq -j '.keys[] | select(.image == "RootFS") | .key')
        unzip -o -j "$ipsw.ipsw" $RootName -d .
        ./dmg extract $RootName out.dmg -k $RootKey
        isA6=
        dyld="dyld_shared_cache_armv7"
        case $device in
            iPad3,[456] | iPhone5,[1234] ) isA6=1; dyld+="s";;
        esac
        ./hfsplus out.dmg extract System/Library/Caches/com.apple.dyld/$dyld
        ./haxx $dyld dyld_haxx | tee out
        off=$(cat out)
        ((rv++))
        echo "if(rv == $rv){" | tee -a of.c
        echo "    // ${device}_$build" | tee -a of.c
        echo "$off" | tee -a of.c
        echo -e "    return;\n}" | tee -a of.c
        echo "} else if(!strcmp(argv[1], \"--${device}_$build\")) {" | tee -a if.c
        echo "    rv=$rv;" | tee -a if.c
        [[ -n $isA6 ]] && echo "    isA6=$isA6;" | tee -a if.c
        [[ -n $isIOS9 ]] && echo "    isIOS9=$isIOS9;" | tee -a if.c
        rm $ipsw.ipsw $RootName out.dmg $dyld dyld_haxx
    done
done
