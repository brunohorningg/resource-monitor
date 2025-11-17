# Regras e Especificações da Atividade – RA3

Documento consolidado contendo todas as regras, requisitos, proibições e orientações necessárias para implementação completa do trabalho conforme o arquivo oficial. Fonte: atividade fornecida pelo professor.  

---

## 1. Contexto e Motivação
Containers dependem de namespaces e cgroups do kernel Linux. O trabalho exige a criação de ferramentas de profiling, análise e limitação de recursos, utilizando mecanismos reais do kernel. É obrigatório compreender `/proc`, `/sys/fs/cgroup` e syscalls de namespaces.

---

## 2. Objetivos de Aprendizagem
Você deve:
- Entender profundamente `/proc`, `/sys/fs/cgroup` e syscalls de namespaces.
- Implementar coletores de métricas em C/C++23.
- Analisar overhead de isolamento e limitação.
- Avaliar trade‑offs de isolamento versus performance.
- Correlacionar métricas de processo, namespace e cgroup.
- Documentar os experimentos.
- Entregar código funcional, limpo e demonstrar autoria.

---

## 3. Descrição do Sistema a Desenvolver
O projeto consiste em **3 componentes obrigatórios**:

### 3.1 Resource Profiler
Ferramenta que coleta:
- CPU: user time, system time, context switches, threads.
- Memória: RSS, VSZ, page faults, swap.
- I/O: bytes lidos/escritos, syscalls, operações de disco.
- Rede: bytes, pacotes, conexões.

### 3.2 Namespace Analyzer
Ferramenta que:
- Identifica namespaces ativos.
- Mapeia processos por namespace.
- Compara namespaces entre processos.
- Mede overhead de criação de namespaces.
- Gera relatório completo do isolamento.

### 3.3 Control Group Manager
Ferramenta que:
- Lê métricas de CPU, Memory, BlkIO e PIDs.
- Cria cgroups experimentais.
- Move processos para cgroups.
- Aplica limites de CPU e memória.
- Mede precisão do throttling.
- Gera relatórios de uso e limites.

---

## 4. Requisitos Técnicos

### 4.1 Requisitos Obrigatórios
**Linguagens:**
- C ou C++ 2023 para coletores.
- Python/Shell apenas auxiliar.

**Resource Profiler obrigatório:**
- Monitorar PID com intervalo configurável.
- Coletar CPU, memória e I/O.
- Calcular CPU% e taxas de I/O.
- Exportar CSV/JSON.
- Tratar erros (PID inválido, permissão).

**Namespace Analyzer obrigatório:**
- Listar namespaces de um processo.
- Encontrar processos em um namespace.
- Comparar namespaces entre dois processos.
- Relatório global do sistema.

**Control Group Manager obrigatório:**
- Ler CPU, Memory, BlkIO.
- Criar cgroup.
- Mover processo.
- Definir limites.
- Relatório final.

**Qualidade de código:**
- Compilar sem warnings: `-Wall -Wextra`.
- Código limpo, estruturado e comentado.
- Makefile funcional.
- README completo.

### 4.2 Funcionalidades opcionais (extra)
- Zero memory leaks (valgrind).
- Interface ncurses.
- Suporte a cgroup v2.
- Dashboard web.
- Detecção de anomalias.
- Monitorar múltiplos processos.
- Comparar com ferramentas externas.

---

## 5. Estrutura Obrigatória do Projeto

```
resource-monitor/
├── README.md
├── Makefile
├── docs/
│   └── ARCHITECTURE.md
├── include/
│   ├── monitor.h
│   ├── namespace.h
│   └── cgroup.h
├── src/
│   ├── cpu_monitor.c
│   ├── memory_monitor.c
│   ├── io_monitor.c
│   ├── namespace_analyzer.c
│   ├── cgroup_manager.c
│   └── main.c
├── tests/
│   ├── test_cpu.c
│   ├── test_memory.c
│   └── test_io.c
└── scripts/
    ├── visualize.py
    └── compare_tools.sh
```

---

## 6. Experimentos Obrigatórios

### 6.1 Experimento 1 – Overhead do Profiler
Medições:
- Tempo com e sem profiler.
- CPU overhead.
- Latência amostral.

### 6.2 Experimento 2 – Isolamento via Namespaces
Medições:
- Tabela de isolamento.
- Overhead de criação.
- Nº de processos por namespace.

### 6.3 Experimento 3 – Throttling de CPU
Medições:
- CPU% real vs limite.
- Desvio percentual.
- Throughput.

### 6.4 Experimento 4 – Limitação de Memória
Medições:
- Máximo alocado.
- memory.failcnt.
- Comportamento no limite.

### 6.5 Experimento 5 – Limitação de I/O
Medições:
- Throughput vs limite.
- Latência.
- Tempo total.

---

## 7. Entregáveis e Regras de Git
- Commits equilibrados.
- Uso obrigatório de pull requests (exceto trabalho individual).
- README deve descrever exatamente:
  - Projeto.
  - Compilação.
  - Uso.
  - Dependências.
  - Autoria.

Falta de commits equilibrados implica penalidade severa.

---

## 8. Critérios de Avaliação (100 pts)
- Funcionalidade: 45
- Correção: 25
- Qualidade: 10
- Eficiência: 10
- Robustez: 10

**Deméritos:**
- Não compila → zero.
- Warnings → -10.
- Erros não tratados → -10.
- Suspeita de plágio → zero.
- Histórico de commits ruim → -30.
- PRs ausentes → -30.
- README incompleto → -10 por aluno (não se aplica para individual).

**Bônus:**
- Funcionalidades extras.
- Documentação excepcional.
- Testes automatizados.

---

## 9. Prova de Autoria
Você deve ser capaz de explicar **todo** o trabalho.
Se falhar → penalidade de 35%.

---

## 10. Restrições e Proibições
- Proibido usar bibliotecas externas (exceto libc e padrões do sistema).
- Proibido entregar código que não compila.
- Proibido copiar código de colegas ou IA.
- Proibido commits diretos na main (exceto trabalho individual).
- Proibido depender de root sem documentar.
- Proibido deixar de testar no ambiente onde será apresentado.

---

## 11. Ferramentas Permitidas
- strace, perf, valgrind, gdb.
- htop, iotop, systemd‑cgtop.
- gnuplot, matplotlib, graphviz.

---

## 12. Perguntas Frequentes (Resumo)
- Não precisa funcionar em qualquer distro.
- Não precisa interface gráfica.
- Algumas ações precisam de root.
- Código em C/C++ obrigatório.
- Python só para scripts auxiliares.
- Se não funcionar na apresentação → zero.

---

Fim do documento.  
