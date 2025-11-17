// monitor.h
#ifndef MONITOR_H
#define MONITOR_H

#include <sys/types.h>

struct cpu_info {
    unsigned long utime_ticks;
    unsigned long stime_ticks;
    long num_threads;
    unsigned long voluntary_ctxt_switches;
    unsigned long nonvoluntary_ctxt_switches;
};

struct mem_info {
    unsigned long vm_size_kb;
    unsigned long vm_rss_kb;
    unsigned long page_faults;
    unsigned long swap_kb;
};

struct io_info {
    unsigned long read_bytes;
    unsigned long write_bytes;
    unsigned long syscr;
    unsigned long syscw;
};

int collect_cpu(pid_t pid, struct cpu_info *out);
int collect_mem(pid_t pid, struct mem_info *out);
int collect_io(pid_t pid, struct io_info *out);

#endif
