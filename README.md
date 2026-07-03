# Out-of-memory in exfatprogs 1.3.2 `boot_region_checksum`

## Description

An out-of-memory vulnerability in exfatprogs 1.3.2 (denial-of-service). When `fsck_main()` processes a crafted exFAT image, `boot_region_checksum()` reads the sector count from the boot region and allocates a corresponding buffer. A malformed image can declare an extremely large sector count, causing `malloc(2147483648)` (2GB), which exceeds the memory limit and triggers an OOM.

The allocation occurs at `boot_region_checksum()` called through `read_boot_region()` → `exfat_boot_region_check()` → `fsck_main()`.

## Affected version

exfatprogs 1.3.2

## Reproduction

This repository includes `poc_exfatprogs_oom.bin`, a 1330-byte crafted exFAT image that triggers the OOM.

```bash
ASAN_OPTIONS=detect_leaks=0 \
/home/fuzz_exfat \
/home/poc_exfatprogs_oom.bin
```

### Expected output

```
exfatprogs version : 1.3.2
==...== ERROR: libFuzzer: out-of-memory (malloc(2147483648))

    #0 boot_region_checksum
    #1 read_boot_region
    #2 exfat_boot_region_check
    #3 fsck_main

SUMMARY: libFuzzer: out-of-memory
```
