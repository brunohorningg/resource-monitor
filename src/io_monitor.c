#include "../include/monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int collect_io(pid_t pid, struct io_info *out) {
    if (!out) return -1;
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/io", pid);
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char buf[256];
    unsigned long read_bytes=0, write_bytes=0, syscr=0, syscw=0;
    while (fgets(buf, sizeof(buf), f)) {
        if (sscanf(buf, "read_bytes: %lu", &read_bytes) == 1) continue;
        if (sscanf(buf, "write_bytes: %lu", &write_bytes) == 1) continue;
        if (sscanf(buf, "syscr: %lu", &syscr) == 1) continue;
        if (sscanf(buf, "syscw: %lu", &syscw) == 1) continue;
    }
    fclose(f);
    out->read_bytes = read_bytes;
    out->write_bytes = write_bytes;
    out->syscr = syscr;
    out->syscw = syscw;
    return 0;
}
