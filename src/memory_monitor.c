#include "../include/monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int collect_mem(pid_t pid, struct mem_info *out) {
    if (!out) return -1;
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char buf[256];
    unsigned long vmrss=0, vmsize=0, faults=0, swap=0;
    while (fgets(buf, sizeof(buf), f)) {
        if (sscanf(buf, "VmSize: %lu kB", &vmsize) == 1) continue;
        if (sscanf(buf, "VmRSS: %lu kB", &vmrss) == 1) continue;
        if (sscanf(buf, "PageFaults: %lu", &faults) == 1) continue;
        if (sscanf(buf, "VmSwap: %lu kB", &swap) == 1) continue;
    }
    fclose(f);
    out->vm_size_kb = vmsize;
    out->vm_rss_kb = vmrss;
    out->page_faults = faults;
    out->swap_kb = swap;
    return 0;
}
