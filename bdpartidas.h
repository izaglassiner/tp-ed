// bdpartidas.h
#include "partida.h"
#include "bdtimes.h"

#ifndef BDPARTIDAS_H
#define BDPARTIDAS_H

// Estruturas de dados BDPartidas e partida_node
typedef struct partida_node PartidaNode;
typedef struct partidas BDPartidas;

// Funções para manipular a estutura de dados BDPartidas:

// Função para criar partida
BDPartidas* bdpartidas_criar();

// Desaloca memória da estrutura de dados BDPArtidas
void bdpartidas_free(BDPartidas* bd_p);

// Carrega banco de dados a partir de um arquivo csv apontado por um caminho
int bdpartidas_carregar_csv(BDPartidas* bd_p, char* caminho);

// Função para listar partidas por time
int bdpartidas_listar_por_time(BDPartidas* bd_p, BDTimes* bd_t, int modo, char* prefixo);

// Função para processar partidas e armazenar resultados
void bdpartidas_processar_resultados(BDPartidas* bd_p, BDTimes* bd_t);

// Função para buscar partida por ID
Partida* bdpartidas_buscar_id(BDPartidas* bd_p, int id);

// Função para inserir uma partida ao fim da lista
int bdpartidas_inserir(BDPartidas* bd_p, int id_t1, int id_t2, int g1, int g2);

// Função que retorna se a lista está vazia ou não
int bdpartidas_is_empty(BDPartidas* bd_p);

// Função para atualizar os dados de uma partida
int bdpartidas_atualizar(BDPartidas* bd_p, int id, int novo_g1, int novo_g2);

// Função para remover uma partida
int bdpartidas_remover(BDPartidas* bd_p, int id);

// Salva dados das partidas em um arquivo csv apontado por um caminho
int bdpartidas_salvar_csv(BDPartidas* bd_p, char* caminho);

// Retorna o inteiro com o last_id em bd_partidas
int bdpartidas_last_id(BDPartidas* bd_p);

#endif