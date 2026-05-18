// bdpartidas.h
#include "partida.h"
#include "bdtimes.h"

#ifndef BDPARTIDAS_H
#define BDPARTIDAS_H

#define MAX_PARTIDAS 200

typedef struct{
    Partida* partidas[MAX_PARTIDAS];
    int quantidade;
} BDPartidas;

// Funções para manipular a estutura de dados BDPartidas:

// Função para criar partida
BDPartidas* bdpartidas_criar();

// Função para remover partida
void bdpartidas_destruir(BDPartidas* bd_p);

// Função para leitura de um arquivo bd_partidas.csv
int bdpartidas_carregar_csv(BDPartidas* bd_p, char* caminho);

// Função para listar partidas por time
void bdpartidas_listar_por_time(BDPartidas* bd_p, BDTimes* bd_t, int modo, char* prefixo);

// Função para processar partidas e armazenar resultados
void bdpartidas_processar_resultados(BDPartidas* bd_p, BDTimes* bd_t);

#endif