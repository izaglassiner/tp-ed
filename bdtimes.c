// bdtimes.c
#include "bdtimes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para criar um BD Time
BDTimes* bdtimes_criar()
{
    BDTimes* bd_t = (BDTimes*) malloc(sizeof(BDTimes));
    if (bd_t == NULL){
        printf("Erro ao alocar memória para BDTimes.");
        return NULL;
    }
    bd_t->quantidade = 0;
    return bd_t;
}

// Função para remover um BD Time
void bdtimes_destruir(BDTimes* bd_t)
{
    if (bd_t == NULL){
        return;
    }
    for (int i = 0; i < bd_t->quantidade; i++){
        if (bd_t->times[i] != NULL){
            time_destruir(bd_t->times[i]);
        }
    }
    free(bd_t);
}

// Função para leitura de um arquivo bd_time.csv
int bdtimes_carregar_csv(BDTimes* bd_t, char* caminho)
{
    if (bd_t == NULL || caminho == NULL){
        return 0;
    }
    FILE* arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.");
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
                bd_t->times[bd_t->quantidade] = t;
                bd_t->quantidade++;
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
    // Percorre o vetor procurando pelo ID correspondente
    for (int i = 0; i < bd_t->quantidade; i++){
        if (time_id(bd_t->times[i]) == id) {
            return bd_t->times[i]; // Retorna o time com o ID procurado
        }
    }
    return NULL; // Retorna NULL se o ID não foi encontrado
}

// Função para imprimir a tabela de times
void bdtimes_imprimir_tabela(BDTimes *bd_t)
{
    if (bd_t == NULL){
        return;
    }
    printf("%-4s %-15s %2s %2s %2s %3s %3s %4s %3s\n",
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    for (int i = 0; i < bd_t->quantidade; i++) {
        Time* t = bd_t->times[i];
        printf("%-4d %-15s %2d %2d %2d %3d %3d %4d %3d\n",
               time_id(t),
               time_nome(t),
               time_vitorias(t),
               time_empates(t),
               time_derrotas(t),
               time_gols_marcados(t),
               time_gols_sofridos(t),
               time_saldo_gols(t),
               time_pontos(t));
    }
}

// Função para buscar time por prefixo
void bdtimes_buscar_prefixo(BDTimes* bd_t, char* prefixo) {
    if (bd_t == NULL || prefixo == NULL) {
        return;
    }
    int tam = strlen(prefixo);
    int encontrou = 0;
    for (int i = 0; i < bd_t->quantidade; i++) {
        Time* t = bd_t->times[i];
        // strncmp compara apenas os primeiros 'tam' caracteres
        if (strncmp(time_nome(t), prefixo, tam) == 0) {
            if (!encontrou) {
                printf("%-4s %-15s %2s %2s %2s %3s %3s %4s %3s\n",
                       "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
                encontrou = 1;
            }
            printf("%-4d %-15s %2d %2d %2d %3d %3d %4d %3d\n",
                   time_id(t),
                   time_nome(t),
                   time_vitorias(t),
                   time_empates(t),
                   time_derrotas(t),
                   time_gols_marcados(t),
                   time_gols_sofridos(t),
                   time_saldo_gols(t),
                   time_pontos(t));
        }
    }
    if (!encontrou) {
        printf("Nenhum time encontrado com o prefixo '%s'.\n", prefixo);
    }
}