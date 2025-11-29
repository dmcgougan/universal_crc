#!/bin/sh

gcc -std=c99 -Wall -pedantic -Os -o universal_crc ../universal_crc.c

poly="0x956789abcdef789b"
init="0"
xor="0"
rev="-r"
nd=""

for algo in "bit   " "tab16 " "tab16i" "tab   " "tabw  " "tabi  " "tabiw "; do
    for bits in " 3" " 5" " 8" 13 16 23 32 49 64; do

./universal_crc -b $bits -p $poly -i $init -x $xor $rev $nd -a $algo >crc_code.c
gcc -std=gnu99 -Wall -pedantic -O2 -o test_perf test_perf.c
perf=`./test_perf`

echo "$bits    $algo    $perf"

    done
done
