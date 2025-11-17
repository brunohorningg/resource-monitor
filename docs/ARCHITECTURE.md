
# Arquitetura (skeleton)

Este documento descreve a arquitetura inicial do projeto `resource-monitor`.

Overview
- Linguagem principal: C (C23) para coletores/gerenciadores
- Layout: headers em `include/`, fontes em `src/`, bins em `bin/`, testes em `tests/`

Componentes
- Resource Profiler: coletores para CPU, memória e I/O (em C). Gera linhas CSV no stdout. Binarry: `bin/resource_profiler`.
- Namespace Analyzer: identifica namespaces e permite comparar/mapeá-los. Binary: `bin/namespace_analyzer`.
- Control Group Manager: criação de cgroups, aplicação de limites, mover PIDs e leitura de métricas. Binary: `bin/cgroup_manager`.

Build
- Use `make all` para compilar os três binários.

Notes
- Implementações usam `procfs` e `sysfs` para leitura/escrita. Operações que alteram cgroups podem exigir privilégios de root.
