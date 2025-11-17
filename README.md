# Resource Monitor — Projeto RA3

Projeto de monitoramento de recursos (skeleton).

Build
```
make all
```

Binaries serão gerados em `bin/`.

Componentes e branches
- `feature/resource-profiler`: coletores de CPU, memória e I/O
- `feature/namespace-analyzer`: analisador de namespaces
- `feature/cgroup-manager`: gerenciador de cgroups

Uso rápido
- Resource profiler (monitor PID a cada 1s):
```
bin/resource_profiler --pid 1234 --interval 1
```
- Namespace analyzer (listar namespaces de um PID):
```
bin/namespace_analyzer 1234
```
- Cgroup manager (exemplos; pode precisar de root):
```
# criar cgroup (v1 path example)
bin/cgroup_manager --create /sys/fs/cgroup/memory/mycg
# set memory
bin/cgroup_manager --set-mem /sys/fs/cgroup/memory/mycg 200000000
# add pid
bin/cgroup_manager --add-pid /sys/fs/cgroup/memory/mycg 1234
# read metrics
bin/cgroup_manager --read /sys/fs/cgroup/memory/mycg
```

Tests
```
make tests
bin/tests/test_cpu
bin/tests/test_memory
bin/tests/test_io
```

PRs
- Crie uma branch por feature (nome já sugerido acima) e abra um pull request quando pronta.

Observações
- Algumas operações (criar cgroups, mover processos) podem requerer privilégios de root.
