/* haxx_overwrite.c - dyld_shared_cache hack for daibutsu jailbreak
 * This is used in pangu 9 (9.0-9.1), and fix in 9.2
 * copyright (c) 2021/04/12 dora2ios
 * license : Anyone but do not abuse.
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/stat.h>

struct dyld_cache_header
{
    char        magic[16];              // e.g. "dyld_v0    i386"
    uint32_t    mappingOffset;          // file offset to first dyld_cache_mapping_info
    uint32_t    mappingCount;           // number of dyld_cache_mapping_info entries
    uint32_t    imagesOffset;           // file offset to first dyld_cache_image_info
    uint32_t    imagesCount;            // number of dyld_cache_image_info entries
    uint64_t    dyldBaseAddress;        // base address of dyld when cache was built
    uint64_t    codeSignatureOffset;    // file offset of code signature blob
    uint64_t    codeSignatureSize;      // size of code signature blob (zero means to end of file)
    uint64_t    slideInfoOffset;        // file offset of kernel slid info
    uint64_t    slideInfoSize;          // size of kernel slid info
    uint64_t    localSymbolsOffset;     // file offset of where local symbols are stored
    uint64_t    localSymbolsSize;       // size of local symbols information
    uint8_t     uuid[16];               // unique value for each shared cache file
};

struct dyld_cache_mapping_info {
    uint64_t    address;
    uint64_t    size;
    uint64_t    fileOffset;
    uint32_t    maxProt;
    uint32_t    initProt;
};

struct dyld_cache_image_info
{
    uint64_t    address;
    uint64_t    modTime;
    uint64_t    inode;
    uint32_t    pathFileOffset;
    uint32_t    pad;
};


uint64_t exportTableOffset;
uint64_t MISValidateSignature;
uint64_t MOV_R0_0__BX_LR;
int isIOS9=0;
// no idea

void offset_init(int rv){
    if(rv == 1){
        // iPhone4,1_12A365
        MISValidateSignature = 0x2f0c6090;
        MOV_R0_0__BX_LR = 0x2f0c495e;
        exportTableOffset = 0x1275e5dd;
        return;
    }
    if(rv == 2){
        // iPhone4,1_12A402
        MISValidateSignature = 0x2f0c4090;
        MOV_R0_0__BX_LR = 0x2f0c295e;
        exportTableOffset = 0x1275e5d9;
        return;
    }
    if(rv == 3){
        // iPhone4,1_12A405
        MISValidateSignature = 0x2f0c4090;
        MOV_R0_0__BX_LR = 0x2f0c295e;
        exportTableOffset = 0x1275e5d9;
        return;
    }
    if(rv == 4){
        // iPhone4,1_12B411
        MISValidateSignature = 0x2f1cb090;
        MOV_R0_0__BX_LR = 0x2f1c995e;
        exportTableOffset = 0x128816ef;
        return;
    }
    if(rv == 5){
        // iPhone4,1_12B435
        MISValidateSignature = 0x2f1e0090;
        MOV_R0_0__BX_LR = 0x2f1de95e;
        exportTableOffset = 0x12899995;
        return;
    }
    if(rv == 6){
        // iPhone4,1_12B440
        MISValidateSignature = 0x2f1e1090;
        MOV_R0_0__BX_LR = 0x2f1df95e;
        exportTableOffset = 0x12899995;
        return;
    }
    if(rv == 7){
        // iPhone4,1_12B466
        MISValidateSignature = 0x2f1e1090;
        MOV_R0_0__BX_LR = 0x2f1df95e;
        exportTableOffset = 0x128999d1;
        return;
    }
    if(rv == 8){
        // iPhone4,1_12D508
        MISValidateSignature = 0x2f694ccc;
        MOV_R0_0__BX_LR = 0x2f69356a;
        exportTableOffset = 0x1308b729;
        return;
    }
    if(rv == 9){
        // iPhone4,1_12F70
        MISValidateSignature = 0x2fa30ca0;
        MOV_R0_0__BX_LR = 0x2fa2f506;
        exportTableOffset = 0x132ca533;
        return;
    }
    if(rv == 10){
        // iPhone4,1_12H143
        MISValidateSignature = 0x2fe3dca0;
        MOV_R0_0__BX_LR = 0x2fe3c506;
        exportTableOffset = 0x137e7734;
        return;
    }
    if(rv == 11){
        // iPhone4,1_12H321
        MISValidateSignature = 0x2fe47ca0;
        MOV_R0_0__BX_LR = 0x2fe46506;
        exportTableOffset = 0x137f3e85;
        return;
    }
    if(rv == 12){
        // iPhone5,1_12A365
        MISValidateSignature = 0x2f02b0b4;
        MOV_R0_0__BX_LR = 0x2f029982;
        exportTableOffset = 0x1262e8a9;
        return;
    }
    if(rv == 13){
        // iPhone5,1_12A402
        MISValidateSignature = 0x2f02d0b4;
        MOV_R0_0__BX_LR = 0x2f02b982;
        exportTableOffset = 0x1262e8a9;
        return;
    }
    if(rv == 14){
        // iPhone5,1_12A405
        MISValidateSignature = 0x2f02d0b4;
        MOV_R0_0__BX_LR = 0x2f02b982;
        exportTableOffset = 0x1262e8a9;
        return;
    }
    if(rv == 15){
        // iPhone5,1_12B411
        MISValidateSignature = 0x2f1330b4;
        MOV_R0_0__BX_LR = 0x2f131982;
        exportTableOffset = 0x1275198b;
        return;
    }
    if(rv == 16){
        // iPhone5,1_12B435
        MISValidateSignature = 0x2f1410b4;
        MOV_R0_0__BX_LR = 0x2f13f982;
        exportTableOffset = 0x12761c42;
        return;
    }
    if(rv == 17){
        // iPhone5,1_12B440
        MISValidateSignature = 0x2f1420b4;
        MOV_R0_0__BX_LR = 0x2f140982;
        exportTableOffset = 0x12761c42;
        return;
    }
    if(rv == 18){
        // iPhone5,1_12B466
        MISValidateSignature = 0x2f1440b4;
        MOV_R0_0__BX_LR = 0x2f142982;
        exportTableOffset = 0x12765c82;
        return;
    }
    if(rv == 19){
        // iPhone5,1_12D508
        MISValidateSignature = 0x2f833cf4;
        MOV_R0_0__BX_LR = 0x2f832592;
        exportTableOffset = 0x13207b65;
        return;
    }
    if(rv == 20){
        // iPhone5,1_12F70
        MISValidateSignature = 0x2fc6ecc8;
        MOV_R0_0__BX_LR = 0x2fc6d52e;
        exportTableOffset = 0x1350f6ab;
        return;
    }
    if(rv == 21){
        // iPhone5,1_12H143
        MISValidateSignature = 0x30075cc8;
        MOV_R0_0__BX_LR = 0x3007452e;
        exportTableOffset = 0x13a24188;
        return;
    }
    if(rv == 22){
        // iPhone5,1_12H321
        MISValidateSignature = 0x30082cc8;
        MOV_R0_0__BX_LR = 0x3008152e;
        exportTableOffset = 0x13a3092d;
        return;
    }
    if(rv == 23){
        // iPhone5,2_12A365
        MISValidateSignature = 0x2f02b0b4;
        MOV_R0_0__BX_LR = 0x2f029982;
        exportTableOffset = 0x1262e8a9;
        return;
    }
    if(rv == 24){
        // iPhone5,2_12A402
        MISValidateSignature = 0x2f02d0b4;
        MOV_R0_0__BX_LR = 0x2f02b982;
        exportTableOffset = 0x1262e8a9;
        return;
    }
    if(rv == 25){
        // iPhone5,2_12A405
        MISValidateSignature = 0x2f02d0b4;
        MOV_R0_0__BX_LR = 0x2f02b982;
        exportTableOffset = 0x1262e8a9;
        return;
    }
    if(rv == 26){
        // iPhone5,2_12B411
        MISValidateSignature = 0x2f1330b4;
        MOV_R0_0__BX_LR = 0x2f131982;
        exportTableOffset = 0x1275198b;
        return;
    }
    if(rv == 27){
        // iPhone5,2_12B435
        MISValidateSignature = 0x2f1410b4;
        MOV_R0_0__BX_LR = 0x2f13f982;
        exportTableOffset = 0x12761c42;
        return;
    }
    if(rv == 28){
        // iPhone5,2_12B440
        MISValidateSignature = 0x2f1420b4;
        MOV_R0_0__BX_LR = 0x2f140982;
        exportTableOffset = 0x12761c42;
        return;
    }
    if(rv == 29){
        // iPhone5,2_12B466
        MISValidateSignature = 0x2f1440b4;
        MOV_R0_0__BX_LR = 0x2f142982;
        exportTableOffset = 0x12765c82;
        return;
    }
    if(rv == 30){
        // iPhone5,2_12D508
        MISValidateSignature = 0x2f833cf4;
        MOV_R0_0__BX_LR = 0x2f832592;
        exportTableOffset = 0x13207b65;
        return;
    }
    if(rv == 31){
        // iPhone5,2_12F70
        MISValidateSignature = 0x2fc6ecc8;
        MOV_R0_0__BX_LR = 0x2fc6d52e;
        exportTableOffset = 0x1350f6ab;
        return;
    }
    if(rv == 32){
        // iPhone5,2_12H143
        MISValidateSignature = 0x30075cc8;
        MOV_R0_0__BX_LR = 0x3007452e;
        exportTableOffset = 0x13a24188;
        return;
    }
    if(rv == 33){
        // iPhone5,2_12H321
        MISValidateSignature = 0x30082cc8;
        MOV_R0_0__BX_LR = 0x3008152e;
        exportTableOffset = 0x13a3092d;
        return;
    }
    if(rv == 34){
        // iPhone5,3_12A365
        MISValidateSignature = 0x2f06b0b4;
        MOV_R0_0__BX_LR = 0x2f069982;
        exportTableOffset = 0x12676e99;
        return;
    }
    if(rv == 35){
        // iPhone5,3_12A402
        MISValidateSignature = 0x2f06d0b4;
        MOV_R0_0__BX_LR = 0x2f06b982;
        exportTableOffset = 0x12676e99;
        return;
    }
    if(rv == 36){
        // iPhone5,3_12A405
        MISValidateSignature = 0x2f06d0b4;
        MOV_R0_0__BX_LR = 0x2f06b982;
        exportTableOffset = 0x12676e99;
        return;
    }
    if(rv == 37){
        // iPhone5,3_12B411
        MISValidateSignature = 0x2f1720b4;
        MOV_R0_0__BX_LR = 0x2f170982;
        exportTableOffset = 0x12795f7b;
        return;
    }
    if(rv == 38){
        // iPhone5,3_12B435
        MISValidateSignature = 0x2f1800b4;
        MOV_R0_0__BX_LR = 0x2f17e982;
        exportTableOffset = 0x127aa232;
        return;
    }
    if(rv == 39){
        // iPhone5,3_12B440
        MISValidateSignature = 0x2f1810b4;
        MOV_R0_0__BX_LR = 0x2f17f982;
        exportTableOffset = 0x127aa232;
        return;
    }
    if(rv == 40){
        // iPhone5,3_12B466
        MISValidateSignature = 0x2f1830b4;
        MOV_R0_0__BX_LR = 0x2f181982;
        exportTableOffset = 0x127ae272;
        return;
    }
    if(rv == 41){
        // iPhone5,3_12D508
        MISValidateSignature = 0x2f872cf4;
        MOV_R0_0__BX_LR = 0x2f871592;
        exportTableOffset = 0x13250155;
        return;
    }
    if(rv == 42){
        // iPhone5,3_12F70
        MISValidateSignature = 0x2ff0ccc8;
        MOV_R0_0__BX_LR = 0x2ff0b52e;
        exportTableOffset = 0x137c8c19;
        return;
    }
    if(rv == 43){
        // iPhone5,3_12H143
        MISValidateSignature = 0x30313cc8;
        MOV_R0_0__BX_LR = 0x3031252e;
        exportTableOffset = 0x13cdd214;
        return;
    }
    if(rv == 44){
        // iPhone5,3_12H321
        MISValidateSignature = 0x30320cc8;
        MOV_R0_0__BX_LR = 0x3031f52e;
        exportTableOffset = 0x13ce59b9;
        return;
    }
    if(rv == 45){
        // iPhone5,4_12A365
        MISValidateSignature = 0x2f06b0b4;
        MOV_R0_0__BX_LR = 0x2f069982;
        exportTableOffset = 0x12676e99;
        return;
    }
    if(rv == 46){
        // iPhone5,4_12A402
        MISValidateSignature = 0x2f06d0b4;
        MOV_R0_0__BX_LR = 0x2f06b982;
        exportTableOffset = 0x12676e99;
        return;
    }
    if(rv == 47){
        // iPhone5,4_12A405
        MISValidateSignature = 0x2f06d0b4;
        MOV_R0_0__BX_LR = 0x2f06b982;
        exportTableOffset = 0x12676e99;
        return;
    }
    if(rv == 48){
        // iPhone5,4_12B411
        MISValidateSignature = 0x2f1720b4;
        MOV_R0_0__BX_LR = 0x2f170982;
        exportTableOffset = 0x12795f7b;
        return;
    }
    if(rv == 49){
        // iPhone5,4_12B435
        MISValidateSignature = 0x2f1800b4;
        MOV_R0_0__BX_LR = 0x2f17e982;
        exportTableOffset = 0x127aa232;
        return;
    }
    if(rv == 50){
        // iPhone5,4_12B440
        MISValidateSignature = 0x2f1810b4;
        MOV_R0_0__BX_LR = 0x2f17f982;
        exportTableOffset = 0x127aa232;
        return;
    }
    if(rv == 51){
        // iPhone5,4_12B466
        MISValidateSignature = 0x2f1830b4;
        MOV_R0_0__BX_LR = 0x2f181982;
        exportTableOffset = 0x127ae272;
        return;
    }
    if(rv == 52){
        // iPhone5,4_12D508
        MISValidateSignature = 0x2f872cf4;
        MOV_R0_0__BX_LR = 0x2f871592;
        exportTableOffset = 0x13250155;
        return;
    }
    if(rv == 53){
        // iPhone5,4_12F70
        MISValidateSignature = 0x2ff0ccc8;
        MOV_R0_0__BX_LR = 0x2ff0b52e;
        exportTableOffset = 0x137c8c19;
        return;
    }
    if(rv == 54){
        // iPhone5,4_12H143
        MISValidateSignature = 0x30313cc8;
        MOV_R0_0__BX_LR = 0x3031252e;
        exportTableOffset = 0x13cdd214;
        return;
    }
    if(rv == 55){
        // iPhone5,4_12H321
        MISValidateSignature = 0x30320cc8;
        MOV_R0_0__BX_LR = 0x3031f52e;
        exportTableOffset = 0x13ce59b9;
        return;
    }
    if(rv == 56){
        // iPad2,1_12A365
        MISValidateSignature = 0x2f304090;
        MOV_R0_0__BX_LR = 0x2f30295e;
        exportTableOffset = 0x129e69f9;
        return;
    }
    if(rv == 57){
        // iPad2,1_12A402
        MISValidateSignature = 0x2f302090;
        MOV_R0_0__BX_LR = 0x2f30095e;
        exportTableOffset = 0x129e29f5;
        return;
    }
    if(rv == 58){
        // iPad2,1_12A405
        MISValidateSignature = 0x2f302090;
        MOV_R0_0__BX_LR = 0x2f30095e;
        exportTableOffset = 0x129e29f5;
        return;
    }
    if(rv == 59){
        // iPad2,1_12B410
        MISValidateSignature = 0x2f40d090;
        MOV_R0_0__BX_LR = 0x2f40b95e;
        exportTableOffset = 0x12b0dc1b;
        return;
    }
    if(rv == 60){
        // iPad2,1_12B435
        MISValidateSignature = 0x2f421090;
        MOV_R0_0__BX_LR = 0x2f41f95e;
        exportTableOffset = 0x12b25ec1;
        return;
    }
    if(rv == 61){
        // iPad2,1_12B440
        MISValidateSignature = 0x2f422090;
        MOV_R0_0__BX_LR = 0x2f42095e;
        exportTableOffset = 0x12b25ec1;
        return;
    }
    if(rv == 62){
        // iPad2,1_12B466
        MISValidateSignature = 0x2f422090;
        MOV_R0_0__BX_LR = 0x2f42095e;
        exportTableOffset = 0x12b25efd;
        return;
    }
    if(rv == 63){
        // iPad2,1_12D508
        MISValidateSignature = 0x2f899ccc;
        MOV_R0_0__BX_LR = 0x2f89856a;
        exportTableOffset = 0x132ce4ad;
        return;
    }
    if(rv == 64){
        // iPad2,1_12F69
        MISValidateSignature = 0x2fc36ca0;
        MOV_R0_0__BX_LR = 0x2fc35506;
        exportTableOffset = 0x1350d92b;
        return;
    }
    if(rv == 65){
        // iPad2,1_12H143
        MISValidateSignature = 0x30045ca0;
        MOV_R0_0__BX_LR = 0x30044506;
        exportTableOffset = 0x13a2e408;
        return;
    }
    if(rv == 66){
        // iPad2,1_12H321
        MISValidateSignature = 0x30050ca0;
        MOV_R0_0__BX_LR = 0x3004f506;
        exportTableOffset = 0x13a3ab59;
        return;
    }
    if(rv == 67){
        // iPad2,2_12A365
        MISValidateSignature = 0x2f441090;
        MOV_R0_0__BX_LR = 0x2f43f95e;
        exportTableOffset = 0x12b3ebdd;
        return;
    }
    if(rv == 68){
        // iPad2,2_12A402
        MISValidateSignature = 0x2f43f090;
        MOV_R0_0__BX_LR = 0x2f43d95e;
        exportTableOffset = 0x12b3ebd9;
        return;
    }
    if(rv == 69){
        // iPad2,2_12A405
        MISValidateSignature = 0x2f43f090;
        MOV_R0_0__BX_LR = 0x2f43d95e;
        exportTableOffset = 0x12b3ebd9;
        return;
    }
    if(rv == 70){
        // iPad2,2_12B410
        MISValidateSignature = 0x2f54b090;
        MOV_R0_0__BX_LR = 0x2f54995e;
        exportTableOffset = 0x12c65dff;
        return;
    }
    if(rv == 71){
        // iPad2,2_12B435
        MISValidateSignature = 0x2f55f090;
        MOV_R0_0__BX_LR = 0x2f55d95e;
        exportTableOffset = 0x12c820a5;
        return;
    }
    if(rv == 72){
        // iPad2,2_12B440
        MISValidateSignature = 0x2f560090;
        MOV_R0_0__BX_LR = 0x2f55e95e;
        exportTableOffset = 0x12c820a5;
        return;
    }
    if(rv == 73){
        // iPad2,2_12B466
        MISValidateSignature = 0x2f560090;
        MOV_R0_0__BX_LR = 0x2f55e95e;
        exportTableOffset = 0x12c7e0e1;
        return;
    }
    if(rv == 74){
        // iPad2,2_12D508
        MISValidateSignature = 0x2f9d6ccc;
        MOV_R0_0__BX_LR = 0x2f9d556a;
        exportTableOffset = 0x13427145;
        return;
    }
    if(rv == 75){
        // iPad2,2_12F69
        MISValidateSignature = 0x2fd73ca0;
        MOV_R0_0__BX_LR = 0x2fd72506;
        exportTableOffset = 0x13665f31;
        return;
    }
    if(rv == 76){
        // iPad2,2_12H143
        MISValidateSignature = 0x30183ca0;
        MOV_R0_0__BX_LR = 0x30182506;
        exportTableOffset = 0x13b8662c;
        return;
    }
    if(rv == 77){
        // iPad2,2_12H321
        MISValidateSignature = 0x3018dca0;
        MOV_R0_0__BX_LR = 0x3018c506;
        exportTableOffset = 0x13b8ed7d;
        return;
    }
    if(rv == 78){
        // iPad2,3_12A365
        MISValidateSignature = 0x2f441090;
        MOV_R0_0__BX_LR = 0x2f43f95e;
        exportTableOffset = 0x12b3ebdd;
        return;
    }
    if(rv == 79){
        // iPad2,3_12A402
        MISValidateSignature = 0x2f43f090;
        MOV_R0_0__BX_LR = 0x2f43d95e;
        exportTableOffset = 0x12b3ebd9;
        return;
    }
    if(rv == 80){
        // iPad2,3_12A405
        MISValidateSignature = 0x2f43f090;
        MOV_R0_0__BX_LR = 0x2f43d95e;
        exportTableOffset = 0x12b3ebd9;
        return;
    }
    if(rv == 81){
        // iPad2,3_12B410
        MISValidateSignature = 0x2f54b090;
        MOV_R0_0__BX_LR = 0x2f54995e;
        exportTableOffset = 0x12c65dff;
        return;
    }
    if(rv == 82){
        // iPad2,3_12B435
        MISValidateSignature = 0x2f55f090;
        MOV_R0_0__BX_LR = 0x2f55d95e;
        exportTableOffset = 0x12c820a5;
        return;
    }
    if(rv == 83){
        // iPad2,3_12B440
        MISValidateSignature = 0x2f560090;
        MOV_R0_0__BX_LR = 0x2f55e95e;
        exportTableOffset = 0x12c820a5;
        return;
    }
    if(rv == 84){
        // iPad2,3_12B466
        MISValidateSignature = 0x2f560090;
        MOV_R0_0__BX_LR = 0x2f55e95e;
        exportTableOffset = 0x12c7e0e1;
        return;
    }
    if(rv == 85){
        // iPad2,3_12D508
        MISValidateSignature = 0x2f9d6ccc;
        MOV_R0_0__BX_LR = 0x2f9d556a;
        exportTableOffset = 0x13427145;
        return;
    }
    if(rv == 86){
        // iPad2,3_12F69
        MISValidateSignature = 0x2fd73ca0;
        MOV_R0_0__BX_LR = 0x2fd72506;
        exportTableOffset = 0x13665f31;
        return;
    }
    if(rv == 87){
        // iPad2,3_12H143
        MISValidateSignature = 0x30183ca0;
        MOV_R0_0__BX_LR = 0x30182506;
        exportTableOffset = 0x13b8662c;
        return;
    }
    if(rv == 88){
        // iPad2,3_12H321
        MISValidateSignature = 0x3018dca0;
        MOV_R0_0__BX_LR = 0x3018c506;
        exportTableOffset = 0x13b8ed7d;
        return;
    }
    if(rv == 89){
        // iPad2,4_12A365
        MISValidateSignature = 0x2f304090;
        MOV_R0_0__BX_LR = 0x2f30295e;
        exportTableOffset = 0x129e69f9;
        return;
    }
    if(rv == 90){
        // iPad2,4_12A402
        MISValidateSignature = 0x2f302090;
        MOV_R0_0__BX_LR = 0x2f30095e;
        exportTableOffset = 0x129e29f5;
        return;
    }
    if(rv == 91){
        // iPad2,4_12A405
        MISValidateSignature = 0x2f302090;
        MOV_R0_0__BX_LR = 0x2f30095e;
        exportTableOffset = 0x129e29f5;
        return;
    }
    if(rv == 92){
        // iPad2,4_12B410
        MISValidateSignature = 0x2f40d090;
        MOV_R0_0__BX_LR = 0x2f40b95e;
        exportTableOffset = 0x12b0dc1b;
        return;
    }
    if(rv == 93){
        // iPad2,4_12B435
        MISValidateSignature = 0x2f421090;
        MOV_R0_0__BX_LR = 0x2f41f95e;
        exportTableOffset = 0x12b25ec1;
        return;
    }
    if(rv == 94){
        // iPad2,4_12B440
        MISValidateSignature = 0x2f422090;
        MOV_R0_0__BX_LR = 0x2f42095e;
        exportTableOffset = 0x12b25ec1;
        return;
    }
    if(rv == 95){
        // iPad2,4_12B466
        MISValidateSignature = 0x2f422090;
        MOV_R0_0__BX_LR = 0x2f42095e;
        exportTableOffset = 0x12b25efd;
        return;
    }
    if(rv == 96){
        // iPad2,4_12D508
        MISValidateSignature = 0x2f899ccc;
        MOV_R0_0__BX_LR = 0x2f89856a;
        exportTableOffset = 0x132ce4ad;
        return;
    }
    if(rv == 97){
        // iPad2,4_12F69
        MISValidateSignature = 0x2fc36ca0;
        MOV_R0_0__BX_LR = 0x2fc35506;
        exportTableOffset = 0x1350d92b;
        return;
    }
    if(rv == 98){
        // iPad2,4_12H143
        MISValidateSignature = 0x30045ca0;
        MOV_R0_0__BX_LR = 0x30044506;
        exportTableOffset = 0x13a2e408;
        return;
    }
    if(rv == 99){
        // iPad2,4_12H321
        MISValidateSignature = 0x30050ca0;
        MOV_R0_0__BX_LR = 0x3004f506;
        exportTableOffset = 0x13a3ab59;
        return;
    }
    if(rv == 100){
        // iPad2,5_12A365
        MISValidateSignature = 0x2f3d9090;
        MOV_R0_0__BX_LR = 0x2f3d795e;
        exportTableOffset = 0x12ae5fa9;
        return;
    }
    if(rv == 101){
        // iPad2,5_12A402
        MISValidateSignature = 0x2f3d7090;
        MOV_R0_0__BX_LR = 0x2f3d595e;
        exportTableOffset = 0x12ae5fa5;
        return;
    }
    if(rv == 102){
        // iPad2,5_12A405
        MISValidateSignature = 0x2f3d7090;
        MOV_R0_0__BX_LR = 0x2f3d595e;
        exportTableOffset = 0x12ae5fa5;
        return;
    }
    if(rv == 103){
        // iPad2,5_12B410
        MISValidateSignature = 0x2f4e4090;
        MOV_R0_0__BX_LR = 0x2f4e295e;
        exportTableOffset = 0x12c111cb;
        return;
    }
    if(rv == 104){
        // iPad2,5_12B435
        MISValidateSignature = 0x2f4f9090;
        MOV_R0_0__BX_LR = 0x2f4f795e;
        exportTableOffset = 0x12c29471;
        return;
    }
    if(rv == 105){
        // iPad2,5_12B440
        MISValidateSignature = 0x2f4fa090;
        MOV_R0_0__BX_LR = 0x2f4f895e;
        exportTableOffset = 0x12c29471;
        return;
    }
    if(rv == 106){
        // iPad2,5_12B466
        MISValidateSignature = 0x2f4fa090;
        MOV_R0_0__BX_LR = 0x2f4f895e;
        exportTableOffset = 0x12c294ad;
        return;
    }
    if(rv == 107){
        // iPad2,5_12D508
        MISValidateSignature = 0x2f97accc;
        MOV_R0_0__BX_LR = 0x2f97956a;
        exportTableOffset = 0x133e2aa1;
        return;
    }
    if(rv == 108){
        // iPad2,5_12F69
        MISValidateSignature = 0x2fd1bca0;
        MOV_R0_0__BX_LR = 0x2fd1a506;
        exportTableOffset = 0x13621935;
        return;
    }
    if(rv == 109){
        // iPad2,5_12H143
        MISValidateSignature = 0x3012aca0;
        MOV_R0_0__BX_LR = 0x30129506;
        exportTableOffset = 0x13b46030;
        return;
    }
    if(rv == 110){
        // iPad2,5_12H321
        MISValidateSignature = 0x30134ca0;
        MOV_R0_0__BX_LR = 0x30133506;
        exportTableOffset = 0x13b4e781;
        return;
    }
    if(rv == 111){
        // iPad2,6_12A365
        MISValidateSignature = 0x2f516090;
        MOV_R0_0__BX_LR = 0x2f51495e;
        exportTableOffset = 0x12c3e18d;
        return;
    }
    if(rv == 112){
        // iPad2,6_12A402
        MISValidateSignature = 0x2f514090;
        MOV_R0_0__BX_LR = 0x2f51295e;
        exportTableOffset = 0x12c3e189;
        return;
    }
    if(rv == 113){
        // iPad2,6_12A405
        MISValidateSignature = 0x2f514090;
        MOV_R0_0__BX_LR = 0x2f51295e;
        exportTableOffset = 0x12c3e189;
        return;
    }
    if(rv == 114){
        // iPad2,6_12B410
        MISValidateSignature = 0x2f621090;
        MOV_R0_0__BX_LR = 0x2f61f95e;
        exportTableOffset = 0x12d653af;
        return;
    }
    if(rv == 115){
        // iPad2,6_12B435
        MISValidateSignature = 0x2f636090;
        MOV_R0_0__BX_LR = 0x2f63495e;
        exportTableOffset = 0x12d81655;
        return;
    }
    if(rv == 116){
        // iPad2,6_12B440
        MISValidateSignature = 0x2f637090;
        MOV_R0_0__BX_LR = 0x2f63595e;
        exportTableOffset = 0x12d85655;
        return;
    }
    if(rv == 117){
        // iPad2,6_12B466
        MISValidateSignature = 0x2f637090;
        MOV_R0_0__BX_LR = 0x2f63595e;
        exportTableOffset = 0x12d81691;
        return;
    }
    if(rv == 118){
        // iPad2,6_12D508
        MISValidateSignature = 0x2fab7ccc;
        MOV_R0_0__BX_LR = 0x2fab656a;
        exportTableOffset = 0x13536ce5;
        return;
    }
    if(rv == 119){
        // iPad2,6_12F69
        MISValidateSignature = 0x2fe58ca0;
        MOV_R0_0__BX_LR = 0x2fe57506;
        exportTableOffset = 0x13779c5d;
        return;
    }
    if(rv == 120){
        // iPad2,6_12H143
        MISValidateSignature = 0x30267ca0;
        MOV_R0_0__BX_LR = 0x30266506;
        exportTableOffset = 0x13c9a254;
        return;
    }
    if(rv == 121){
        // iPad2,6_12H321
        MISValidateSignature = 0x30271ca0;
        MOV_R0_0__BX_LR = 0x30270506;
        exportTableOffset = 0x13ca29a5;
        return;
    }
    if(rv == 122){
        // iPad2,7_12A365
        MISValidateSignature = 0x2f516090;
        MOV_R0_0__BX_LR = 0x2f51495e;
        exportTableOffset = 0x12c3e18d;
        return;
    }
    if(rv == 123){
        // iPad2,7_12A402
        MISValidateSignature = 0x2f514090;
        MOV_R0_0__BX_LR = 0x2f51295e;
        exportTableOffset = 0x12c3e189;
        return;
    }
    if(rv == 124){
        // iPad2,7_12A405
        MISValidateSignature = 0x2f514090;
        MOV_R0_0__BX_LR = 0x2f51295e;
        exportTableOffset = 0x12c3e189;
        return;
    }
    if(rv == 125){
        // iPad2,7_12B410
        MISValidateSignature = 0x2f621090;
        MOV_R0_0__BX_LR = 0x2f61f95e;
        exportTableOffset = 0x12d653af;
        return;
    }
    if(rv == 126){
        // iPad2,7_12B435
        MISValidateSignature = 0x2f636090;
        MOV_R0_0__BX_LR = 0x2f63495e;
        exportTableOffset = 0x12d81655;
        return;
    }
    if(rv == 127){
        // iPad2,7_12B440
        MISValidateSignature = 0x2f637090;
        MOV_R0_0__BX_LR = 0x2f63595e;
        exportTableOffset = 0x12d85655;
        return;
    }
    if(rv == 128){
        // iPad2,7_12B466
        MISValidateSignature = 0x2f637090;
        MOV_R0_0__BX_LR = 0x2f63595e;
        exportTableOffset = 0x12d81691;
        return;
    }
    if(rv == 129){
        // iPad2,7_12D508
        MISValidateSignature = 0x2fab7ccc;
        MOV_R0_0__BX_LR = 0x2fab656a;
        exportTableOffset = 0x13536ce5;
        return;
    }
    if(rv == 130){
        // iPad2,7_12F69
        MISValidateSignature = 0x2fe58ca0;
        MOV_R0_0__BX_LR = 0x2fe57506;
        exportTableOffset = 0x13779c5d;
        return;
    }
    if(rv == 131){
        // iPad2,7_12H143
        MISValidateSignature = 0x30267ca0;
        MOV_R0_0__BX_LR = 0x30266506;
        exportTableOffset = 0x13c9a254;
        return;
    }
    if(rv == 132){
        // iPad2,7_12H321
        MISValidateSignature = 0x30271ca0;
        MOV_R0_0__BX_LR = 0x30270506;
        exportTableOffset = 0x13ca29a5;
        return;
    }
    if(rv == 133){
        // iPad3,1_12A365
        MISValidateSignature = 0x2f398090;
        MOV_R0_0__BX_LR = 0x2f39695e;
        exportTableOffset = 0x12a97fb1;
        return;
    }
    if(rv == 134){
        // iPad3,1_12A402
        MISValidateSignature = 0x2f396090;
        MOV_R0_0__BX_LR = 0x2f39495e;
        exportTableOffset = 0x12a93fad;
        return;
    }
    if(rv == 135){
        // iPad3,1_12A405
        MISValidateSignature = 0x2f396090;
        MOV_R0_0__BX_LR = 0x2f39495e;
        exportTableOffset = 0x12a93fad;
        return;
    }
    if(rv == 136){
        // iPad3,1_12B410
        MISValidateSignature = 0x2f4a7090;
        MOV_R0_0__BX_LR = 0x2f4a595e;
        exportTableOffset = 0x12bc726f;
        return;
    }
    if(rv == 137){
        // iPad3,1_12B435
        MISValidateSignature = 0x2f4bc090;
        MOV_R0_0__BX_LR = 0x2f4ba95e;
        exportTableOffset = 0x12bdf515;
        return;
    }
    if(rv == 138){
        // iPad3,1_12B440
        MISValidateSignature = 0x2f4bd090;
        MOV_R0_0__BX_LR = 0x2f4bb95e;
        exportTableOffset = 0x12bdf515;
        return;
    }
    if(rv == 139){
        // iPad3,1_12B466
        MISValidateSignature = 0x2f4bd090;
        MOV_R0_0__BX_LR = 0x2f4bb95e;
        exportTableOffset = 0x12bdf551;
        return;
    }
    if(rv == 140){
        // iPad3,1_12D508
        MISValidateSignature = 0x2f93bccc;
        MOV_R0_0__BX_LR = 0x2f93a56a;
        exportTableOffset = 0x13390a51;
        return;
    }
    if(rv == 141){
        // iPad3,1_12F69
        MISValidateSignature = 0x2fcdaca0;
        MOV_R0_0__BX_LR = 0x2fcd9506;
        exportTableOffset = 0x135d38f1;
        return;
    }
    if(rv == 142){
        // iPad3,1_12H143
        MISValidateSignature = 0x300e9ca0;
        MOV_R0_0__BX_LR = 0x300e8506;
        exportTableOffset = 0x13aeffec;
        return;
    }
    if(rv == 143){
        // iPad3,1_12H321
        MISValidateSignature = 0x300f3ca0;
        MOV_R0_0__BX_LR = 0x300f2506;
        exportTableOffset = 0x13afc73d;
        return;
    }
    if(rv == 144){
        // iPad3,2_12A365
        MISValidateSignature = 0x2f4d5090;
        MOV_R0_0__BX_LR = 0x2f4d395e;
        exportTableOffset = 0x12bec195;
        return;
    }
    if(rv == 145){
        // iPad3,2_12A402
        MISValidateSignature = 0x2f4d3090;
        MOV_R0_0__BX_LR = 0x2f4d195e;
        exportTableOffset = 0x12bec191;
        return;
    }
    if(rv == 146){
        // iPad3,2_12A405
        MISValidateSignature = 0x2f4d3090;
        MOV_R0_0__BX_LR = 0x2f4d195e;
        exportTableOffset = 0x12bec191;
        return;
    }
    if(rv == 147){
        // iPad3,2_12B410
        MISValidateSignature = 0x2f5e4090;
        MOV_R0_0__BX_LR = 0x2f5e295e;
        exportTableOffset = 0x12d1b453;
        return;
    }
    if(rv == 148){
        // iPad3,2_12B435
        MISValidateSignature = 0x2f5f9090;
        MOV_R0_0__BX_LR = 0x2f5f795e;
        exportTableOffset = 0x12d336f9;
        return;
    }
    if(rv == 149){
        // iPad3,2_12B440
        MISValidateSignature = 0x2f5fa090;
        MOV_R0_0__BX_LR = 0x2f5f895e;
        exportTableOffset = 0x12d336f9;
        return;
    }
    if(rv == 150){
        // iPad3,2_12B466
        MISValidateSignature = 0x2f5fa090;
        MOV_R0_0__BX_LR = 0x2f5f895e;
        exportTableOffset = 0x12d33735;
        return;
    }
    if(rv == 151){
        // iPad3,2_12D508
        MISValidateSignature = 0x2fa79ccc;
        MOV_R0_0__BX_LR = 0x2fa7856a;
        exportTableOffset = 0x134e8d09;
        return;
    }
    if(rv == 152){
        // iPad3,2_12F69
        MISValidateSignature = 0x2fe17ca0;
        MOV_R0_0__BX_LR = 0x2fe16506;
        exportTableOffset = 0x13727c11;
        return;
    }
    if(rv == 153){
        // iPad3,2_12H143
        MISValidateSignature = 0x30226ca0;
        MOV_R0_0__BX_LR = 0x30225506;
        exportTableOffset = 0x13c4c210;
        return;
    }
    if(rv == 154){
        // iPad3,2_12H321
        MISValidateSignature = 0x30230ca0;
        MOV_R0_0__BX_LR = 0x3022f506;
        exportTableOffset = 0x13c54961;
        return;
    }
    if(rv == 155){
        // iPad3,3_12A365
        MISValidateSignature = 0x2f4d5090;
        MOV_R0_0__BX_LR = 0x2f4d395e;
        exportTableOffset = 0x12bec195;
        return;
    }
    if(rv == 156){
        // iPad3,3_12A402
        MISValidateSignature = 0x2f4d3090;
        MOV_R0_0__BX_LR = 0x2f4d195e;
        exportTableOffset = 0x12bec191;
        return;
    }
    if(rv == 157){
        // iPad3,3_12A405
        MISValidateSignature = 0x2f4d3090;
        MOV_R0_0__BX_LR = 0x2f4d195e;
        exportTableOffset = 0x12bec191;
        return;
    }
    if(rv == 158){
        // iPad3,3_12B410
        MISValidateSignature = 0x2f5e4090;
        MOV_R0_0__BX_LR = 0x2f5e295e;
        exportTableOffset = 0x12d1b453;
        return;
    }
    if(rv == 159){
        // iPad3,3_12B435
        MISValidateSignature = 0x2f5f9090;
        MOV_R0_0__BX_LR = 0x2f5f795e;
        exportTableOffset = 0x12d336f9;
        return;
    }
    if(rv == 160){
        // iPad3,3_12B440
        MISValidateSignature = 0x2f5fa090;
        MOV_R0_0__BX_LR = 0x2f5f895e;
        exportTableOffset = 0x12d336f9;
        return;
    }
    if(rv == 161){
        // iPad3,3_12B466
        MISValidateSignature = 0x2f5fa090;
        MOV_R0_0__BX_LR = 0x2f5f895e;
        exportTableOffset = 0x12d33735;
        return;
    }
    if(rv == 162){
        // iPad3,3_12D508
        MISValidateSignature = 0x2fa79ccc;
        MOV_R0_0__BX_LR = 0x2fa7856a;
        exportTableOffset = 0x134e8d09;
        return;
    }
    if(rv == 163){
        // iPad3,3_12F69
        MISValidateSignature = 0x2fe17ca0;
        MOV_R0_0__BX_LR = 0x2fe16506;
        exportTableOffset = 0x13727c11;
        return;
    }
    if(rv == 164){
        // iPad3,3_12H143
        MISValidateSignature = 0x30226ca0;
        MOV_R0_0__BX_LR = 0x30225506;
        exportTableOffset = 0x13c4c210;
        return;
    }
    if(rv == 165){
        // iPad3,3_12H321
        MISValidateSignature = 0x30230ca0;
        MOV_R0_0__BX_LR = 0x3022f506;
        exportTableOffset = 0x13c54961;
        return;
    }
    if(rv == 166){
        // iPad3,4_12A365
        MISValidateSignature = 0x2f3220b4;
        MOV_R0_0__BX_LR = 0x2f320982;
        exportTableOffset = 0x1298c369;
        return;
    }
    if(rv == 167){
        // iPad3,4_12A402
        MISValidateSignature = 0x2f3240b4;
        MOV_R0_0__BX_LR = 0x2f322982;
        exportTableOffset = 0x12990369;
        return;
    }
    if(rv == 168){
        // iPad3,4_12A405
        MISValidateSignature = 0x2f3240b4;
        MOV_R0_0__BX_LR = 0x2f322982;
        exportTableOffset = 0x12990369;
        return;
    }
    if(rv == 169){
        // iPad3,4_12B410
        MISValidateSignature = 0x2f4300b4;
        MOV_R0_0__BX_LR = 0x2f42e982;
        exportTableOffset = 0x12ab756b;
        return;
    }
    if(rv == 170){
        // iPad3,4_12B435
        MISValidateSignature = 0x2f43e0b4;
        MOV_R0_0__BX_LR = 0x2f43c982;
        exportTableOffset = 0x12ac7822;
        return;
    }
    if(rv == 171){
        // iPad3,4_12B440
        MISValidateSignature = 0x2f43f0b4;
        MOV_R0_0__BX_LR = 0x2f43d982;
        exportTableOffset = 0x12acb822;
        return;
    }
    if(rv == 172){
        // iPad3,4_12B466
        MISValidateSignature = 0x2f4410b4;
        MOV_R0_0__BX_LR = 0x2f43f982;
        exportTableOffset = 0x12acb862;
        return;
    }
    if(rv == 173){
        // iPad3,4_12D508
        MISValidateSignature = 0x2f8c3cf4;
        MOV_R0_0__BX_LR = 0x2f8c2592;
        exportTableOffset = 0x13288459;
        return;
    }
    if(rv == 174){
        // iPad3,4_12F69
        MISValidateSignature = 0x2fc67cc8;
        MOV_R0_0__BX_LR = 0x2fc6652e;
        exportTableOffset = 0x134cf8fb;
        return;
    }
    if(rv == 175){
        // iPad3,4_12H143
        MISValidateSignature = 0x30086cc8;
        MOV_R0_0__BX_LR = 0x3008552e;
        exportTableOffset = 0x13a003d8;
        return;
    }
    if(rv == 176){
        // iPad3,4_12H321
        MISValidateSignature = 0x30093cc8;
        MOV_R0_0__BX_LR = 0x3009252e;
        exportTableOffset = 0x13a08b7d;
        return;
    }
    if(rv == 177){
        // iPad3,5_12A365
        MISValidateSignature = 0x2f45f0b4;
        MOV_R0_0__BX_LR = 0x2f45d982;
        exportTableOffset = 0x12ae454d;
        return;
    }
    if(rv == 178){
        // iPad3,5_12A402
        MISValidateSignature = 0x2f4610b4;
        MOV_R0_0__BX_LR = 0x2f45f982;
        exportTableOffset = 0x12ae454d;
        return;
    }
    if(rv == 179){
        // iPad3,5_12A405
        MISValidateSignature = 0x2f4610b4;
        MOV_R0_0__BX_LR = 0x2f45f982;
        exportTableOffset = 0x12ae454d;
        return;
    }
    if(rv == 180){
        // iPad3,5_12B410
        MISValidateSignature = 0x2f56d0b4;
        MOV_R0_0__BX_LR = 0x2f56b982;
        exportTableOffset = 0x12c0b74f;
        return;
    }
    if(rv == 181){
        // iPad3,5_12B435
        MISValidateSignature = 0x2f57b0b4;
        MOV_R0_0__BX_LR = 0x2f579982;
        exportTableOffset = 0x12c23a06;
        return;
    }
    if(rv == 182){
        // iPad3,5_12B440
        MISValidateSignature = 0x2f57c0b4;
        MOV_R0_0__BX_LR = 0x2f57a982;
        exportTableOffset = 0x12c23a06;
        return;
    }
    if(rv == 183){
        // iPad3,5_12B466
        MISValidateSignature = 0x2f57e0b4;
        MOV_R0_0__BX_LR = 0x2f57c982;
        exportTableOffset = 0x12c23a46;
        return;
    }
    if(rv == 184){
        // iPad3,5_12D508
        MISValidateSignature = 0x2fa00cf4;
        MOV_R0_0__BX_LR = 0x2f9ff592;
        exportTableOffset = 0x133dd0e9;
        return;
    }
    if(rv == 185){
        // iPad3,5_12F69
        MISValidateSignature = 0x2fda4cc8;
        MOV_R0_0__BX_LR = 0x2fda352e;
        exportTableOffset = 0x13627f59;
        return;
    }
    if(rv == 186){
        // iPad3,5_12H143
        MISValidateSignature = 0x301c3cc8;
        MOV_R0_0__BX_LR = 0x301c252e;
        exportTableOffset = 0x13b545fc;
        return;
    }
    if(rv == 187){
        // iPad3,5_12H321
        MISValidateSignature = 0x301d0cc8;
        MOV_R0_0__BX_LR = 0x301cf52e;
        exportTableOffset = 0x13b5cda1;
        return;
    }
    if(rv == 188){
        // iPad3,6_12A365
        MISValidateSignature = 0x2f45f0b4;
        MOV_R0_0__BX_LR = 0x2f45d982;
        exportTableOffset = 0x12ae454d;
        return;
    }
    if(rv == 189){
        // iPad3,6_12A402
        MISValidateSignature = 0x2f4610b4;
        MOV_R0_0__BX_LR = 0x2f45f982;
        exportTableOffset = 0x12ae454d;
        return;
    }
    if(rv == 190){
        // iPad3,6_12A405
        MISValidateSignature = 0x2f4610b4;
        MOV_R0_0__BX_LR = 0x2f45f982;
        exportTableOffset = 0x12ae454d;
        return;
    }
    if(rv == 191){
        // iPad3,6_12B410
        MISValidateSignature = 0x2f56d0b4;
        MOV_R0_0__BX_LR = 0x2f56b982;
        exportTableOffset = 0x12c0b74f;
        return;
    }
    if(rv == 192){
        // iPad3,6_12B435
        MISValidateSignature = 0x2f57b0b4;
        MOV_R0_0__BX_LR = 0x2f579982;
        exportTableOffset = 0x12c23a06;
        return;
    }
    if(rv == 193){
        // iPad3,6_12B440
        MISValidateSignature = 0x2f57c0b4;
        MOV_R0_0__BX_LR = 0x2f57a982;
        exportTableOffset = 0x12c23a06;
        return;
    }
    if(rv == 194){
        // iPad3,6_12B466
        MISValidateSignature = 0x2f57e0b4;
        MOV_R0_0__BX_LR = 0x2f57c982;
        exportTableOffset = 0x12c23a46;
        return;
    }
    if(rv == 195){
        // iPad3,6_12D508
        MISValidateSignature = 0x2fa00cf4;
        MOV_R0_0__BX_LR = 0x2f9ff592;
        exportTableOffset = 0x133dd0e9;
        return;
    }
    if(rv == 196){
        // iPad3,6_12F69
        MISValidateSignature = 0x2fda4cc8;
        MOV_R0_0__BX_LR = 0x2fda352e;
        exportTableOffset = 0x13627f59;
        return;
    }
    if(rv == 197){
        // iPad3,6_12H143
        MISValidateSignature = 0x301c3cc8;
        MOV_R0_0__BX_LR = 0x301c252e;
        exportTableOffset = 0x13b545fc;
        return;
    }
    if(rv == 198){
        // iPad3,6_12H321
        MISValidateSignature = 0x301d0cc8;
        MOV_R0_0__BX_LR = 0x301cf52e;
        exportTableOffset = 0x13b5cda1;
        return;
    }
    if(rv == 199){
        // iPod5,1_12A365
        MISValidateSignature = 0x2ef8c090;
        MOV_R0_0__BX_LR = 0x2ef8a95e;
        exportTableOffset = 0x12613195;
        return;
    }
    if(rv == 200){
        // iPod5,1_12A402
        MISValidateSignature = 0x2ef8a090;
        MOV_R0_0__BX_LR = 0x2ef8895e;
        exportTableOffset = 0x1260f191;
        return;
    }
    if(rv == 201){
        // iPod5,1_12A405
        MISValidateSignature = 0x2ef8a090;
        MOV_R0_0__BX_LR = 0x2ef8895e;
        exportTableOffset = 0x1260f191;
        return;
    }
    if(rv == 202){
        // iPod5,1_12B411
        MISValidateSignature = 0x2f091090;
        MOV_R0_0__BX_LR = 0x2f08f95e;
        exportTableOffset = 0x127322a7;
        return;
    }
    if(rv == 203){
        // iPod5,1_12B435
        MISValidateSignature = 0x2f0a6090;
        MOV_R0_0__BX_LR = 0x2f0a495e;
        exportTableOffset = 0x1274e54d;
        return;
    }
    if(rv == 204){
        // iPod5,1_12B440
        MISValidateSignature = 0x2f0a7090;
        MOV_R0_0__BX_LR = 0x2f0a595e;
        exportTableOffset = 0x1275254d;
        return;
    }
    if(rv == 205){
        // iPod5,1_12B466
        MISValidateSignature = 0x2f0a7090;
        MOV_R0_0__BX_LR = 0x2f0a595e;
        exportTableOffset = 0x1274e589;
        return;
    }
    if(rv == 206){
        // iPod5,1_12D508
        MISValidateSignature = 0x2f55bccc;
        MOV_R0_0__BX_LR = 0x2f55a56a;
        exportTableOffset = 0x12f44315;
        return;
    }
    if(rv == 207){
        // iPod5,1_12F69
        MISValidateSignature = 0x2f8faca0;
        MOV_R0_0__BX_LR = 0x2f8f9506;
        exportTableOffset = 0x131830b3;
        return;
    }
    if(rv == 208){
        // iPod5,1_12H143
        MISValidateSignature = 0x2fd07ca0;
        MOV_R0_0__BX_LR = 0x2fd06506;
        exportTableOffset = 0x136a426c;
        return;
    }
    if(rv == 209){
        // iPod5,1_12H321
        MISValidateSignature = 0x2fd11ca0;
        MOV_R0_0__BX_LR = 0x2fd10506;
        exportTableOffset = 0x136ac9bd;
        return;
    }
    if(rv == 210){
        // iPhone4,1_13A344
        MISValidateSignature = 0x331aae7c;
        MOV_R0_0__BX_LR = 0x331a93e6;
        exportTableOffset = 0x1736ff01;
        return;
    }
    if(rv == 211){
        // iPhone4,1_13A404
        MISValidateSignature = 0x331aae7c;
        MOV_R0_0__BX_LR = 0x331a93e6;
        exportTableOffset = 0x1736ff01;
        return;
    }
    if(rv == 212){
        // iPhone4,1_13A452
        MISValidateSignature = 0x331abe7c;
        MOV_R0_0__BX_LR = 0x331aa3e6;
        exportTableOffset = 0x1736ff5d;
        return;
    }
    if(rv == 213){
        // iPhone4,1_13B143
        MISValidateSignature = 0x33175d90;
        MOV_R0_0__BX_LR = 0x331742c2;
        exportTableOffset = 0x1737f7be;
        return;
    }
    if(rv == 214){
        // iPhone5,1_13A344
        MISValidateSignature = 0x33533ec8;
        MOV_R0_0__BX_LR = 0x3353242e;
        exportTableOffset = 0x17720751;
        return;
    }
    if(rv == 215){
        // iPhone5,1_13A404
        MISValidateSignature = 0x33533ec8;
        MOV_R0_0__BX_LR = 0x3353242e;
        exportTableOffset = 0x17720751;
        return;
    }
    if(rv == 216){
        // iPhone5,1_13A452
        MISValidateSignature = 0x33533ec8;
        MOV_R0_0__BX_LR = 0x3353242e;
        exportTableOffset = 0x177207ad;
        return;
    }
    if(rv == 217){
        // iPhone5,1_13B143
        MISValidateSignature = 0x334feddc;
        MOV_R0_0__BX_LR = 0x334fd30a;
        exportTableOffset = 0x1772ae86;
        return;
    }
    if(rv == 218){
        // iPhone5,2_13A344
        MISValidateSignature = 0x33533ec8;
        MOV_R0_0__BX_LR = 0x3353242e;
        exportTableOffset = 0x17720751;
        return;
    }
    if(rv == 219){
        // iPhone5,2_13A404
        MISValidateSignature = 0x33533ec8;
        MOV_R0_0__BX_LR = 0x3353242e;
        exportTableOffset = 0x17720751;
        return;
    }
    if(rv == 220){
        // iPhone5,2_13A452
        MISValidateSignature = 0x33533ec8;
        MOV_R0_0__BX_LR = 0x3353242e;
        exportTableOffset = 0x177207ad;
        return;
    }
    if(rv == 221){
        // iPhone5,2_13B143
        MISValidateSignature = 0x334feddc;
        MOV_R0_0__BX_LR = 0x334fd30a;
        exportTableOffset = 0x1772ae86;
        return;
    }
    if(rv == 222){
        // iPhone5,3_13A344
        MISValidateSignature = 0x337e6ec8;
        MOV_R0_0__BX_LR = 0x337e542e;
        exportTableOffset = 0x179e9159;
        return;
    }
    if(rv == 223){
        // iPhone5,3_13A404
        MISValidateSignature = 0x337e6ec8;
        MOV_R0_0__BX_LR = 0x337e542e;
        exportTableOffset = 0x179e9159;
        return;
    }
    if(rv == 224){
        // iPhone5,3_13A452
        MISValidateSignature = 0x337e6ec8;
        MOV_R0_0__BX_LR = 0x337e542e;
        exportTableOffset = 0x179e91b5;
        return;
    }
    if(rv == 225){
        // iPhone5,3_13B143
        MISValidateSignature = 0x337b1ddc;
        MOV_R0_0__BX_LR = 0x337b030a;
        exportTableOffset = 0x179f8ad6;
        return;
    }
    if(rv == 226){
        // iPhone5,4_13A344
        MISValidateSignature = 0x337e6ec8;
        MOV_R0_0__BX_LR = 0x337e542e;
        exportTableOffset = 0x179e9159;
        return;
    }
    if(rv == 227){
        // iPhone5,4_13A404
        MISValidateSignature = 0x337e6ec8;
        MOV_R0_0__BX_LR = 0x337e542e;
        exportTableOffset = 0x179e9159;
        return;
    }
    if(rv == 228){
        // iPhone5,4_13A452
        MISValidateSignature = 0x337e6ec8;
        MOV_R0_0__BX_LR = 0x337e542e;
        exportTableOffset = 0x179e91b5;
        return;
    }
    if(rv == 229){
        // iPhone5,4_13B143
        MISValidateSignature = 0x337b1ddc;
        MOV_R0_0__BX_LR = 0x337b030a;
        exportTableOffset = 0x179f8ad6;
        return;
    }
    if(rv == 230){
        // iPad2,1_13A344
        MISValidateSignature = 0x335d5e7c;
        MOV_R0_0__BX_LR = 0x335d43e6;
        exportTableOffset = 0x177f9361;
        return;
    }
    if(rv == 231){
        // iPad2,1_13A404
        MISValidateSignature = 0x335d5e7c;
        MOV_R0_0__BX_LR = 0x335d43e6;
        exportTableOffset = 0x177f9361;
        return;
    }
    if(rv == 232){
        // iPad2,1_13A452
        MISValidateSignature = 0x335d6e7c;
        MOV_R0_0__BX_LR = 0x335d53e6;
        exportTableOffset = 0x177f93bd;
        return;
    }
    if(rv == 233){
        // iPad2,1_13B143
        MISValidateSignature = 0x33812d90;
        MOV_R0_0__BX_LR = 0x338112c2;
        exportTableOffset = 0x17ac2f5a;
        return;
    }
    if(rv == 234){
        // iPad2,2_13A344
        MISValidateSignature = 0x33768e7c;
        MOV_R0_0__BX_LR = 0x337673e6;
        exportTableOffset = 0x179aa38d;
        return;
    }
    if(rv == 235){
        // iPad2,2_13A404
        MISValidateSignature = 0x33768e7c;
        MOV_R0_0__BX_LR = 0x337673e6;
        exportTableOffset = 0x179aa38d;
        return;
    }
    if(rv == 236){
        // iPad2,2_13A452
        MISValidateSignature = 0x33769e7c;
        MOV_R0_0__BX_LR = 0x337683e6;
        exportTableOffset = 0x179ae3e9;
        return;
    }
    if(rv == 237){
        // iPad2,2_13B143
        MISValidateSignature = 0x339a5d90;
        MOV_R0_0__BX_LR = 0x339a42c2;
        exportTableOffset = 0x17c74516;
        return;
    }
    if(rv == 238){
        // iPad2,3_13A344
        MISValidateSignature = 0x33768e7c;
        MOV_R0_0__BX_LR = 0x337673e6;
        exportTableOffset = 0x179aa38d;
        return;
    }
    if(rv == 239){
        // iPad2,3_13A404
        MISValidateSignature = 0x33768e7c;
        MOV_R0_0__BX_LR = 0x337673e6;
        exportTableOffset = 0x179aa38d;
        return;
    }
    if(rv == 240){
        // iPad2,3_13A452
        MISValidateSignature = 0x33769e7c;
        MOV_R0_0__BX_LR = 0x337683e6;
        exportTableOffset = 0x179ae3e9;
        return;
    }
    if(rv == 241){
        // iPad2,3_13B143
        MISValidateSignature = 0x339a5d90;
        MOV_R0_0__BX_LR = 0x339a42c2;
        exportTableOffset = 0x17c74516;
        return;
    }
    if(rv == 242){
        // iPad2,4_13A344
        MISValidateSignature = 0x335d5e7c;
        MOV_R0_0__BX_LR = 0x335d43e6;
        exportTableOffset = 0x177f9361;
        return;
    }
    if(rv == 243){
        // iPad2,4_13A404
        MISValidateSignature = 0x335d5e7c;
        MOV_R0_0__BX_LR = 0x335d43e6;
        exportTableOffset = 0x177f9361;
        return;
    }
    if(rv == 244){
        // iPad2,4_13A452
        MISValidateSignature = 0x335d6e7c;
        MOV_R0_0__BX_LR = 0x335d53e6;
        exportTableOffset = 0x177f93bd;
        return;
    }
    if(rv == 245){
        // iPad2,4_13B143
        MISValidateSignature = 0x33812d90;
        MOV_R0_0__BX_LR = 0x338112c2;
        exportTableOffset = 0x17ac2f5a;
        return;
    }
    if(rv == 246){
        // iPad2,5_13A344
        MISValidateSignature = 0x33654e7c;
        MOV_R0_0__BX_LR = 0x336533e6;
        exportTableOffset = 0x17890871;
        return;
    }
    if(rv == 247){
        // iPad2,5_13A404
        MISValidateSignature = 0x33654e7c;
        MOV_R0_0__BX_LR = 0x336533e6;
        exportTableOffset = 0x17890871;
        return;
    }
    if(rv == 248){
        // iPad2,5_13A452
        MISValidateSignature = 0x33655e7c;
        MOV_R0_0__BX_LR = 0x336543e6;
        exportTableOffset = 0x178948cd;
        return;
    }
    if(rv == 249){
        // iPad2,5_13B143
        MISValidateSignature = 0x33892d90;
        MOV_R0_0__BX_LR = 0x338912c2;
        exportTableOffset = 0x17b5a6c2;
        return;
    }
    if(rv == 250){
        // iPad2,6_13A344
        MISValidateSignature = 0x337e7e7c;
        MOV_R0_0__BX_LR = 0x337e63e6;
        exportTableOffset = 0x17a4089d;
        return;
    }
    if(rv == 251){
        // iPad2,6_13A404
        MISValidateSignature = 0x337e7e7c;
        MOV_R0_0__BX_LR = 0x337e63e6;
        exportTableOffset = 0x17a4089d;
        return;
    }
    if(rv == 252){
        // iPad2,6_13A452
        MISValidateSignature = 0x337e8e7c;
        MOV_R0_0__BX_LR = 0x337e73e6;
        exportTableOffset = 0x17a408f9;
        return;
    }
    if(rv == 253){
        // iPad2,6_13B143
        MISValidateSignature = 0x33a25d90;
        MOV_R0_0__BX_LR = 0x33a242c2;
        exportTableOffset = 0x17d0aa26;
        return;
    }
    if(rv == 254){
        // iPad2,7_13A344
        MISValidateSignature = 0x337e7e7c;
        MOV_R0_0__BX_LR = 0x337e63e6;
        exportTableOffset = 0x17a4089d;
        return;
    }
    if(rv == 255){
        // iPad2,7_13A404
        MISValidateSignature = 0x337e7e7c;
        MOV_R0_0__BX_LR = 0x337e63e6;
        exportTableOffset = 0x17a4089d;
        return;
    }
    if(rv == 256){
        // iPad2,7_13A452
        MISValidateSignature = 0x337e8e7c;
        MOV_R0_0__BX_LR = 0x337e73e6;
        exportTableOffset = 0x17a408f9;
        return;
    }
    if(rv == 257){
        // iPad2,7_13B143
        MISValidateSignature = 0x33a25d90;
        MOV_R0_0__BX_LR = 0x33a242c2;
        exportTableOffset = 0x17d0aa26;
        return;
    }
    if(rv == 258){
        // iPad3,1_13A344
        MISValidateSignature = 0x33610e7c;
        MOV_R0_0__BX_LR = 0x3360f3e6;
        exportTableOffset = 0x1783958d;
        return;
    }
    if(rv == 259){
        // iPad3,1_13A404
        MISValidateSignature = 0x33610e7c;
        MOV_R0_0__BX_LR = 0x3360f3e6;
        exportTableOffset = 0x1783958d;
        return;
    }
    if(rv == 260){
        // iPad3,1_13A452
        MISValidateSignature = 0x33611e7c;
        MOV_R0_0__BX_LR = 0x336103e6;
        exportTableOffset = 0x1783d5e9;
        return;
    }
    if(rv == 261){
        // iPad3,1_13B143
        MISValidateSignature = 0x3384dd90;
        MOV_R0_0__BX_LR = 0x3384c2c2;
        exportTableOffset = 0x17b07186;
        return;
    }
    if(rv == 262){
        // iPad3,2_13A344
        MISValidateSignature = 0x337a3e7c;
        MOV_R0_0__BX_LR = 0x337a23e6;
        exportTableOffset = 0x179ee5b9;
        return;
    }
    if(rv == 263){
        // iPad3,2_13A404
        MISValidateSignature = 0x337a3e7c;
        MOV_R0_0__BX_LR = 0x337a23e6;
        exportTableOffset = 0x179ee5b9;
        return;
    }
    if(rv == 264){
        // iPad3,2_13A452
        MISValidateSignature = 0x337a4e7c;
        MOV_R0_0__BX_LR = 0x337a33e6;
        exportTableOffset = 0x179ee615;
        return;
    }
    if(rv == 265){
        // iPad3,2_13B143
        MISValidateSignature = 0x339e0d90;
        MOV_R0_0__BX_LR = 0x339df2c2;
        exportTableOffset = 0x17cb4742;
        return;
    }
    if(rv == 266){
        // iPad3,3_13A344
        MISValidateSignature = 0x337a3e7c;
        MOV_R0_0__BX_LR = 0x337a23e6;
        exportTableOffset = 0x179ee5b9;
        return;
    }
    if(rv == 267){
        // iPad3,3_13A404
        MISValidateSignature = 0x337a3e7c;
        MOV_R0_0__BX_LR = 0x337a23e6;
        exportTableOffset = 0x179ee5b9;
        return;
    }
    if(rv == 268){
        // iPad3,3_13A452
        MISValidateSignature = 0x337a4e7c;
        MOV_R0_0__BX_LR = 0x337a33e6;
        exportTableOffset = 0x179ee615;
        return;
    }
    if(rv == 269){
        // iPad3,3_13B143
        MISValidateSignature = 0x339e0d90;
        MOV_R0_0__BX_LR = 0x339df2c2;
        exportTableOffset = 0x17cb4742;
        return;
    }
    if(rv == 270){
        // iPad3,4_13A344
        MISValidateSignature = 0x33642ec8;
        MOV_R0_0__BX_LR = 0x3364142e;
        exportTableOffset = 0x177d4051;
        return;
    }
    if(rv == 271){
        // iPad3,4_13A404
        MISValidateSignature = 0x33642ec8;
        MOV_R0_0__BX_LR = 0x3364142e;
        exportTableOffset = 0x177d4051;
        return;
    }
    if(rv == 272){
        // iPad3,4_13A452
        MISValidateSignature = 0x33642ec8;
        MOV_R0_0__BX_LR = 0x3364142e;
        exportTableOffset = 0x177d40ad;
        return;
    }
    if(rv == 273){
        // iPad3,4_13B143
        MISValidateSignature = 0x3387addc;
        MOV_R0_0__BX_LR = 0x3387930a;
        exportTableOffset = 0x17a95c5e;
        return;
    }
    if(rv == 274){
        // iPad3,5_13A344
        MISValidateSignature = 0x337d5ec8;
        MOV_R0_0__BX_LR = 0x337d442e;
        exportTableOffset = 0x17989079;
        return;
    }
    if(rv == 275){
        // iPad3,5_13A404
        MISValidateSignature = 0x337d5ec8;
        MOV_R0_0__BX_LR = 0x337d442e;
        exportTableOffset = 0x17989079;
        return;
    }
    if(rv == 276){
        // iPad3,5_13A452
        MISValidateSignature = 0x337d5ec8;
        MOV_R0_0__BX_LR = 0x337d442e;
        exportTableOffset = 0x179890d5;
        return;
    }
    if(rv == 277){
        // iPad3,5_13B143
        MISValidateSignature = 0x33a0dddc;
        MOV_R0_0__BX_LR = 0x33a0c30a;
        exportTableOffset = 0x17c4b21a;
        return;
    }
    if(rv == 278){
        // iPad3,6_13A344
        MISValidateSignature = 0x337d5ec8;
        MOV_R0_0__BX_LR = 0x337d442e;
        exportTableOffset = 0x17989079;
        return;
    }
    if(rv == 279){
        // iPad3,6_13A404
        MISValidateSignature = 0x337d5ec8;
        MOV_R0_0__BX_LR = 0x337d442e;
        exportTableOffset = 0x17989079;
        return;
    }
    if(rv == 280){
        // iPad3,6_13A452
        MISValidateSignature = 0x337d5ec8;
        MOV_R0_0__BX_LR = 0x337d442e;
        exportTableOffset = 0x179890d5;
        return;
    }
    if(rv == 281){
        // iPad3,6_13B143
        MISValidateSignature = 0x33a0dddc;
        MOV_R0_0__BX_LR = 0x33a0c30a;
        exportTableOffset = 0x17c4b21a;
        return;
    }
    if(rv == 282){
        // iPod5,1_13A344
        MISValidateSignature = 0x332efe7c;
        MOV_R0_0__BX_LR = 0x332ee3e6;
        exportTableOffset = 0x174b59b1;
        return;
    }
    if(rv == 283){
        // iPod5,1_13A404
        MISValidateSignature = 0x332efe7c;
        MOV_R0_0__BX_LR = 0x332ee3e6;
        exportTableOffset = 0x174b59b1;
        return;
    }
    if(rv == 284){
        // iPod5,1_13A452
        MISValidateSignature = 0x332f0e7c;
        MOV_R0_0__BX_LR = 0x332ef3e6;
        exportTableOffset = 0x174b5a0d;
        return;
    }
    if(rv == 285){
        // iPod5,1_13B143
        MISValidateSignature = 0x332bcd90;
        MOV_R0_0__BX_LR = 0x332bb2c2;
        exportTableOffset = 0x174c5106;
        return;
    }
}

int dyld_hack(char *infile, int rv){
    
    void *buf;
    size_t bufSize = 0x100000;
    size_t sz;
    
    
    // open file
    FILE *fd = fopen(infile, "r");
    if (!fd) {
        printf("error opening %s\n", infile);
        return -1;
    }
    
    fseek(fd, 0, SEEK_END);
    sz = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    
    buf = malloc(bufSize);
    if (!buf) {
        printf("error allocating file buffer\n");
        fclose(fd);
        return -1;
    }
    
    fread(buf, bufSize, 1, fd);
    fclose(fd);
    // end
    
    offset_init(rv);
    
    struct dyld_cache_header *header = buf;
    
    printf("magic               : %s\n", header->magic);
    printf("mappingOffset       : %08x\n", header->mappingOffset);
    printf("mappingCount        : %u\n", header->mappingCount);
    printf("imagesOffset        : %08x\n", header->imagesOffset);
    printf("imagesCount         : %u\n", header->imagesCount);
    printf("dyldBaseAddress     : %016llx\n", header->dyldBaseAddress);
    printf("codeSignatureOffset : %016llx\n", header->codeSignatureOffset);
    printf("codeSignatureSize   : %016llx\n", header->codeSignatureSize);
    //printf("\n");
    
    
    // cs
    void *csBuf;
    size_t csBufSize = 0x8;
    // open file
    fd = fopen(infile, "r");
    if (!fd) {
        printf("error opening %s\n", infile);
        return -1;
    }
    
    fseek(fd, header->codeSignatureOffset, SEEK_SET);
    
    csBuf = malloc(csBufSize);
    if (!csBuf) {
        printf("error allocating file buffer\n");
        fclose(fd);
        return -1;
    }
    
    fread(csBuf, csBufSize, 1, fd);
    fclose(fd);
    // end
    
    
    struct dyld_cache_mapping_info *mapInfo = buf + header->mappingOffset;
    for (int i=0; i < header->mappingCount; i++) {
        printf("dyld_cache_mapping_info [%i]\n", i);
        printf("address    : %016llx\n",  mapInfo->address);
        printf("size       : %016llx\n",  mapInfo->size);
        printf("fileOffset : %016llx\n",  mapInfo->fileOffset);
        printf("maxProt    : %08x\n",  mapInfo->maxProt);
        printf("initProt   : %08x\n",  mapInfo->initProt);
        mapInfo++;
        //printf("\n");
    }
    mapInfo = buf + header->mappingOffset;
    
    
    // search str: "/System/Library/Caches/com.apple.xpc/sdk.dylib"
    const char* searchStr8 = "/System/Library/Caches/com.apple.xpc/sdk.dylib";
    const char* searchStr9 = "/System/Library/Frameworks/CoreGraphics.framework/Resources/libCGCorePDF.dylib";
    
    uint64_t pathOffset;
    if(isIOS9){
        pathOffset = (uint64_t)memmem(buf, bufSize, searchStr9, strlen(searchStr9));
    } else {
        pathOffset = (uint64_t)memmem(buf, bufSize, searchStr8, strlen(searchStr8));
    }
    pathOffset -= (uint64_t)buf;
    
    int pathCount;
    struct dyld_cache_image_info *imageInfo = buf + header->imagesOffset;
    for (int i=0; i < header->imagesCount; i++) {
        //printf("dyld_cache_image_info [%i]\n", i);
        //printf("address        : %016llx\n", imageInfo->address);
        //printf("pathFileOffset : %08x\n", imageInfo->pathFileOffset);
        if(imageInfo->pathFileOffset == pathOffset) pathCount = i;
        //printf("path           : %s\n", (char *)buf+imageInfo->pathFileOffset);
        imageInfo++;
        //printf("pad            : %08x\n", imageInfo->pad);
        ////printf("\n");
    }
    
    if(isIOS9){
        printf("path name  : %s\n", searchStr9);
    } else {
        printf("path name  : %s\n", searchStr8);
    }
    printf("pathOffset : %016llx\n", pathOffset);
    printf("pathCount  : %d\n", pathCount);
    
    imageInfo = buf + header->imagesOffset;
    //printf("\n");
    
    
    uint64_t pad = 0x2000;
    uint64_t dataSize = 0x4000;
    
    uint64_t baseAddr = mapInfo->address;
    uint64_t imageInfo_baseAddr = imageInfo->address;
    uint64_t headerSize = imageInfo_baseAddr - baseAddr;
    size_t newSize = (sz&~0xfff) + pad + headerSize + dataSize;
    
    printf("baseAddr       : %016llx\n", mapInfo->address);
    printf("imageInfo_base : %016llx\n", imageInfo_baseAddr);
    printf("headerSize     : %016llx\n", headerSize);
    printf("size           : %zx -> %zx\n", sz, newSize);
    //printf("\n");
    
    
    // dump header
    void *hdBuf = malloc(headerSize);
    bzero(hdBuf, headerSize);
    memcpy(hdBuf, buf, headerSize);
    
    // create newBuf
    void *newHdBuf = malloc(headerSize);
    bzero(newHdBuf, headerSize);
    
    /* copy fakeheader */
    uint64_t newHeaderOffset = ((sz&~0xfff)+pad);
    printf("[memcpy] header [sz: %016llx] : %016llx -> %016llx\n", headerSize, (uint64_t)0, newHeaderOffset);
    memcpy(newHdBuf, hdBuf, headerSize);
    
    // dump data
    uint64_t dataOffset = (exportTableOffset&~0xfff);
    uint64_t newDataOffset = ((sz&~0xfff)+pad+headerSize);
    
    void *dtBuf = malloc(dataSize);
    bzero(dtBuf, dataSize);
    
    fd = fopen(infile, "r");
    if (!fd) {
        printf("error opening %s\n", infile);
        return -1;
    }
    
    fseek(fd, dataOffset, SEEK_SET);
    
    dtBuf = malloc(dataSize);
    if (!dtBuf) {
        printf("error allocating file buffer\n");
        fclose(fd);
        return -1;
    }
    
    fread(dtBuf, dataSize, 1, fd);
    fclose(fd);
    // end
    
    // new
    void *newDtBuf = malloc(dataSize);
    bzero(newDtBuf, dataSize);
    /* copy fakedata */
    printf("[memcpy] data   [sz: %016llx] : %016llx -> %016llx\n", dataSize, dataOffset, newDataOffset);
    memcpy(newDtBuf, dtBuf, dataSize);
    //printf("\n");
    
    /* buffer
     * buf:      file header [sz:bufSize]
     * hdBuf:    header      [sz:headerSize]
     * newHdBuf: fake header [sz:headerSize]
     * dtBuf:    data        [sz:dataSize]
     * newDtBuf: fake data   [sz:dataSize]
     * csBuf:    cs data     [sz:csBufSize]
     */
    
    header = hdBuf;
    mapInfo = hdBuf + header->mappingOffset;
    imageInfo = hdBuf + header->imagesOffset;
    
    /* header haxx */
    
    // 1, mappingCount == 6
    uint32_t newCount = 6;
    printf("[RemapHeader1] newCount: %08x -> %08x\n", header->mappingCount, newCount);
    *(uint32_t*)(newHdBuf+offsetof(struct dyld_cache_header, mappingCount)) = newCount;
    //printf("\n");
    
    // 2, imagesOffset = imagesOffset + 3*sizeof(struct dyld_cache_mapping_info)
    uint32_t newImgOffset  = header->imagesOffset + 3*sizeof(struct dyld_cache_mapping_info);
    printf("[RemapHeader2] newImgOffset: %08x -> %08x\n", header->imagesOffset, newImgOffset);
    *(uint32_t*)(newHdBuf+offsetof(struct dyld_cache_header, imagesOffset)) = newImgOffset;
    //printf("\n");
    
    // 3, remap header
    
    // flags
#define F_R (1)
#define F_W (2)
#define F_X (4)
    
    uint64_t nextBase;
    uint64_t nextSize;
    uint64_t nextOffset;
    uint64_t tableBaseSize;
    
    // dyld_cache_mapping_info[i]
    for(int i=0;i<newCount;i++){
        printf("[RemapHeader3] dyld_cache_mapping_info [%i]\n", i);
        
        
        if(i==0){
            nextBase = mapInfo->address + headerSize;
            nextSize = mapInfo->size - headerSize;
            nextOffset = headerSize;
            
            printf("address    : %016llx\n", mapInfo->address);
            
            printf("size       : %016llx -> %016llx\n",  mapInfo->size, headerSize);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, size))
                         ) = headerSize;
            
            printf("fileOffset : %016llx -> %016llx\n",  mapInfo->fileOffset, newHeaderOffset);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, fileOffset))
                         ) = newHeaderOffset;
            
            printf("maxProt    : %08x -> %08x\n",  mapInfo->maxProt,  (F_R));
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, maxProt))
                         ) = (F_R);
            
            printf("initProt   : %08x -> %08x\n",  mapInfo->initProt, (F_R));
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, initProt))
                         ) = (F_R);
        }
        
        if(i==1){
            printf("address    : %016llx -> %016llx\n",  mapInfo->address, nextBase);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, address))
                         ) = nextBase;
            
            printf("size       : %016llx -> %016llx\n",  mapInfo->size, nextSize);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, size))
                         ) = nextSize;
            
            printf("fileOffset : %016llx -> %016llx\n",  mapInfo->fileOffset, nextOffset);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, fileOffset))
                         ) = nextOffset;
            
            printf("maxProt    : %08x -> %08x\n",  mapInfo->maxProt, (mapInfo-1)->maxProt);
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, maxProt))
                         ) = (mapInfo-1)->maxProt;
            
            printf("initProt   : %08x -> %08x\n",  mapInfo->initProt, (mapInfo-1)->maxProt);
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, initProt))
                         ) = (mapInfo-1)->maxProt;
            
        }
        
        if(i==2){
            printf("address    : %016llx -> %016llx\n",  mapInfo->address, (mapInfo-1)->address);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, address))
                         ) = (mapInfo-1)->address;
            
            printf("size       : %016llx -> %016llx\n",  mapInfo->size, (mapInfo-1)->size);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, size))
                         ) = (mapInfo-1)->size;
            
            printf("fileOffset : %016llx -> %016llx\n",  mapInfo->fileOffset, (mapInfo-1)->fileOffset);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, fileOffset))
                         ) = (mapInfo-1)->fileOffset;
            
            printf("maxProt    : %08x -> %08x\n",  mapInfo->maxProt, (mapInfo-1)->maxProt);
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, maxProt))
                         ) = (mapInfo-1)->maxProt;
            
            printf("initProt   : %08x -> %08x\n",  mapInfo->initProt, (mapInfo-1)->maxProt);
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, initProt))
                         ) = (mapInfo-1)->maxProt;
        }
        
        if(i==3){
            nextBase = (mapInfo-1)->address + dataOffset-(mapInfo-1)->fileOffset;
            nextSize = dataOffset-(mapInfo-1)->fileOffset;
            printf("address    : %016llx\n", (mapInfo-1)->address);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, address))
                         ) = (mapInfo-1)->address;
            
            printf("size       : %016llx\n", dataOffset-(mapInfo-1)->fileOffset);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, size))
                         ) = dataOffset-(mapInfo-1)->fileOffset;
            tableBaseSize = dataOffset-(mapInfo-1)->fileOffset;
            
            printf("fileOffset : %016llx\n", (mapInfo-1)->fileOffset);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, fileOffset))
                         ) = (mapInfo-1)->fileOffset;
            
            printf("maxProt    : %08x\n", (mapInfo-1)->maxProt);
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, maxProt))
                         ) = (mapInfo-1)->maxProt;
            
            printf("initProt   : %08x\n", (mapInfo-1)->maxProt);
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, initProt))
                         ) = (mapInfo-1)->maxProt;
        }
        
        if(i==4){
            
            printf("address    : %016llx\n", nextBase);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, address))
                         ) = nextBase;
            
            nextBase = nextBase + dataSize;
            
            printf("size       : %016llx\n", dataSize);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, size))
                         ) = dataSize;
            
            printf("fileOffset : %016llx\n", newDataOffset);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, fileOffset))
                         ) = newDataOffset;
            
            printf("maxProt    : %08x\n", (F_R));
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, maxProt))
                         ) = (F_R);
            
            printf("initProt   : %08x\n", (F_R));
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, initProt))
                         ) = (F_R);
        }
        
        if(i==5){
            printf("address    : %016llx\n", nextBase);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, address))
                         ) = nextBase;
            
            printf("size       : %016llx\n", (mapInfo-3)->size-dataSize-nextSize);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, size))
                         ) = (mapInfo-3)->size-dataSize-nextSize;
            
            printf("fileOffset : %016llx\n", (mapInfo-3)->fileOffset+dataSize+nextSize);
            *(uint64_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, fileOffset))
                         ) = (mapInfo-3)->fileOffset+dataSize+nextSize;
            
            printf("maxProt    : %08x\n", (F_R));
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, maxProt))
                         ) = (F_R);
            
            printf("initProt   : %08x\n", (F_R));
            *(uint32_t*)(newHdBuf
                         + (header->mappingOffset)
                         + (i*sizeof(struct dyld_cache_mapping_info))
                         + (offsetof(struct dyld_cache_mapping_info, initProt))
                         ) = (F_R);
            
        }
        
        mapInfo++;
        //printf("\n");
    }
    mapInfo = hdBuf + header->mappingOffset;
    //printf("\n");
    
    // 4, move dyld_cache_image_info
    printf("[RemapHeader4] moving dyld_cache_image_info[%016llx] %08x -> %08x\n", headerSize-newImgOffset, header->imagesOffset, newImgOffset);
    memcpy(newHdBuf+newImgOffset, hdBuf+header->imagesOffset, headerSize-newImgOffset);
    //printf("\n");
    
    // 5, fix dyld_cache_image_info
    uint32_t addSize = newImgOffset-header->imagesOffset;
    printf("dyld_cache_image_info Point: %016lx\n", header->imagesOffset+(pathCount*sizeof(struct dyld_cache_image_info)));
    for (int i=pathCount; i < header->imagesCount; i++) {
        printf("[RemapHeader5] imageInfo->pathFileOffset [%d]: %08x -> %08x\n",
               i,
               (imageInfo+i)->pathFileOffset,
               (imageInfo+i)->pathFileOffset+addSize);
        
        *(uint32_t*)(newHdBuf
                     + (header->imagesOffset)
                     + (i*sizeof(struct dyld_cache_image_info))
                     + (offsetof(struct dyld_cache_image_info, pathFileOffset))
                     + addSize
                     ) = (imageInfo+i)->pathFileOffset+addSize;
    }
    //printf("\n");
    
    // 6, codesignature
    uint32_t cs_length = __builtin_bswap32(*(uint32_t*)(csBuf+4));
    printf("cs_length: %08x\n", cs_length);
    printf("codeSignatureSize: %016llx -> %016llx\n", header->codeSignatureSize, (uint64_t)cs_length);
    *(uint64_t*)(newHdBuf+offsetof(struct dyld_cache_header, codeSignatureSize)) = cs_length;
    //printf("\n");
    
    // 7, change export table
    uint16_t origTable =  *(uint16_t*)(dtBuf+(exportTableOffset-dataOffset));
    //printf("origTable: %04x\n", origTable);
    
    uint64_t patch_point = (exportTableOffset
                            - ((mapInfo+2)->fileOffset + tableBaseSize)
                            + newDataOffset);
    printf("original_point : %016llx\n", exportTableOffset);
    printf("patch_point    : %016llx\n", patch_point);
    
    uint16_t newTable;
    if(MISValidateSignature > MOV_R0_0__BX_LR){
        uint64_t a = MISValidateSignature - MOV_R0_0__BX_LR;
        printf("a: %016llx\n", a);
        
        int i=0;
        while(a>0x80){
            i++;
            a-=0x80;
        }
        printf("i: %x\n", i);
        
        newTable = origTable - a - i*0x100;
    } else {
        uint64_t a = MOV_R0_0__BX_LR - MISValidateSignature;
        printf("a: %016llx\n", a);
        
        int i=0;
        while(a>0x80){
            i++;
            a-=0x80;
        }
        printf("i: %x\n", i);
        
        newTable = origTable + a + i*0x100;
    }
    printf("%016llx: %04x -> %04x\n", patch_point, __builtin_bswap16(origTable), __builtin_bswap16(newTable));
    
    *(uint16_t*)(newDtBuf+(patch_point-newDataOffset)) = newTable;
    //printf("\n");
    /* end */
    
    
    printf("write: %s\n", infile);
    FILE *out = fopen(infile, "r+");
    if (!out) {
        printf("error opening %s\n", infile);
        return -1;
    }
    
    truncate(infile, newSize);
    
    // header
    fseek(out, 0, SEEK_SET);
    fwrite(newHdBuf, headerSize, 1, out);
    
    fseek(out, newHeaderOffset, SEEK_SET);
    fwrite(hdBuf, headerSize, 1, out);
    
    // data
    fseek(out, newDataOffset, SEEK_SET);
    fwrite(newDtBuf, dataSize, 1, out);
    
    //fwrite(newBuf, newSize, 1, out);
    
    /*
     * hdBuf:    orig header [off: newHeaderOffset, sz: headerSize]
     * newHdBuf: fake header [off: 0,               sz: headerSize]
     * newDtBuf: fake data   [off: newDataOffset,   sz: dataSize]
     */
    
    
    fflush(out);
    
    fclose(fd);
    fclose(out);
    
    free(buf);
    free(hdBuf);
    free(newHdBuf);
    free(dtBuf);
    free(newDtBuf);
    free(csBuf);
    
    return 0;
}

int main(int argc, char **argv){
    
    if(argc != 2){
        printf("%s --[device type]_[build id]\n", argv[0]);
        return 0;
    }
    
    int i;
    int rv=0;
    int isA6=0;
 
    if(!strcmp(argv[1], "--iPhone4,1_12A365")) {
        rv=1;
    } else if(!strcmp(argv[1], "--iPhone4,1_12A402")) {
        rv=2;
    } else if(!strcmp(argv[1], "--iPhone4,1_12A405")) {
        rv=3;
    } else if(!strcmp(argv[1], "--iPhone4,1_12B411")) {
        rv=4;
    } else if(!strcmp(argv[1], "--iPhone4,1_12B435")) {
        rv=5;
    } else if(!strcmp(argv[1], "--iPhone4,1_12B440")) {
        rv=6;
    } else if(!strcmp(argv[1], "--iPhone4,1_12B466")) {
        rv=7;
    } else if(!strcmp(argv[1], "--iPhone4,1_12D508")) {
        rv=8;
    } else if(!strcmp(argv[1], "--iPhone4,1_12F70")) {
        rv=9;
    } else if(!strcmp(argv[1], "--iPhone4,1_12H143")) {
        rv=10;
    } else if(!strcmp(argv[1], "--iPhone4,1_12H321")) {
        rv=11;
    } else if(!strcmp(argv[1], "--iPhone5,1_12A365")) {
        rv=12;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_12A402")) {
        rv=13;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_12A405")) {
        rv=14;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_12B411")) {
        rv=15;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_12B435")) {
        rv=16;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_12B440")) {
        rv=17;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_12B466")) {
        rv=18;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_12D508")) {
        rv=19;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_12F70")) {
        rv=20;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_12H143")) {
        rv=21;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_12H321")) {
        rv=22;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12A365")) {
        rv=23;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12A402")) {
        rv=24;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12A405")) {
        rv=25;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12B411")) {
        rv=26;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12B435")) {
        rv=27;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12B440")) {
        rv=28;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12B466")) {
        rv=29;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12D508")) {
        rv=30;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12F70")) {
        rv=31;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12H143")) {
        rv=32;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_12H321")) {
        rv=33;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12A365")) {
        rv=34;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12A402")) {
        rv=35;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12A405")) {
        rv=36;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12B411")) {
        rv=37;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12B435")) {
        rv=38;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12B440")) {
        rv=39;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12B466")) {
        rv=40;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12D508")) {
        rv=41;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12F70")) {
        rv=42;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12H143")) {
        rv=43;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_12H321")) {
        rv=44;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12A365")) {
        rv=45;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12A402")) {
        rv=46;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12A405")) {
        rv=47;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12B411")) {
        rv=48;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12B435")) {
        rv=49;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12B440")) {
        rv=50;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12B466")) {
        rv=51;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12D508")) {
        rv=52;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12F70")) {
        rv=53;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12H143")) {
        rv=54;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_12H321")) {
        rv=55;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad2,1_12A365")) {
        rv=56;
    } else if(!strcmp(argv[1], "--iPad2,1_12A402")) {
        rv=57;
    } else if(!strcmp(argv[1], "--iPad2,1_12A405")) {
        rv=58;
    } else if(!strcmp(argv[1], "--iPad2,1_12B410")) {
        rv=59;
    } else if(!strcmp(argv[1], "--iPad2,1_12B435")) {
        rv=60;
    } else if(!strcmp(argv[1], "--iPad2,1_12B440")) {
        rv=61;
    } else if(!strcmp(argv[1], "--iPad2,1_12B466")) {
        rv=62;
    } else if(!strcmp(argv[1], "--iPad2,1_12D508")) {
        rv=63;
    } else if(!strcmp(argv[1], "--iPad2,1_12F69")) {
        rv=64;
    } else if(!strcmp(argv[1], "--iPad2,1_12H143")) {
        rv=65;
    } else if(!strcmp(argv[1], "--iPad2,1_12H321")) {
        rv=66;
    } else if(!strcmp(argv[1], "--iPad2,2_12A365")) {
        rv=67;
    } else if(!strcmp(argv[1], "--iPad2,2_12A402")) {
        rv=68;
    } else if(!strcmp(argv[1], "--iPad2,2_12A405")) {
        rv=69;
    } else if(!strcmp(argv[1], "--iPad2,2_12B410")) {
        rv=70;
    } else if(!strcmp(argv[1], "--iPad2,2_12B435")) {
        rv=71;
    } else if(!strcmp(argv[1], "--iPad2,2_12B440")) {
        rv=72;
    } else if(!strcmp(argv[1], "--iPad2,2_12B466")) {
        rv=73;
    } else if(!strcmp(argv[1], "--iPad2,2_12D508")) {
        rv=74;
    } else if(!strcmp(argv[1], "--iPad2,2_12F69")) {
        rv=75;
    } else if(!strcmp(argv[1], "--iPad2,2_12H143")) {
        rv=76;
    } else if(!strcmp(argv[1], "--iPad2,2_12H321")) {
        rv=77;
    } else if(!strcmp(argv[1], "--iPad2,3_12A365")) {
        rv=78;
    } else if(!strcmp(argv[1], "--iPad2,3_12A402")) {
        rv=79;
    } else if(!strcmp(argv[1], "--iPad2,3_12A405")) {
        rv=80;
    } else if(!strcmp(argv[1], "--iPad2,3_12B410")) {
        rv=81;
    } else if(!strcmp(argv[1], "--iPad2,3_12B435")) {
        rv=82;
    } else if(!strcmp(argv[1], "--iPad2,3_12B440")) {
        rv=83;
    } else if(!strcmp(argv[1], "--iPad2,3_12B466")) {
        rv=84;
    } else if(!strcmp(argv[1], "--iPad2,3_12D508")) {
        rv=85;
    } else if(!strcmp(argv[1], "--iPad2,3_12F69")) {
        rv=86;
    } else if(!strcmp(argv[1], "--iPad2,3_12H143")) {
        rv=87;
    } else if(!strcmp(argv[1], "--iPad2,3_12H321")) {
        rv=88;
    } else if(!strcmp(argv[1], "--iPad2,4_12A365")) {
        rv=89;
    } else if(!strcmp(argv[1], "--iPad2,4_12A402")) {
        rv=90;
    } else if(!strcmp(argv[1], "--iPad2,4_12A405")) {
        rv=91;
    } else if(!strcmp(argv[1], "--iPad2,4_12B410")) {
        rv=92;
    } else if(!strcmp(argv[1], "--iPad2,4_12B435")) {
        rv=93;
    } else if(!strcmp(argv[1], "--iPad2,4_12B440")) {
        rv=94;
    } else if(!strcmp(argv[1], "--iPad2,4_12B466")) {
        rv=95;
    } else if(!strcmp(argv[1], "--iPad2,4_12D508")) {
        rv=96;
    } else if(!strcmp(argv[1], "--iPad2,4_12F69")) {
        rv=97;
    } else if(!strcmp(argv[1], "--iPad2,4_12H143")) {
        rv=98;
    } else if(!strcmp(argv[1], "--iPad2,4_12H321")) {
        rv=99;
    } else if(!strcmp(argv[1], "--iPad2,5_12A365")) {
        rv=100;
    } else if(!strcmp(argv[1], "--iPad2,5_12A402")) {
        rv=101;
    } else if(!strcmp(argv[1], "--iPad2,5_12A405")) {
        rv=102;
    } else if(!strcmp(argv[1], "--iPad2,5_12B410")) {
        rv=103;
    } else if(!strcmp(argv[1], "--iPad2,5_12B435")) {
        rv=104;
    } else if(!strcmp(argv[1], "--iPad2,5_12B440")) {
        rv=105;
    } else if(!strcmp(argv[1], "--iPad2,5_12B466")) {
        rv=106;
    } else if(!strcmp(argv[1], "--iPad2,5_12D508")) {
        rv=107;
    } else if(!strcmp(argv[1], "--iPad2,5_12F69")) {
        rv=108;
    } else if(!strcmp(argv[1], "--iPad2,5_12H143")) {
        rv=109;
    } else if(!strcmp(argv[1], "--iPad2,5_12H321")) {
        rv=110;
    } else if(!strcmp(argv[1], "--iPad2,6_12A365")) {
        rv=111;
    } else if(!strcmp(argv[1], "--iPad2,6_12A402")) {
        rv=112;
    } else if(!strcmp(argv[1], "--iPad2,6_12A405")) {
        rv=113;
    } else if(!strcmp(argv[1], "--iPad2,6_12B410")) {
        rv=114;
    } else if(!strcmp(argv[1], "--iPad2,6_12B435")) {
        rv=115;
    } else if(!strcmp(argv[1], "--iPad2,6_12B440")) {
        rv=116;
    } else if(!strcmp(argv[1], "--iPad2,6_12B466")) {
        rv=117;
    } else if(!strcmp(argv[1], "--iPad2,6_12D508")) {
        rv=118;
    } else if(!strcmp(argv[1], "--iPad2,6_12F69")) {
        rv=119;
    } else if(!strcmp(argv[1], "--iPad2,6_12H143")) {
        rv=120;
    } else if(!strcmp(argv[1], "--iPad2,6_12H321")) {
        rv=121;
    } else if(!strcmp(argv[1], "--iPad2,7_12A365")) {
        rv=122;
    } else if(!strcmp(argv[1], "--iPad2,7_12A402")) {
        rv=123;
    } else if(!strcmp(argv[1], "--iPad2,7_12A405")) {
        rv=124;
    } else if(!strcmp(argv[1], "--iPad2,7_12B410")) {
        rv=125;
    } else if(!strcmp(argv[1], "--iPad2,7_12B435")) {
        rv=126;
    } else if(!strcmp(argv[1], "--iPad2,7_12B440")) {
        rv=127;
    } else if(!strcmp(argv[1], "--iPad2,7_12B466")) {
        rv=128;
    } else if(!strcmp(argv[1], "--iPad2,7_12D508")) {
        rv=129;
    } else if(!strcmp(argv[1], "--iPad2,7_12F69")) {
        rv=130;
    } else if(!strcmp(argv[1], "--iPad2,7_12H143")) {
        rv=131;
    } else if(!strcmp(argv[1], "--iPad2,7_12H321")) {
        rv=132;
    } else if(!strcmp(argv[1], "--iPad3,1_12A365")) {
        rv=133;
    } else if(!strcmp(argv[1], "--iPad3,1_12A402")) {
        rv=134;
    } else if(!strcmp(argv[1], "--iPad3,1_12A405")) {
        rv=135;
    } else if(!strcmp(argv[1], "--iPad3,1_12B410")) {
        rv=136;
    } else if(!strcmp(argv[1], "--iPad3,1_12B435")) {
        rv=137;
    } else if(!strcmp(argv[1], "--iPad3,1_12B440")) {
        rv=138;
    } else if(!strcmp(argv[1], "--iPad3,1_12B466")) {
        rv=139;
    } else if(!strcmp(argv[1], "--iPad3,1_12D508")) {
        rv=140;
    } else if(!strcmp(argv[1], "--iPad3,1_12F69")) {
        rv=141;
    } else if(!strcmp(argv[1], "--iPad3,1_12H143")) {
        rv=142;
    } else if(!strcmp(argv[1], "--iPad3,1_12H321")) {
        rv=143;
    } else if(!strcmp(argv[1], "--iPad3,2_12A365")) {
        rv=144;
    } else if(!strcmp(argv[1], "--iPad3,2_12A402")) {
        rv=145;
    } else if(!strcmp(argv[1], "--iPad3,2_12A405")) {
        rv=146;
    } else if(!strcmp(argv[1], "--iPad3,2_12B410")) {
        rv=147;
    } else if(!strcmp(argv[1], "--iPad3,2_12B435")) {
        rv=148;
    } else if(!strcmp(argv[1], "--iPad3,2_12B440")) {
        rv=149;
    } else if(!strcmp(argv[1], "--iPad3,2_12B466")) {
        rv=150;
    } else if(!strcmp(argv[1], "--iPad3,2_12D508")) {
        rv=151;
    } else if(!strcmp(argv[1], "--iPad3,2_12F69")) {
        rv=152;
    } else if(!strcmp(argv[1], "--iPad3,2_12H143")) {
        rv=153;
    } else if(!strcmp(argv[1], "--iPad3,2_12H321")) {
        rv=154;
    } else if(!strcmp(argv[1], "--iPad3,3_12A365")) {
        rv=155;
    } else if(!strcmp(argv[1], "--iPad3,3_12A402")) {
        rv=156;
    } else if(!strcmp(argv[1], "--iPad3,3_12A405")) {
        rv=157;
    } else if(!strcmp(argv[1], "--iPad3,3_12B410")) {
        rv=158;
    } else if(!strcmp(argv[1], "--iPad3,3_12B435")) {
        rv=159;
    } else if(!strcmp(argv[1], "--iPad3,3_12B440")) {
        rv=160;
    } else if(!strcmp(argv[1], "--iPad3,3_12B466")) {
        rv=161;
    } else if(!strcmp(argv[1], "--iPad3,3_12D508")) {
        rv=162;
    } else if(!strcmp(argv[1], "--iPad3,3_12F69")) {
        rv=163;
    } else if(!strcmp(argv[1], "--iPad3,3_12H143")) {
        rv=164;
    } else if(!strcmp(argv[1], "--iPad3,3_12H321")) {
        rv=165;
    } else if(!strcmp(argv[1], "--iPad3,4_12A365")) {
        rv=166;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,4_12A402")) {
        rv=167;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,4_12A405")) {
        rv=168;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,4_12B410")) {
        rv=169;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,4_12B435")) {
        rv=170;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,4_12B440")) {
        rv=171;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,4_12B466")) {
        rv=172;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,4_12D508")) {
        rv=173;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,4_12F69")) {
        rv=174;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,4_12H143")) {
        rv=175;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,4_12H321")) {
        rv=176;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12A365")) {
        rv=177;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12A402")) {
        rv=178;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12A405")) {
        rv=179;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12B410")) {
        rv=180;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12B435")) {
        rv=181;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12B440")) {
        rv=182;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12B466")) {
        rv=183;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12D508")) {
        rv=184;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12F69")) {
        rv=185;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12H143")) {
        rv=186;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,5_12H321")) {
        rv=187;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12A365")) {
        rv=188;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12A402")) {
        rv=189;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12A405")) {
        rv=190;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12B410")) {
        rv=191;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12B435")) {
        rv=192;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12B440")) {
        rv=193;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12B466")) {
        rv=194;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12D508")) {
        rv=195;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12F69")) {
        rv=196;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12H143")) {
        rv=197;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPad3,6_12H321")) {
        rv=198;
        isA6=1;
    } else if(!strcmp(argv[1], "--iPod5,1_12A365")) {
        rv=199;
    } else if(!strcmp(argv[1], "--iPod5,1_12A402")) {
        rv=200;
    } else if(!strcmp(argv[1], "--iPod5,1_12A405")) {
        rv=201;
    } else if(!strcmp(argv[1], "--iPod5,1_12B411")) {
        rv=202;
    } else if(!strcmp(argv[1], "--iPod5,1_12B435")) {
        rv=203;
    } else if(!strcmp(argv[1], "--iPod5,1_12B440")) {
        rv=204;
    } else if(!strcmp(argv[1], "--iPod5,1_12B466")) {
        rv=205;
    } else if(!strcmp(argv[1], "--iPod5,1_12D508")) {
        rv=206;
    } else if(!strcmp(argv[1], "--iPod5,1_12F69")) {
        rv=207;
    } else if(!strcmp(argv[1], "--iPod5,1_12H143")) {
        rv=208;
    } else if(!strcmp(argv[1], "--iPod5,1_12H321")) {
        rv=209;
    } else if(!strcmp(argv[1], "--iPhone4,1_13A344")) {
        rv=210;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone4,1_13A404")) {
        rv=211;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone4,1_13A452")) {
        rv=212;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone4,1_13B143")) {
        rv=213;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_13A344")) {
        rv=214;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_13A404")) {
        rv=215;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_13A452")) {
        rv=216;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,1_13B143")) {
        rv=217;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_13A344")) {
        rv=218;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_13A404")) {
        rv=219;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_13A452")) {
        rv=220;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,2_13B143")) {
        rv=221;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_13A344")) {
        rv=222;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_13A404")) {
        rv=223;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_13A452")) {
        rv=224;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,3_13B143")) {
        rv=225;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_13A344")) {
        rv=226;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_13A404")) {
        rv=227;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_13A452")) {
        rv=228;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPhone5,4_13B143")) {
        rv=229;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,1_13A344")) {
        rv=230;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,1_13A404")) {
        rv=231;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,1_13A452")) {
        rv=232;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,1_13B143")) {
        rv=233;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,2_13A344")) {
        rv=234;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,2_13A404")) {
        rv=235;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,2_13A452")) {
        rv=236;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,2_13B143")) {
        rv=237;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,3_13A344")) {
        rv=238;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,3_13A404")) {
        rv=239;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,3_13A452")) {
        rv=240;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,3_13B143")) {
        rv=241;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,4_13A344")) {
        rv=242;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,4_13A404")) {
        rv=243;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,4_13A452")) {
        rv=244;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,4_13B143")) {
        rv=245;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,5_13A344")) {
        rv=246;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,5_13A404")) {
        rv=247;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,5_13A452")) {
        rv=248;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,5_13B143")) {
        rv=249;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,6_13A344")) {
        rv=250;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,6_13A404")) {
        rv=251;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,6_13A452")) {
        rv=252;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,6_13B143")) {
        rv=253;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,7_13A344")) {
        rv=254;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,7_13A404")) {
        rv=255;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,7_13A452")) {
        rv=256;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad2,7_13B143")) {
        rv=257;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,1_13A344")) {
        rv=258;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,1_13A404")) {
        rv=259;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,1_13A452")) {
        rv=260;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,1_13B143")) {
        rv=261;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,2_13A344")) {
        rv=262;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,2_13A404")) {
        rv=263;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,2_13A452")) {
        rv=264;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,2_13B143")) {
        rv=265;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,3_13A344")) {
        rv=266;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,3_13A404")) {
        rv=267;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,3_13A452")) {
        rv=268;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,3_13B143")) {
        rv=269;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,4_13A344")) {
        rv=270;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,4_13A404")) {
        rv=271;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,4_13A452")) {
        rv=272;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,4_13B143")) {
        rv=273;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,5_13A344")) {
        rv=274;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,5_13A404")) {
        rv=275;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,5_13A452")) {
        rv=276;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,5_13B143")) {
        rv=277;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,6_13A344")) {
        rv=278;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,6_13A404")) {
        rv=279;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,6_13A452")) {
        rv=280;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPad3,6_13B143")) {
        rv=281;
        isA6=1;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPod5,1_13A344")) {
        rv=282;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPod5,1_13A404")) {
        rv=283;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPod5,1_13A452")) {
        rv=284;
        isIOS9=1;
    } else if(!strcmp(argv[1], "--iPod5,1_13B143")) {
        rv=285;
        isIOS9=1;
    } else {
        printf("[-] ERROR: This device is not supported!\n");
        reboot(0);
    }
    
    chmod("/mnt1/private", 0777);
    chmod("/mnt1/private/var", 0777);
    chmod("/mnt2/mobile", 0777);
    chmod("/mnt2/mobile/Library", 0777);
    chmod("/mnt2/mobile/Library/Preferences", 0777);
    
    sleep(1);
    
    if(isA6 == 1){
        dyld_hack("/mnt1/System/Library/Caches/com.apple.dyld/dyld_shared_cache_armv7s", rv);
    } else {
        dyld_hack("/mnt1/System/Library/Caches/com.apple.dyld/dyld_shared_cache_armv7", rv);
    }
    
    sleep(3);
    // syncing disk
    for(i=0;i<10;i++){
        sync();
    }
    
    sleep(5);
    
    reboot(0);
    return 0;
}
