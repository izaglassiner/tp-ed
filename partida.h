// partida.h
#ifndef PARTIDA_H
#define PARTIDA_H

// Definindo a estrutura de dados partida
typedef struct{
    int id;        // ID da partida
    int id_time1;  // ID do primeiro time participante
    int id_time2;  // ID do segundo time participante
    int gols_time1; // Quantidade de gols marcada pelo primeiro time
    int gols_time2; // Quantidade de gols marcada pelo segundo time
} Partida;

// Funções para a manipulação da esturura de dados partida:

// Função para criar uma partida
Partida* partida_criar(int id, int id_t1, int id_t2, int g1, int g2);

// Função para remover uma partida
void partida_free(Partida* p);

// Função que retorna o ID de uma partida
int partida_get_id(Partida* p);

// Função que retorna o ID do primeiro time participante de uma partida
int partida_get_id_t1(Partida* p);

// Função que retorna o ID do segundo time participante de uma partida
int partida_get_id_t2(Partida* p);

// Função que retorna o número de gols do primeiro time participante de uma partida
int partida_get_g1(Partida* p);

// Função que retorna o número de gols do segundo time participante de uma partida
int partida_get_g2(Partida* p);

#endif