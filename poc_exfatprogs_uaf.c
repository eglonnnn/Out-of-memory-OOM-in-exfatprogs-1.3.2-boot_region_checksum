/*
 * Standalone reproducer for exfatprogs heap-use-after-free in fsck_main cleanup.
 * Build: clang-12 -fsanitize=address -g -O1 -I../../../exfatprogs/include \
 *          -Dmain=fsck_main -DPACKAGE='"exfatprogs"' -DVERSION='"1.3.2"' \
 *          poc_exfatprogs_uaf.c \
 *          ../../../exfatprogs/lib/libexfat.c ../../../exfatprogs/lib/exfat_fs.c \
 *          ../../../exfatprogs/lib/exfat_dir.c ../../../exfatprogs/lib/utils.c \
 *          ../../../exfatprogs/fsck/fsck.c ../../../exfatprogs/fsck/repair.c \
 *          -o poc_exfatprogs_uaf
 * Run: ./poc_exfatprogs_uaf <corrupted-exfat-image>
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int fsck_main(int argc, char * const argv[]);

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <exfat-image>\n", argv[0]);
        return 1;
    }

    /* fsck.exfat may call exit() internally; run in a loop to stress cleanup. */
    for (int i = 0; i < 100; i++) {
        char *args[] = {"fsck.exfat", "-n", argv[1], NULL};
        fsck_main(3, args);
    }
    return 0;
}
