#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "../include/monitor.h"

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s --pid PID [--interval SECONDS]\n", prog);
}

int main(int argc, char **argv) {
    pid_t pid = 0;
    int interval = 1;
    for (int i=1;i<argc;i++) {
        if (strcmp(argv[i], "--pid") == 0 && i+1<argc) { pid = (pid_t)atoi(argv[++i]); }
        else if (strcmp(argv[i], "--interval") == 0 && i+1<argc) { interval = atoi(argv[++i]); }
        else { usage(argv[0]); return 1; }
    }
    if (pid <= 0) { usage(argv[0]); return 1; }

    printf("timestamp,utime_ticks,stime_ticks,num_threads,read_bytes,write_bytes,vm_size_kb,vm_rss_kb\n");
    while (1) {
        struct cpu_info c = {0};
        struct io_info io = {0};
        struct mem_info m = {0};
        if (collect_cpu(pid, &c) != 0) { fprintf(stderr, "failed to collect cpu for %d\n", pid); return 2; }
        if (collect_io(pid, &io) != 0) { /* best effort */ }
        if (collect_mem(pid, &m) != 0) { /* best effort */ }
        time_t t = time(NULL);
        printf("%ld,%lu,%lu,%ld,%lu,%lu,%lu,%lu\n", (long)t, c.utime_ticks, c.stime_ticks, c.num_threads, io.read_bytes, io.write_bytes, m.vm_size_kb, m.vm_rss_kb);
        fflush(stdout);
        sleep(interval);
    }
    return 0;
}
