#!/bin/sh -x

gcc -std=c99 -Wall -pedantic -DTEST_LITTLE_ENDIAN -Os -o uc_test ../universal_crc.c

poly="0x956789abcdef789b"

for bits in 3 4 5 8 13 16 23 32 49 64; do
    for init in 0 0x91919177deadbee5; do
        for xor in 0 0x12312366baafeeea 0xffffffffffffffff; do
            for rev in "" "-r"; do
                for nd in "" "-n"; do
                    for algo in "-a bit" "-a tab16" "-a tab16i" "-a tab" "-a tabw" "-a tabi" "-a tabiw"; do
#                    for algo in "-a tab16i"; do

(./uc_test -b $bits -p $poly -i $init -x $xor $rev $nd $algo --test | gcc -std=c99 -Wall -pedantic -pipe -O0 -o test -x c -) && ./test || exit 1

                    done
                done
            done
        done
    done
done

gcc -std=c99 -Wall -pedantic -DTEST_BIG_ENDIAN -Os -o uc_test universal_crc.c

for bits in 5 8 13 16 23 32 47 64; do
    for init in 0 0x91919177deadbee5; do
        for xor in 0 0x12312366baafeeea 0xffffffffffffffff; do
            for rev in "" "-r"; do
                for nd in "" "-n"; do
                    for algo in "-a tabw" "-a tabiw"; do

(./uc_test -b $bits -p $poly -i $init -x $xor $rev $nd $algo --test | gcc -std=c99 -Wall -pedantic -pipe -O0 -o test -x c -) && ./test || exit 1

                    done
                done
            done
        done
    done
done
