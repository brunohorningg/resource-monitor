#include "../include/monitor.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    struct io_info io;
    if (collect_io(getpid(), &io) != 0) {
        fprintf(stderr, "collect_io failed\n");
        return 1;
    }
    printf("read=%lu write=%lu syscr=%lu syscw=%lu\n", io.read_bytes, io.write_bytes, io.syscr, io.syscw);
    return 0;
}
