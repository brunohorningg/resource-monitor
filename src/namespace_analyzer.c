#include "../include/namespace.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

static int print_ns_link(const char *path) {
    char buf[256];
    ssize_t r = readlink(path, buf, sizeof(buf)-1);
    if (r < 0) return -1;
    buf[r] = '\0';
    printf("%s -> %s\n", path, buf);
    return 0;
}

int list_namespaces(pid_t pid) {
    char base[256];
    snprintf(base, sizeof(base), "/proc/%d/ns", pid);
    DIR *d = opendir(base);
    if (!d) { perror("opendir"); return -1; }
    struct dirent *de;
    while ((de = readdir(d))) {
        if (de->d_name[0] == '.') continue;
        char path[512];
        snprintf(path, sizeof(path), "%s/%s", base, de->d_name);
        print_ns_link(path);
    }
    closedir(d);
    return 0;
}

int compare_process_namespaces(pid_t a, pid_t b) {
    char path_a[256], path_b[256];
    snprintf(path_a, sizeof(path_a), "/proc/%d/ns", a);
    snprintf(path_b, sizeof(path_b), "/proc/%d/ns", b);
    DIR *da = opendir(path_a);
    DIR *db = opendir(path_b);
    if (!da || !db) { if (da) closedir(da); if (db) closedir(db); return -1; }
    struct dirent *dea;
    while ((dea = readdir(da))) {
        if (dea->d_name[0] == '.') continue;
        char pa[512], pb[512];
        snprintf(pa, sizeof(pa), "%s/%s", path_a, dea->d_name);
        snprintf(pb, sizeof(pb), "%s/%s", path_b, dea->d_name);
        char ra[256], rb[256];
        ssize_t la = readlink(pa, ra, sizeof(ra)-1);
        ssize_t lb = readlink(pb, rb, sizeof(rb)-1);
        if (la<0 || lb<0) continue;
        ra[la]='\0'; rb[lb]='\0';
        printf("ns %s: %s vs %s -> %s\n", dea->d_name, ra, rb, strcmp(ra, rb)==0?"SAME":"DIFFERENT");
    }
    closedir(da); closedir(db);
    return 0;
}

int find_procs_in_ns(const char *ns_path) {
    /* ns_path should be like /proc/<pid>/ns/<type> or a /proc/*/ns/<type> link value */
    /* This is a simple helper: iterate /proc, for each pid compare the link value */
    char target[256];
    ssize_t rt = readlink(ns_path, target, sizeof(target)-1);
    if (rt < 0) { perror("readlink target"); return -1; }
    target[rt] = '\0';
    DIR *d = opendir("/proc");
    if (!d) { perror("opendir /proc"); return -1; }
    struct dirent *de;
    while ((de = readdir(d))) {
        if (de->d_type != DT_DIR) continue;
        pid_t pid = atoi(de->d_name);
        if (pid <= 0) continue;
        char probe[512];
        snprintf(probe, sizeof(probe), "/proc/%d/ns/%s", pid, strrchr(ns_path, '/') ? strrchr(ns_path, '/')+1 : "");
        char val[256];
        ssize_t l = readlink(probe, val, sizeof(val)-1);
        if (l < 0) continue;
        val[l] = '\0';
        if (strcmp(val, target) == 0) {
            printf("%d\n", pid);
        }
    }
    closedir(d);
    return 0;
}
