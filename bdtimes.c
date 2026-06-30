// bdtimes.c
#include "bdtimes.h"
#include "texto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Define a estrutura do nó em BDtimes
struct time_node{
    Time* time;
    TimeNode* next;
};

// Define a estrutura de BDtimes
struct times{
    TimeNode* first;
    int quant;
};

// Função para criar uma estrutura BDTime vazia alocando a memoria necessaria
BDTimes* bdtimes_criar()
{
    BDTimes* bd_t = (BDTimes*)malloc(sizeof(BDTimes));
    if (bd_t == NULL){
        printf("Erro ao alocar memória para BDTimes.\n");
        return NULL;
    }
    bd_t->first = NULL;
    bd_t->quant = 0;
    return bd_t;
}

// Função para remover um BD Time (liberação de memoria)
void bdtimes_free(BDTimes* bd_t)
{
    if (bd_t == NULL){
        return;
    }
    
    TimeNode* t = bd_t->first;
    while (t != NULL){
        TimeNode* aux = t->next;
        time_free(t->time);
        free(t);
        t = aux;
    }
    free(bd_t);
}

// Insere time ao inicio da lista
// Função interna, não listada na interface
void bdtimes_inserir_node(BDTimes* bd_t, Time* t){
    TimeNode* novo_time = (TimeNode*)malloc(sizeof(TimeNode));
    novo_time->time = t;
    novo_time->next = bd_t->first;
    bd_t->first = novo_time;
    bd_t->quant++;
}

// Carrega banco de dados a partir de um arquivo csv apontado por um caminho
int bdtimes_carregar_csv(BDTimes* bd_t, char* caminho)
{
    if (bd_t == NULL || caminho == NULL){
        return 0;
    }
    // Abertura do arquivo
    FILE* arquivo = fopen(caminho, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    char linha[100];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char* texto_id = strtok(linha, ",");
        char* nome_time = strtok(NULL, ",\n\r");

        if (texto_id != NULL && nome_time != NULL) {
            int id = atoi(texto_id);
            Time* t = time_criar(id, nome_time);

            if (t != NULL) {
                bdtimes_inserir_node(bd_t, t);
            }
        }
    }
    fclose(arquivo);
    return 1;
}

// Função para buscar o time referente a um ID
Time* bdtimes_buscar_id(BDTimes *bd_t, int id)
{
    if (bd_t == NULL){
        return NULL;
    }
    TimeNode* t = bd_t->first;
    while (t != NULL){
        if (time_id(t->time) == id){
            return t->time;
        }
        t = t->next;
    }
    return NULL; // Retorna NULL se o ID não foi encontrado
}

// Função para resetar os dados de todos os times
void bdtimes_resetar_todos(BDTimes* bd_t){
    if (bd_t == NULL){
        return;
    }
    TimeNode* t = bd_t->first;
    while (t != NULL){
        time_resetar_estatisticas(t->time);
        t = t->next;
    }
}

// Função para ordenar os times de acordo com os critérios de classificação
void bdtimes_ordenar(BDTimes* bd_t, Time** vetor, int* n) {
    int i = 0;
    TimeNode* t = bd_t->first;
    while (t != NULL) {
        vetor[i] = t->time;
        i++;
        t = t->next;
    }
    *n = i;
 
    for (int a = 0; a < *n - 1; a++) {
        for (int b = 0; b < *n - 1 - a; b++) {
            Time* x = vetor[b];
            Time* y = vetor[b + 1];
 
            // x deve vir depois de y se tiver menos pontos
            // em caso de empate, menos vitórias
            // se ainda der empate, menos saldo de gols
            int pontos_iguais = time_pontos(x) == time_pontos(y);
            int vitorias_iguais = time_vitorias(x) == time_vitorias(y);
 
            int x_perde = time_pontos(x) < time_pontos(y)
                || (pontos_iguais && time_vitorias(x) < time_vitorias(y))
                || (pontos_iguais && vitorias_iguais && time_saldo_gols(x) < time_saldo_gols(y));
 
            if (x_perde) {
                vetor[b] = y;
                vetor[b + 1] = x;
            }
        }
    }
}

// Função para imprimir a tabela de classificação ordenada
void bdtimes_imprimir_tabela(BDTimes *bd_t){
    if (bd_t == NULL || bd_t->quant == 0) {
        return;
    }
 
    Time** ordenado = (Time**)malloc(bd_t->quant * sizeof(Time*));
    int n;
    bdtimes_ordenar(bd_t, ordenado, &n);
 
    printf("\n%-4s %-14s %5s %2s %2s %3s %3s %4s %3s\n",
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
 
    for (int i = 0; i < n; i++) {
        Time* t = ordenado[i];
        int largura = 14 + count_special(time_nome(t));
        printf("%-4d %-*s %5d %2d %2d %3d %3d %4d %3d\n",
               time_id(t), largura, time_nome(t),
               time_vitorias(t), time_empates(t), time_derrotas(t),
               time_gols_marcados(t), time_gols_sofridos(t),
               time_saldo_gols(t), time_pontos(t));
    }
 
    free(ordenado);
}

// Função para buscar time por prefixo
void bdtimes_buscar_prefixo(BDTimes* bd_t, char* prefixo) {
    if (bd_t == NULL || prefixo == NULL) {
        return;
    }
 
    int tam = strlen(prefixo);
    int encontrou = 0;
 
    TimeNode* t = bd_t->first;
    while (t != NULL) {
        Time* time = t->time;
        if (strncasecmp(time_nome(time), prefixo, tam) == 0) {
            if (!encontrou) {
                printf("%-4s %-14s %5s %2s %2s %3s %3s %4s %3s\n",
                       "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
                encontrou = 1;
            }
            // largura do nome ajustada
            int largura = 14 + count_special(time_nome(time));
            printf("%-4d %-*s %5d %2d %2d %3d %3d %4d %3d\n",
                   time_id(time), largura, time_nome(time),
                   time_vitorias(time), time_empates(time), time_derrotas(time),
                   time_gols_marcados(time), time_gols_sofridos(time),
                   time_saldo_gols(time), time_pontos(time));
        }
        t = t->next;
    }
 
    if (!encontrou) {
        printf("Nenhum time encontrado com o prefixo '%s'.\n", prefixo);
    }
}

// Salva tabela de classificação ordenada em um arquivo csv
int bdtimes_salvar_classificacao_csv(BDTimes* bd_t, char* caminho) {
    if (bd_t == NULL || caminho == NULL || bd_t->quant == 0) {
        return 0;
    }
 
    FILE* arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar arquivo de classificação.\n");
        return 0;
    }
 
    Time** ordenado = (Time**) malloc(bd_t->quant * sizeof(Time*));
    int n;
    bdtimes_ordenar(bd_t, ordenado, &n);
 
    fprintf(arquivo, "ID,Time,V,E,D,GM,GS,S,PG\n");
    for (int i = 0; i < n; i++) {
        Time* t = ordenado[i];
        fprintf(arquivo, "%d,%s,%d,%d,%d,%d,%d,%d,%d\n",
                time_id(t), time_nome(t),
                time_vitorias(t), time_empates(t), time_derrotas(t),
                time_gols_marcados(t), time_gols_sofridos(t),
                time_saldo_gols(t), time_pontos(t));
    }
 
    free(ordenado);
    fclose(arquivo);
    return 1;
}

// Função para listar apenas os IDs e Nomes de todos os times cadastrados
void bdtimes_listar_ids_nomes(BDTimes* bd_t) {
    if (bd_t == NULL || bd_t->first == NULL) {
        printf("Nenhum time cadastrado.\n");
        return;
    }

    TimeNode* t = bd_t->first;
    printf("ID     Time\n");
    while (t != NULL) {
        printf("%d      %s\n", time_id(t->time), time_nome(t->time));
        t = t->next;
    }
}