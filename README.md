# Campeonato Computacional de Futebol — Parte II

Sistema de gerenciamento de partidas e resultados de um campeonato de futebol, implementado em linguagem C. Os dados são persistidos em arquivos CSV e carregados em uma lista simplesmente encadeada em memória, permitindo consulta, inserção, atualização e remoção de partidas, além da impressão da tabela de classificação ordenada por mérito esportivo.

---

## Estrutura do Repositório

```
├── times.csv              # Base de dados dos times cadastrados
├── bd_partidas.csv        # Base de dados das partidas (regravado pelo programa)
├── bd_classificacao.csv   # Gerado pelo programa: tabela de classificação ordenada
├── partidas_vazio.csv     # Cenário 1: campeonato sem partidas disputadas
├── partidas_parcial.csv   # Cenário 2: campeonato em andamento
├── partidas_completo.csv  # Cenário 3: campeonato finalizado
├── Makefile               # Automação de compilação e execução
├── main.c                 # Ponto de entrada do programa
├── time.c / time.h        # TAD Time
├── bdtimes.c / bdtimes.h  # TAD BDTimes
├── partida.c / partida.h  # TAD Partida
├── bdpartidas.c / bdpartidas.h  # TAD BDPartidas
└── texto.c / texto.h      # Funções utilitárias de processamento de texto
```

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

> Atenção: ao inserir, atualizar ou remover uma partida pelo menu, o sistema regrava `bd_partidas.csv` automaticamente. Para testar outro cenário do zero, copie o arquivo novamente antes de executar.

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
3 - Atualizar partida
4 - Remover partida
5 - Inserir partida
6 - Imprimir tabela de classificação
Q - Sair
```

**Opção 1 — Consultar time:** digite o nome ou um prefixo (ex.: `Se` retorna SemCTRL e SeQueLas).

**Opção 2 — Consultar partidas:** escolha o modo de busca (mandante, visitante ou ambos) e informe o prefixo do nome do time.

**Opção 3 — Atualizar partida:** reaproveita a consulta de partidas (opção 2) para localizar o registro. Em seguida, informe o ID da partida e o novo placar de cada time — digite `-` em um campo para manter o valor atual. Confirme com `S` para aplicar a alteração.

**Opção 4 — Remover partida:** também reaproveita a consulta de partidas para localizar o ID a ser excluído, pedindo confirmação antes de remover.

**Opção 5 — Inserir partida:** informe os IDs dos dois times e o placar de cada um. O ID da nova partida é gerado automaticamente.

**Opção 6 — Imprimir tabela:** exibe todos os times ordenados por mérito esportivo (Pontos Ganhos desc, Vitórias desc, Saldo de Gols desc) e salva o mesmo resultado em `bd_classificacao.csv`:
- Vitórias (V)
- Empates (E)
- Derrotas (D)
- Gols marcados (GM)
- Gols sofridos (GS)
- Saldo de Gols (S)
- Pontos ganhos (PG)

**Q — Sair:** encerra o programa e libera a memória alocada.

### 4. Limpeza

```bash
make clean
```

---

## Sobre os arquivos

**`times.csv`**
Contém os 10 times do campeonato no formato `ID,Time`. Deve estar no mesmo diretório do executável.

**`bd_partidas.csv`**
Arquivo de partidas ativo, lido pelo programa ao iniciar. Sempre que uma partida é inserida, atualizada ou removida pelo menu, o arquivo é regravado automaticamente para refletir o estado em memória.

**`bd_classificacao.csv`**
Gerado automaticamente pela opção 6 do menu, contém a tabela de classificação já ordenada por mérito esportivo.

**`Makefile`**
- `make` — compila e executa o programa
- `make compile` — apenas compila
- `make clean` — remove arquivos objeto e executável

**`main.c`**
Carrega os dois CSVs em memória, processa os resultados das partidas e exibe o menu interativo ao usuário, incluindo as funcionalidades de manutenção de dados (atualizar, remover e inserir partida).

**`time.c / time.h`**
Implementam o TAD `Time`, que representa uma equipe com seus dados de identificação e estatísticas acumuladas de desempenho.

**`bdtimes.c / bdtimes.h`**
Implementam o TAD `BDTimes`, que gerencia a coleção de times em uma lista simplesmente encadeada: carregamento do CSV, busca por ID, busca por prefixo de nome, reset das estatísticas e impressão/exportação da tabela de classificação ordenada.

**`partida.c / partida.h`**
Implementam o TAD `Partida`, que representa um jogo entre dois times com seus respectivos placares.

**`bdpartidas.c / bdpartidas.h`**
Implementam o TAD `BDPartidas`, que gerencia a coleção de partidas em uma lista simplesmente encadeada: carregamento e gravação do CSV, busca por ID, inserção/atualização/remoção de registros, listagem por time e processamento de resultados para atualizar as estatísticas dos times.

**`texto.c / texto.h`**
Função utilitária (`count_special`) que conta os bytes de continuação UTF-8 em uma string, usada para corrigir o alinhamento de colunas no `printf` quando o nome de um time tem acento.

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
- `time_resetar_estatisticas()` — zera as estatísticas acumuladas, usado antes de recalcular a tabela do zero

### 2. BDTimes

Definido em `bdtimes.h`, gerencia uma lista simplesmente encadeada de `Time`, com ponteiro para o primeiro (`first`) nó da lista. Principais operações:

- `bdtimes_carregar_csv()` — lê `times.csv` e cria os times em memória

- `bdtimes_buscar_id()` — localiza um time pelo seu ID

- `bdtimes_buscar_prefixo()` — busca times cujo nome começa com o prefixo informado e imprime suas estatísticas

- `bdtimes_resetar_todos()` — zera as estatísticas de todos os times, usado antes de reprocessar as partidas

- `bdtimes_imprimir_tabela()` — exibe a tabela de classificação ordenada por mérito esportivo (PG desc, V desc, S desc)

- `bdtimes_salvar_classificacao_csv()` — salva essa mesma tabela ordenada em `bd_classificacao.csv`

### 3. Partida

Definido em `partida.h`, representa um jogo com os campos:

- `id` (int): identificador da partida

- `id_time1`, `id_time2` (int): IDs dos times participantes

- `gols_time1`, `gols_time2` (int): gols de cada time

Além dos getters, oferece:
- `partida_set_g1()` / `partida_set_g2()` — atualizam o placar de uma partida já existente, usados pela funcionalidade de atualização

### 4. BDPartidas

Definido em `bdpartidas.h`, gerencia uma lista simplesmente encadeada de `Partida`, também com ponteiro `first`. Mantém ainda um contador interno (`id`) usado para gerar o ID da próxima partida inserida — esse contador não pode ser simplesmente o tamanho da lista, pois uma remoção diminuiria esse valor e poderia gerar um ID já usado por outro registro. Principais operações:

- `bdpartidas_carregar_csv()` / `bdpartidas_salvar_csv()` — leitura e gravação do arquivo de partidas

- `bdpartidas_buscar_id()` — localiza uma partida pelo ID

- `bdpartidas_inserir()` / `bdpartidas_atualizar()` / `bdpartidas_remover()` — operações de manutenção dos dados

- `bdpartidas_processar_resultados()` — percorre todas as partidas e chama `time_registrar_resultado()` para cada time, recalculando as estatísticas

- `bdpartidas_listar_por_time()` — lista partidas filtrando por mandante, visitante ou ambos, com busca por prefixo de nome

---

## Principais Decisões de Implementação

- **Lista simplesmente encadeada: ** tanto `BDTimes` quanto `BDPartidas` usam uma lista encadeada (em vez do vetor estático da Parte I), permitindo inserir e remover partidas sem um limite fixo de tamanho.

- **Recalcular as estatísticas do zero a cada alteração:** em vez de ajustar V/E/D/GM/GS incrementalmente a cada inserção, atualização ou remoção, o sistema sempre chama `bdtimes_resetar_todos()` seguido de `bdpartidas_processar_resultados()`. Como o campeonato tem só 10 times e poucas dezenas de partidas, recalcular tudo é rápido e muito mais simples de manter correto.

- **Contador de próximo ID independente do tamanho da lista:** o `BDPartidas` guarda um contador que só cresce, nunca reaproveitando IDs de partidas removidas, evitando colisões de ID após uma remoção.

- **Ordenação por cópia em vetor:** para a tabela de classificação, os times são copiados da lista encadeada para um vetor temporário de ponteiros e ordenados com bubble sort (suficiente para 10 elementos), comparando Pontos Ganhos, Vitórias e Saldo de Gols, nessa ordem de prioridade. A lista original não é alterada, só o vetor temporário.

- **Persistência após cada operação:** toda inserção, atualização ou remoção de partida regrava `bd_partidas.csv` imediatamente, e a opção de imprimir a tabela também gera `bd_classificacao.csv` — assim os arquivos em disco nunca ficam desatualizados em relação ao estado em memória.

- **Módulo separado para funções de texto:** a contagem de caracteres especiais (usada para alinhar corretamente colunas com nomes acentuados, já que cada acento ocupa 2 bytes em UTF-8 mas só 1 espaço visível) foi isolada em `texto.c`/`texto.h`, por ser uma função genérica de processamento de string, sem relação direta com times ou partidas.

- **Dados derivados calculados sob demanda:** saldo de gols e pontos ganhos não são armazenados na struct — são calculados pelas funções `time_saldo_gols()` e `time_pontos()` no momento em que são necessários. Isso evita inconsistências caso os dados base sejam alterados no futuro.

- **Busca por prefixo com `strncasecmp`:** a busca de times e partidas compara apenas os primeiros `n` caracteres do nome (onde `n` é o tamanho do prefixo digitado), permitindo encontrar múltiplos resultados com uma entrada parcial independentemente da capitalização (maiúscula ou minúscula).

- **Modularização por TAD:** cada TAD tem seu próprio par `.c`/`.h`, com interface pública definida no `.h` e implementação encapsulada no `.c`. O `main.c` interage apenas com as funções públicas, sem acessar os campos das structs diretamente.