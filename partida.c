// partida.c
#include "partida.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para criar uma partida
Partida* partida_criar(int id, int id_t1, int id_t2, int g1, int g2)
{
    Partida *p = (Partida *) malloc(sizeof(Partida)); // Alocando memória para uma nova partida
    if (p == NULL){
        printf("Erro ao alocar memória para partida.\n");
        return NULL;
    }
    p->id = id;
    p->id_time1 = id_t1;
    p->id_time2 = id_t2;
    p->gols_time1 = g1;
    p->gols_time2 = g2;
    return p;
}

// Função para remover uma partida
void partida_free(Partida* p)
{
    if (p != NULL){
        free(p);
    }
}

// Função que retorna o ID de uma partida
int partida_get_id(Partida* p)
{
    return p->id;
}

// Função que retorna o ID do primeiro time participante de uma partida
int partida_get_id_t1(Partida* p)
{
    return p->id_time1;
}

// Função que retorna o ID do segundo time participante de uma partida
int partida_get_id_t2(Partida* p)
{
    return p->id_time2;
}

// Função que retorna o número de gols do primeiro time participante de uma partida
int partida_get_g1(Partida* p)
{
    return p->gols_time1;
}

// Função que retorna o número de gols do segundo time participante de uma partida
int partida_get_g2(Partida* p)
{
    return p->gols_time2;
}

// Função para atualizar atualizar o número de gols em uma partida (g1)
void partida_set_g1(Partida* p, int gols){
    if (p != NULL){
        p->gols_time1 = gols;
    }
}

// Função para atualizar o número de gols em uma partida (g2)
void partida_set_g2(Partida* p, int gols){
    if (p != NULL){
        p->gols_time2 = gols;
    }
}