# Arquitetura (skeleton)

Este documento descreve a arquitetura inicial do projeto `resource-monitor`.

Componentes:
- Resource Profiler: coletores para CPU, memória e I/O (em C). Gera CSV/JSON.
- Namespace Analyzer: identifica namespaces e mapeia processos.
- Control Group Manager: operações básicas de leitura/criação de cgroups.

Cada componente terá um binário em `bin/` e headers em `include/`.
