#include "../include/monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int collect_cpu(pid_t pid, struct cpu_info *out) {
    if (!out) return -1;
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    /* read fields according to procfs stat format */
    unsigned long utime=0, stime=0; long cutime=0, cstime=0; long num_threads=0;
    /* We only need to parse until num_threads (field 20) */
    char buf[1024];
    if (!fgets(buf, sizeof(buf), f)) { fclose(f); return -1; }
    fclose(f);
    /* Simplistic parse: skip pid and comm */
    char *p = strchr(buf, ')');
    if (!p) return -1;
    p++; /* after ) */
    /* now parse fields */
    /* fields: state(1) ppid(1) pgrp(1) session(1) tty_nr(1) tpgid(1) flags(1) minflt(1) cminflt(1) majflt(1) cmajflt(1) utime(1) stime(1) cutime(1) cstime(1) priority(1) nice(1) num_threads(1) ... */
    /* advance to utime (field 14 after comm) */
    int i;
    for (i=0;i<13;i++) {
        char *next = strchr(p, ' ');
        if (!next) return -1;
        p = next + 1;
    }
    if (sscanf(p, "%lu %lu %ld %ld %*s", &utime, &stime, &cutime, &cstime) < 2) {
        /* fallback: try basic sscanf */
    }
    /* find num_threads separately from /proc/[pid]/status */
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    f = fopen(path, "r");
    if (!f) return -1;
    char key[128];
    while (fgets(buf, sizeof(buf), f)) {
        if (sscanf(buf, "Threads: %ld", &num_threads) == 1) break;
    }
    rewind(f);
    unsigned long voluntary=0, nonvoluntary=0;
    while (fgets(buf, sizeof(buf), f)) {
        if (sscanf(buf, "voluntary_ctxt_switches: %lu", &voluntary) == 1) continue;
        if (sscanf(buf, "nonvoluntary_ctxt_switches: %lu", &nonvoluntary) == 1) continue;
    }
    fclose(f);

    out->utime_ticks = utime;
    out->stime_ticks = stime;
    out->num_threads = num_threads;
    out->voluntary_ctxt_switches = voluntary;
    out->nonvoluntary_ctxt_switches = nonvoluntary;
    return 0;
}
