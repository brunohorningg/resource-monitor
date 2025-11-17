#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <sys/types.h>

int list_namespaces(pid_t pid);
int find_procs_in_ns(const char *ns_path);
int compare_process_namespaces(pid_t a, pid_t b);

#endif
