# Campeonato Computacional de Futebol — Parte I

Sistema de gerenciamento de partidas e resultados de um campeonato de futebol, implementado em linguagem C. Os dados são persistidos em arquivos CSV e carregados em memória para consulta durante a execução.

---

## Estrutura do Repositório

```
├── times.csv              # Base de dados dos times cadastrados
├── bd_partidas.csv        # Base de dados das partidas (substituir conforme o cenário)
├── partidas_vazio.csv     # Cenário 1: campeonato sem partidas disputadas
├── partidas_parcial.csv   # Cenário 2: campeonato em andamento
├── partidas_completo.csv  # Cenário 3: campeonato finalizado
├── Makefile               # Automação de compilação e execução
├── main.c                 # Ponto de entrada do programa
├── time.c / time.h        # TAD Time
├── bdtimes.c / bdtimes.h  # TAD BDTimes
├── partida.c / partida.h  # TAD Partida
└── bdpartidas.c / bdpartidas.h  # TAD BDPartidas
```

---

### Sobre os arquivos

**`times.csv`**
Contém os 10 times do campeonato no formato `ID,Time`. Deve estar no mesmo diretório do executável.

**`bd_partidas.csv`**
Arquivo de partidas ativo, lido pelo programa ao iniciar. Para trocar de cenário, copie o arquivo desejado com esse nome (veja as instruções de execução).

**`Makefile`**
- `make` — compila e executa o programa
- `make compile` — apenas compila
- `make clean` — remove arquivos objeto e executável

**`main.c`**
Carrega os dois CSVs em memória, processa os resultados das partidas e exibe o menu interativo ao usuário.

**`time.c / time.h`**
Implementam o TAD `Time`, que representa uma equipe com seus dados de identificação e estatísticas acumuladas de desempenho.

**`bdtimes.c / bdtimes.h`**
Implementam o TAD `BDTimes`, que gerencia a coleção de times: carregamento do CSV, busca por ID, busca por prefixo de nome e impressão da tabela de classificação.

**`partida.c / partida.h`**
Implementam o TAD `Partida`, que representa um jogo entre dois times com seus respectivos placares.

**`bdpartidas.c / bdpartidas.h`**
Implementam o TAD `BDPartidas`, que gerencia a coleção de partidas: carregamento do CSV, listagem por time e processamento de resultados para atualizar as estatísticas dos times.

---

## TADs Utilizados

### 1. Time

Definido em `time.h`, representa uma equipe de futebol com os campos:

- `id` (int): identificador único do time
- `nome` (char[50]): nome do time
- `vitorias`, `empates`, `derrotas` (int): contadores de resultados
- `gols_marcados`, `gols_sofridos` (int): totais de gols

Oferece funções para obter dados derivados calculados sob demanda:
- `time_saldo_gols()` — diferença entre gols marcados e sofridos
- `time_pontos()` — calculado como 3 × vitórias + empates
- `time_partidas()` — soma de vitórias, empates e derrotas
- `time_registrar_resultado()` — atualiza as estatísticas a partir do placar de uma partida

### 2. BDTimes

Definido em `bdtimes.h`, gerencia um vetor estático de até 10 ponteiros para `Time`. Principais operações:

- `bdtimes_carregar_csv()` — lê `times.csv` e cria os times em memória
- `bdtimes_buscar_id()` — localiza um time pelo seu ID
- `bdtimes_buscar_prefixo()` — busca times cujo nome começa com o prefixo informado e imprime suas estatísticas
- `bdtimes_imprimir_tabela()` — exibe a tabela de classificação completa ordenada por ID

### 3. Partida

Definido em `partida.h`, representa um jogo com os campos:

- `id` (int): identificador da partida
- `id_time1`, `id_time2` (int): IDs dos times participantes
- `gols_time1`, `gols_time2` (int): gols de cada time

### 4. BDPartidas

Definido em `bdpartidas.h`, gerencia um vetor estático de até 200 ponteiros para `Partida`. Principais operações:

- `bdpartidas_carregar_csv()` — lê `bd_partidas.csv` e cria as partidas em memória
- `bdpartidas_processar_resultados()` — percorre todas as partidas e chama `time_registrar_resultado()` para cada time, acumulando as estatísticas
- `bdpartidas_listar_por_time()` — lista partidas filtrando por mandante, visitante ou ambos, com busca por prefixo de nome

---

## Instruções para Execução

### Pré-requisitos

- Compilador GCC instalado
- Sistema Linux (testado no Ubuntu 22.04 com GCC 11)
- Todos os arquivos `.c`, `.h`, `Makefile` e CSVs na mesma pasta

### 1. Escolher o cenário de teste

Copie o arquivo de partidas desejado com o nome `bd_partidas.csv`:

```bash
# Cenário 1 — campeonato vazio (nenhuma partida disputada)
cp partidas_vazio.csv bd_partidas.csv

# Cenário 2 — campeonato em andamento
cp partidas_parcial.csv bd_partidas.csv

# Cenário 3 — campeonato finalizado
cp partidas_completo.csv bd_partidas.csv
```

### 2. Compilar e executar

```bash
make
```

O `make` sozinho compila e já executa o programa. Para compilar sem executar:

```bash
make compile
```

### 3. Usar o menu

Ao iniciar, o sistema exibe o menu principal:

```
=== Sistema de Gerenciamento de Partidas ===
1 - Consultar time
2 - Consultar partidas
3 - Atualizar partida (desabilitado)
4 - Remover partida (desabilitado)
5 - Inserir partida (desabilitado)
6 - Imprimir tabela de classificação
Q - Sair
```

**Opção 1 — Consultar time:** digite o nome ou um prefixo (ex.: `Se` retorna SemCTRL e SeQueLas).

**Opção 2 — Consultar partidas:** escolha o modo de busca (mandante, visitante ou ambos) e informe o prefixo do nome do time.

**Opção 6 — Imprimir tabela:** exibe todos os times com suas estatísticas acumuladas (V, E, D, GM, GS, S, PG).

**Q — Sair:** encerra o programa e libera a memória alocada.

### 4. Limpeza

```bash
make clean
```

---

## Principais Decisões de Implementação

- **Vetor estático com alocação dinâmica dos elementos:** o vetor de ponteiros é estático (tamanho fixo definido em tempo de compilação), mas cada `Time` e cada `Partida` é alocado individualmente com `malloc`. Isso combina a previsibilidade de memória do vetor estático com a flexibilidade do acesso via ponteiro, conforme sugerido pelo enunciado.

- **Separação entre leitura e processamento:** o carregamento dos CSVs (`bdtimes_carregar_csv` e `bdpartidas_carregar_csv`) apenas popula as estruturas com os dados brutos. O cálculo das estatísticas é feito em uma etapa separada (`bdpartidas_processar_resultados`), chamada uma única vez no início. Isso torna cada função com uma responsabilidade clara.

- **Dados derivados calculados sob demanda:** saldo de gols e pontos ganhos não são armazenados na struct — são calculados pelas funções `time_saldo_gols()` e `time_pontos()` no momento em que são necessários. Isso evita inconsistências caso os dados base sejam alterados no futuro.

- **Busca por prefixo com `strncmp`:** a busca de times e partidas compara apenas os primeiros `n` caracteres do nome (onde `n` é o tamanho do prefixo digitado), permitindo encontrar múltiplos resultados com uma entrada parcial.

- **Modularização por TAD:** cada TAD tem seu próprio par `.c`/`.h`, com interface pública definida no `.h` e implementação encapsulada no `.c`. O `main.c` interage apenas com as funções públicas, sem acessar os campos das structs diretamente.
