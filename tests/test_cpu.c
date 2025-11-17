#include "../include/monitor.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    struct cpu_info c;
    if (collect_cpu(getpid(), &c) != 0) {
        fprintf(stderr, "collect_cpu failed\n");
        return 1;
    }
    printf("utime=%lu stime=%lu threads=%ld\n", c.utime_ticks, c.stime_ticks, c.num_threads);
    return 0;
}
