// bdpartidas.c
#include "bdpartidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>  // strncasecmp

// Função para criar partida
BDPartidas* bdpartidas_criar()
{
    BDPartidas* bd_p = (BDPartidas*)malloc(sizeof(BDPartidas));
    if (bd_p == NULL) {
        printf("Erro ao alocar memória para o banco de partidas.\n");
        return NULL;
    }
    bd_p->front = NULL;
    bd_p->rear = NULL;
    bd_p->quant = 0;
    bd_p->id = 0;
    return bd_p;
}

// Desaloca memória da estrutura de dados BDPArtidas
void bdpartidas_free(BDPartidas* bd_p)
{
    if (bd_p == NULL){
        return;
    }
    PartidaNode* p = bd_p->front;
    
    while (p != NULL){
        PartidaNode* aux = p->next;
        partida_free(p->partida);
        free(p);
        p = aux;
    }
    free(bd_p);
}

// Função para inserir nova partida ao fim da lista
// Função interna não apontada na interface
void bdpartidas_inserir_node(BDPartidas* bd_p, Partida* p){
    PartidaNode* nova_partida = (PartidaNode*) malloc(sizeof(PartidaNode));
    nova_partida->partida = p;
    nova_partida->next = NULL;

    if (bd_p->front == NULL){
        bd_p->front = nova_partida;
    } else {
        bd_p->rear->next = nova_partida;
    }
    bd_p->rear = nova_partida;
    bd_p->quant++;

    // Mantendo contador maior que o último ID usado
    if (partida_get_id(p) >= bd_p->id){
        bd_p->id = partida_get_id(p)+1;
    }
}

//Carrega bando de dado a partir de um arquivo csv apontado por um caminho
int bdpartidas_carregar_csv(BDPartidas* bd_p, char* caminho)
{
    if (bd_p == NULL || caminho == NULL){
        return 0;
    }
    // Abrindo o arquivo
    FILE* arquivo = fopen(caminho, "r");
    if (arquivo == NULL){
        return 0;
    }

    char cabecalho[100];
    fgets(cabecalho, sizeof(cabecalho), arquivo); // Pula o cabeçalho

    // Inserindo cada partida do arquivo em bd_p
    int id, t1, t2, g1, g2;
    while (fscanf(arquivo, "%d,%d,%d,%d,%d", &id, &t1, &t2, &g1, &g2) == 5) {
        Partida* p = partida_criar(id, t1, t2, g1, g2);
        if (p != NULL){
            bdpartidas_inserir_node(bd_p, p);
        }
    }

    fclose(arquivo);
    return 1;
}

// Função para buscar partida por ID
Partida* bdpartidas_buscar_id(BDPartidas* bd_p, int id) {
    if (bd_p == NULL) {
        return NULL;
    }
    PartidaNode* p = bd_p->front;
    while (p != NULL) {
        if (partida_get_id(p->partida) == id) {
            return p->partida;
        }
        p = p->next;
    }
    return NULL;
}

// Função para inserir uma partida ao fim da lista
int bdpartidas_inserir(BDPartidas* bd_p, int id_t1, int id_t2, int g1, int g2) {
    if (bd_p == NULL) {
        return -1;
    }
 
    int novo_id = bd_p->id;
    Partida* p = partida_criar(novo_id, id_t1, id_t2, g1, g2);
    if (p == NULL) {
        return -1;
    }
 
    bdpartidas_inserir_node(bd_p, p);
    return novo_id;
}

// // Função para atualizar os dados de uma partida
int bdpartidas_atualizar(BDPartidas* bd_p, int id, int novo_g1, int novo_g2) {
    Partida* p = bdpartidas_buscar_id(bd_p, id);
    if (p == NULL) {
        return 0;
    }
 
    if (novo_g1 >= 0) {
        partida_set_g1(p, novo_g1);
    }
    if (novo_g2 >= 0) {
        partida_set_g2(p, novo_g2);
    }
    return 1;
}

// // Função para remover uma partida
int bdpartidas_remover(BDPartidas* bd_p, int id) {
    if (bd_p == NULL || bd_p->front == NULL) {
        return 0;
    }
 
    PartidaNode* p = bd_p->front;
    PartidaNode* aux = NULL;
 
    while (p != NULL && partida_get_id(p->partida) != id) {
        aux = p;
        p = p->next;
    }
 
    if (p == NULL) {
        return 0; 
    }
 
    if (aux == NULL) {
        bd_p->front = p->next; // removendo o primeiro nó da lista
    } else {
        aux->next = p->next;
    }
 
    // se o nó removido era o último
    if (p == bd_p->rear) {
        bd_p->rear = aux;
    }
 
    partida_free(p->partida);
    free(p);
    bd_p->quant--;
    return 1;
}


// Função para listar partidas por time
void bdpartidas_listar_por_time(BDPartidas* bd_p, BDTimes* bd_t, int modo, char* prefixo)
{
    if (bd_p == NULL || bd_t == NULL || prefixo == NULL) {
        return;
    }

    int encontrou = 0;
    int tam = strlen(prefixo);

    PartidaNode* p = bd_p->front;
    while (p != NULL) {
        Partida* partida = p->partida;
        Time* t1 = bdtimes_buscar_id(bd_t, partida_get_id_t1(partida));
        Time* t2 = bdtimes_buscar_id(bd_t, partida_get_id_t2(partida));
 
        if (t1 && t2) {
            char* n1 = time_nome(t1);
            char* n2 = time_nome(t2);
            int m1 = (strncasecmp(n1, prefixo, tam) == 0);
            int m2 = (strncasecmp(n2, prefixo, tam) == 0);
 
            if ((modo == 1 && m1) || (modo == 2 && m2) || (modo == 3 && (m1 || m2))) {
                if (!encontrou) {
                    printf("\n%-4s %-14s %-14s %-8s %-8s\n",
                           "ID", "Time1", "Time2", "Placar1", "Placar2");
                    encontrou = 1;
                }
                printf("%-4d %-14s %-14s %-8d %-8d\n",
                       partida_get_id(partida), n1, n2,
                       partida_get_g1(partida), partida_get_g2(partida));
            }
        }
        p = p->next;
    }
 
    if (!encontrou) {
        printf("Nenhuma partida encontrada com o termo pesquisado.\n");
    }
}

// Função para processar partidas e armazenar resultados
void bdpartidas_processar_resultados(BDPartidas* bd_p, BDTimes* bd_t) {
    if (bd_p == NULL || bd_t == NULL){
        return;
    }

    PartidaNode* p = bd_p->front;
    while (p != NULL) {
        Partida* partida = p->partida;
        Time* t1 = bdtimes_buscar_id(bd_t, partida_get_id_t1(partida));
        Time* t2 = bdtimes_buscar_id(bd_t, partida_get_id_t2(partida));
 
        if (t1 != NULL && t2 != NULL) {
            int g1 = partida_get_g1(partida);
            int g2 = partida_get_g2(partida);
            time_registrar_resultado(t1, g1, g2);
            time_registrar_resultado(t2, g2, g1);
        }
        p = p->next;
    }
}

// Salva dados das partidas em um arquivo csv apontado por um caminho
int bdpartidas_salvar_csv(BDPartidas* bd_p, char* caminho){
     if (bd_p == NULL || caminho == NULL) {
        return 0;
    }
 
    FILE* arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar arquivo de partidas.\n");
        return 0;
    }
 
    fprintf(arquivo, "ID,Time1,Time2,GolsTime1,GolsTime2\n");
 
    PartidaNode* p = bd_p->front;
    while (p != NULL) {
        Partida* partida = p->partida;
        fprintf(arquivo, "%d,%d,%d,%d,%d\n",
                partida_get_id(partida), partida_get_id_t1(partida), partida_get_id_t2(partida),
                partida_get_g1(partida), partida_get_g2(partida));
        p = p->next;
    }
 
    fclose(arquivo);
    return 1;
}