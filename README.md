# universal_crc

**universal_crc** is a utility for generating optimized C code for CRC calculation.

## Building

Compile the source with:

```
gcc -Os -o universal_crc universal_crc.c
```

---

## Help Text

This is the help text from the program and serves as documentation:

```
universal_crc version 1.3a
Copyright (C) 2011 Danjel McGougan <danjel.mcgougan@gmail.com>

Usage: universal_crc <parameters>

Parameters:
  -b <bits> | --bits=<bits>
    Number of bits in the CRC register, 1-64 is supported
    Mandatory parameter

  -p <polynomial> | --poly=<polynomial>
    CRC polynomial value;
    Coefficient of x^0 is bit 0 (LSB) of this value
    Coefficient of x^1 is bit 1 of this value, etc.
    Coefficient of x^<bits> is implied 1
    Bit-reversed automatically if -r is used
    Mandatory parameter

  -i <init> | --init=<init>
    Initial value of the CRC register
    Not bit-reversed even if -r is used
    Default 0 if not specified

  -x <xor> | --xor=<xor>
    Value that is XORed to the final CRC register value
    Not bit-reversed even if -r is used
    Default 0 if not specified

  -r | --reverse
    Bit-reverse the CRC register (LSB is shifted out and MSB in)
    This also means that message bits are processed LSB first
    Default is not to reverse

  -n | --non-direct
    Shift in message bits into the CRC register and augment the
    message. This is equivalent to the direct method of not
    augmenting the message and XORing the message bits with the
    bits shifted out of the CRC register, but the initial CRC
    register value needs to be converted (if it is non-zero) for
    compatibility.
    Default is direct mode.

  -a <algorithm> | --algorithm=<algorithm>
    CRC algorithm to use:
      bit    standard bit-at-a-time algorithm (default, smallest cache footprint)
      tab16  table-driven algorithm with small cache footprint (16 table entries)
      tab16i table-driven using two independent lookups (32 table entries)
             good for superscalar cores
      tab    standard table-driven algorithm (256 table entries)
      tabw   standard table-driven algorithm, word-at-a-time
             same as tab but reads 32 bits at a time from memory
      tabi   table-driven algorithm, four independent lookups (1024 entries)
             good for superscalar cores
             inspired by crc32 algorithm in zlib originally by Rodney Brown
      tabiw  table-driven algorithm, four independent lookups, word-at-a-time
             same as tabi but reads 32 bits at a time from memory

  --crc-type=<type>
    Use <type> as the unsigned integer type to hold the CRC value

  --tab-type=<type>
    Use <type> as the unsigned integer type to hold the CRC table entries

  --test
    Generate test code

Examples:
  universal_crc -b 32 -p 0x04c11db7 -i 0xffffffff -x 0xffffffff -r
  (CRC used in i.e. Ethernet, commonly known as CRC-32)

  universal_crc -b 16 -p 0x1021
  (CRC used in XMODEM/CRC)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
```

---

## Performance

These performance numbers show the number of CPU cycles per byte needed to calculate the CRC in a cache-warm scenario, measured on an **AMD Athlon 5050e (64-bit Linux)**.

The **tabiw** algorithm at around **2 cycles/byte** represents state-of-the-art performance for software CRC.

Compiled with **gcc 4.7.2** (Ubuntu 12.10) using `-O2`.

```
bits  algo     cycles/byte
----  ----     -----------
 3    bit       41.00
 5    bit       41.00
 8    bit       41.00
13    bit       41.00
16    bit       41.00
23    bit       41.00
32    bit       41.00
49    bit       27.00
64    bit       27.00
 3    tab16     13.12
 5    tab16     13.00
 8    tab16     13.00
13    tab16     12.12
16    tab16     12.13
23    tab16     13.00
32    tab16     13.00
49    tab16     12.13
64    tab16     12.13
 3    tab16i    8.00
 5    tab16i    8.00
 8    tab16i    8.00
13    tab16i    10.00
16    tab16i    10.00
23    tab16i    10.12
32    tab16i    10.13
49    tab16i    9.12
64    tab16i    9.12
 3    tab       6.12
 5    tab       6.12
 8    tab       6.12
13    tab       8.00
16    tab       8.00
23    tab       7.00
32    tab       7.00
49    tab       7.00
64    tab       7.00
 3    tabw      5.13
 5    tabw      5.13
 8    tabw      5.13
13    tabw      6.25
16    tabw      6.25
23    tabw      5.06
32    tabw      5.06
49    tabw      5.06
64    tabw      5.06
 3    tabi      3.08
 5    tabi      3.08
 8    tabi      3.08
13    tabi      3.26
16    tabi      3.27
23    tabi      3.73
32    tabi      3.73
49    tabi      4.28
64    tabi      4.28
 3    tabiw     2.08
 5    tabiw     2.08
 8    tabiw     2.08
13    tabiw     1.77
16    tabiw     1.77
23    tabiw     2.27
32    tabiw     2.27
49    tabiw     2.57
64    tabiw     2.57
```

---

## Contact

Bug reports and feedback are highly welcome!  
Contact: **<danjel@mcgougan.se>**
