#include "../include/cgroup.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

static int write_string_file(const char *path, const char *value) {
    int fd = open(path, O_WRONLY | O_CLOEXEC);
    if (fd < 0) return -1;
    ssize_t len = write(fd, value, strlen(value));
    close(fd);
    return (len == (ssize_t)strlen(value)) ? 0 : -1;
}

int create_cgroup(const char *cgroup_path) {
    if (!cgroup_path) return -1;
    if (mkdir(cgroup_path, 0755) == 0) return 0;
    if (errno == EEXIST) return 0;
    return -1;
}

int set_memory_limit(const char *cgroup_path, unsigned long bytes) {
    if (!cgroup_path) return -1;
    char path[512];
    snprintf(path, sizeof(path), "%s/memory.limit_in_bytes", cgroup_path);
    char buf[64];
    snprintf(buf, sizeof(buf), "%lu", bytes);
    return write_string_file(path, buf);
}

int set_cpu_shares(const char *cgroup_path, unsigned long shares) {
    if (!cgroup_path) return -1;
    char path[512];
    snprintf(path, sizeof(path), "%s/cpu.shares", cgroup_path);
    char buf[64];
    snprintf(buf, sizeof(buf), "%lu", shares);
    return write_string_file(path, buf);
}

int add_pid_to_cgroup(const char *cgroup_path, pid_t pid) {
    if (!cgroup_path) return -1;
    char path[512];
    /* try cgroup.procs first (works on v1 and v2) */
    snprintf(path, sizeof(path), "%s/cgroup.procs", cgroup_path);
    char buf[64];
    snprintf(buf, sizeof(buf), "%d", (int)pid);
    if (write_string_file(path, buf) == 0) return 0;
    /* fallback to tasks */
    snprintf(path, sizeof(path), "%s/tasks", cgroup_path);
    return write_string_file(path, buf);
}

int read_file_ulong(const char *path, unsigned long *out) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    unsigned long v = 0;
    if (fscanf(f, "%lu", &v) != 1) { fclose(f); return -1; }
    fclose(f);
    *out = v;
    return 0;
}

int read_cgroup_metrics(const char *cgroup_path) {
    if (!cgroup_path) return -1;
    unsigned long v;
    char path[512];
    /* memory */
    snprintf(path, sizeof(path), "%s/memory.usage_in_bytes", cgroup_path);
    if (read_file_ulong(path, &v) == 0) printf("memory.usage_in_bytes: %lu\n", v);
    else printf("memory.usage_in_bytes: (n/a)\n");
    /* cpuacct */
    snprintf(path, sizeof(path), "%s/cpuacct.usage", cgroup_path);
    if (read_file_ulong(path, &v) == 0) printf("cpuacct.usage: %lu\n", v);
    else printf("cpuacct.usage: (n/a)\n");
    /* blkio */
    snprintf(path, sizeof(path), "%s/blkio.throttle.io_service_bytes", cgroup_path);
    FILE *f = fopen(path, "r");
    if (f) {
        char line[256];
        printf("blkio.throttle.io_service_bytes:\n");
        while (fgets(line, sizeof(line), f)) printf("  %s", line);
        fclose(f);
    } else {
        printf("blkio.throttle.io_service_bytes: (n/a)\n");
    }
    return 0;
}

/* Simple CLI to perform create/move/read operations. Examples:
 *  bin/cgroup_manager --create /sys/fs/cgroup/memory/mycg
 *  bin/cgroup_manager --set-mem /sys/fs/cgroup/memory/mycg 200000000
 *  bin/cgroup_manager --set-cpu /sys/fs/cgroup/cpu/mycg 512
 *  bin/cgroup_manager --add-pid /sys/fs/cgroup/memory/mycg 1234
 *  bin/cgroup_manager --read /sys/fs/cgroup/memory/mycg
 */
int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s --create|--set-mem|--set-cpu|--add-pid|--read <cgroup_path> [value]\n", argv[0]);
        return 1;
    }
    const char *cmd = argv[1];
    const char *path = argv[2];
    if (strcmp(cmd, "--create")==0) {
        if (create_cgroup(path) != 0) { perror("create_cgroup"); return 2; }
        printf("created %s\n", path);
        return 0;
    } else if (strcmp(cmd, "--set-mem")==0) {
        if (argc < 4) { fprintf(stderr, "missing bytes value\n"); return 1; }
        unsigned long bytes = strtoul(argv[3], NULL, 10);
        if (set_memory_limit(path, bytes) != 0) { perror("set_memory_limit"); return 2; }
        printf("set memory limit %s = %lu\n", path, bytes);
        return 0;
    } else if (strcmp(cmd, "--set-cpu")==0) {
        if (argc < 4) { fprintf(stderr, "missing shares value\n"); return 1; }
        unsigned long s = strtoul(argv[3], NULL, 10);
        if (set_cpu_shares(path, s) != 0) { perror("set_cpu_shares"); return 2; }
        printf("set cpu shares %s = %lu\n", path, s);
        return 0;
    } else if (strcmp(cmd, "--add-pid")==0) {
        if (argc < 4) { fprintf(stderr, "missing pid\n"); return 1; }
        pid_t pid = (pid_t)atoi(argv[3]);
        if (add_pid_to_cgroup(path, pid) != 0) { perror("add_pid_to_cgroup"); return 2; }
        printf("added pid %d to %s\n", (int)pid, path);
        return 0;
    } else if (strcmp(cmd, "--read")==0) {
        return read_cgroup_metrics(path) == 0 ? 0 : 2;
    }
    fprintf(stderr, "unknown command\n");
    return 1;
}
