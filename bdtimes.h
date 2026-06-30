// bdtimes.h
#include "time.h"

#ifndef BDTIMES_H
#define BDTIMES_H
#define COLS 10

// Estruturas de dados times e time_node
typedef struct time_node TimeNode;
typedef struct times BDTimes;

// Função para criar um BD Time (alocando a memoria necessaria)
BDTimes* bdtimes_criar();

// Desaloca memória da estrutura de dados BD Time
void bdtimes_free(BDTimes* bd_t);

// Carrega banco de dados a partir de um arquivo csv apontado por um caminho
int bdtimes_carregar_csv(BDTimes* bd_t, char* caminho);

// Função para buscar o time referente a um ID
Time* bdtimes_buscar_id(BDTimes *bd_t, int id);

// Função para imprimir a tabela de times
void bdtimes_imprimir_tabela(BDTimes *bd_t);

// Função para buscar time por prefixo
void bdtimes_buscar_prefixo(BDTimes* bd_t, char* prefixo);

// Função para resetar os dados de todos os times
void bdtimes_resetar_todos(BDTimes* bd_t);

// Função para salvar os dados da classificação em um arquivo csv apontado por um caminho
int bdtimes_salvar_classificacao_csv(BDTimes* bd_t, char* caminho);

// Função para listar apenas os IDs e Nomes de todos os times cadastrados
void bdtimes_listar_ids_nomes(BDTimes* bd_t);

#endif