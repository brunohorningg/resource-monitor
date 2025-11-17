#include "../include/monitor.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    struct mem_info m;
    if (collect_mem(getpid(), &m) != 0) {
        fprintf(stderr, "collect_mem failed\n");
        return 1;
    }
    printf("vmsize=%lu vmrss=%lu swap=%lu\n", m.vm_size_kb, m.vm_rss_kb, m.swap_kb);
    return 0;
}
