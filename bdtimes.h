// bdtimes.h
#include "time.h"

#ifndef BDTIMES_H
#define BDTIMES_H

#define COLS 10

// Definindo a estrutura de dados times
typedef struct{
    Time* times[10];
    int quantidade;
} BDTimes;

// Funções para a manipulação da esturura de dados BDTimes
int count_special(const char* str);

// Função para criar um BD Time
BDTimes* bdtimes_criar();

// Função para remover um BD Time
void bdtimes_free(BDTimes* bd_t);

// Função para leitura de um arquivo bd_time.csv
int bdtimes_carregar_csv(BDTimes* bd_t, char* caminho);

// Função para buscar o time referente a um ID
Time* bdtimes_buscar_id(BDTimes *bd_t, int id);

// Função para imprimir a tabela de times
void bdtimes_imprimir_tabela(BDTimes *bd_t);

// Função para buscar time por prefixo
void bdtimes_buscar_prefixo(BDTimes* bd_t, char* prefixo);

#endif