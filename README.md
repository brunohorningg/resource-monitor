# Resource Monitor — Projeto RA3

Projeto de monitoramento de recursos (skeleton). Este repositório contém a estrutura inicial exigida pela atividade RA3 e branches separados para cada componente: resource profiler, namespace analyzer e cgroup manager.

Compilação mínima:

```
make all
```

Binaries serão gerados em `bin/`.

Branches criadas para desenvolvimento (abra PRs a partir delas):
- `feature/resource-profiler`
- `feature/namespace-analyzer`
- `feature/cgroup-manager`

Leia `docs/ARCHITECTURE.md` para detalhes da arquitetura.

Nota: algumas operações (criar cgroups, mover processos) podem requerer privilégios de root.
