// time.c
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para criar um time
Time* time_criar(int id, char* nome)
{
    Time *t = (Time *) malloc(sizeof(Time)); // Alocando memória para um novo time
    if (t == NULL) {
        printf("Erro ao alocar memória para o time.\n");
        return NULL;
    }
    t->id = id;
    strcpy(t->nome, nome);
    t->vitorias = 0;
    t->empates = 0;
    t->derrotas = 0;
    t->gols_marcados = 0;
    t->gols_sofridos = 0;
    return t;
}

// Função para free um time
void time_free(Time* t)
{
    if (t != NULL) {
        free(t);
    }
}

// Função que retorna o ID de um time
int time_id(Time* t)
{
    return t->id;
}

// Função que retorna o nome de um time
char* time_nome(Time* t)
{
    return t->nome;
}

// Função que retorna o número de vitórias de um time
int time_vitorias(Time* t)
{
    return t->vitorias;
}

// Função que retorna o número de empates de um time
int time_empates(Time* t)
{
    return t->empates;
}

// Função que retorna o número de derrotas de um time
int time_derrotas(Time* t)
{
    return t->derrotas;
}

// Função que retorna o número de gols marcados por um time
int time_gols_marcados(Time* t)
{
    return t->gols_marcados;
}

// Função que retorna o número de gols sofridos por um time
int time_gols_sofridos(Time* t){
    return t->gols_sofridos;
}

// Função que retorna o número de partidas disputadas por um time
int time_partidas(Time* t){
    int partidas = 0;
    partidas = t->vitorias + t->empates + t->derrotas;
    return partidas;
}

// Função que retorna o saldo de gols de um time
int time_saldo_gols(Time* t){
    int saldo_gols = 0;
    saldo_gols = t->gols_marcados - t->gols_sofridos;
    return saldo_gols;
}

// Função que calcula o número de pontos de um time
int time_pontos(Time* t){
    int pontos = 0;
    pontos = (3*t->vitorias) + (t->empates);
    return pontos;
}

// Função para registrar o resultado de uma partida
void time_registrar_resultado(Time* t, int gols_feitos, int gols_tomados) {
    if (t == NULL){
        return;
    }

    t->gols_marcados += gols_feitos;
    t->gols_sofridos += gols_tomados;

    if (gols_feitos > gols_tomados) {
        t->vitorias++;
    }
    else if (gols_feitos == gols_tomados) {
        t->empates++;
    }
    else {
        t->derrotas++;
    }
}