#ifndef CGROUP_H
#define CGROUP_H

#include <sys/types.h>

/* Basic cgroup manager helpers.
 * Implemented to work with typical cgroup v1 layout under /sys/fs/cgroup.
 * Functions accept full cgroup path (e.g. /sys/fs/cgroup/memory/mytest)
 */

int create_cgroup(const char *cgroup_path);
int set_memory_limit(const char *cgroup_path, unsigned long bytes);
int set_cpu_shares(const char *cgroup_path, unsigned long shares);
int add_pid_to_cgroup(const char *cgroup_path, pid_t pid);
int read_cgroup_metrics(const char *cgroup_path);

#endif
