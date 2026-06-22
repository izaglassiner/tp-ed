// time.h
#ifndef TIME_H
#define TIME_H

// Definindo a estrutura de dados Time
typedef struct{
    int id;            // ID do time
    char nome[50];     // Nome do time
    int vitorias;      // Número de vitórias do time
    int empates;       // Número de empates do time
    int derrotas;      // Número de derrotas do time
    int gols_marcados; // Número de gols marcados do time
    int gols_sofridos; // Número de gols sofridos do time
} Time;

// Funções para manipulação da estrutura de dados Time:

// Função para criar um time
Time* time_criar(int id, char* nome);

// Função para free um time
void time_free(Time* t);

// Função que retorna o ID de um time
int time_id(Time* t);

// Função que retorna o nome de um time
char* time_nome(Time* t);

// Função que retorna o número de vitórias de um time
int time_vitorias(Time* t);

// Função que retorna o número de empates de um time
int time_empates(Time* t);

// Função que retorna o número de derrotas de um time
int time_derrotas(Time* t);

// Função que retorna o número de gols marcados por um time
int time_gols_marcados(Time* t);

// Função que retorna o número de gols sofridos por um time
int time_gols_sofridos(Time* t);

// Função que retorna o número de partidas disputadas por um time
int time_partidas(Time* t);

// Função que retorna o saldo de gols de um time
int time_saldo_gols(Time* t);

// Função que calcula o número de pontos de um time
int time_pontos(Time* t);

// Função para registrar resultado de um time
void time_registrar_resultado(Time* t, int gols_feitos, int gols_tomados);

// Função para resetar estatísticas de uma time
void time_resetar_estatisticas(Time *t);

#endif