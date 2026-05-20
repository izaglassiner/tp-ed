// bdpartidas.c
#include "bdpartidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para criar partida
BDPartidas* bdpartidas_criar()
{
    BDPartidas* bd_p = (BDPartidas*)malloc(sizeof(BDPartidas));
    if (bd_p == NULL) {
        printf("Erro ao alocar memória para o banco de partidas.\n");
        return NULL;
    }
    bd_p->quantidade = 0;
    // Inicializa todos os ponteiros do vetor como NULL
    for (int i = 0; i < MAX_PARTIDAS; i++) {
        bd_p->partidas[i] = NULL;
    }
    return bd_p;
}

// Função para remover partida
void bdpartidas_free(BDPartidas* bd_p)
{
    if (bd_p == NULL){
        return;
    }
    // Liberando cada partida individualmente
    for (int i = 0; i < bd_p->quantidade; i++) {
        if (bd_p->partidas[i] != NULL) {
            partida_free(bd_p->partidas[i]);
        }
    }
    // Liberando BDPartidas
    free(bd_p);
}

// Função para leitura de um arquivo bd_partidas.csv
int bdpartidas_carregar_csv(BDPartidas* bd_p, char* caminho)
{
    if (!bd_p || !caminho){
        return 0;
    }

    FILE* arquivo = fopen(caminho, "r");
    if (!arquivo){
        return 0;
    }

    char cabecalho[100];
    fgets(cabecalho, sizeof(cabecalho), arquivo); // Pula o cabeçalho

    int id, t1, t2, g1, g2;
    while (fscanf(arquivo, "%d,%d,%d,%d,%d", &id, &t1, &t2, &g1, &g2) == 5) {
        if (bd_p->quantidade < MAX_PARTIDAS) {
            bd_p->partidas[bd_p->quantidade++] = partida_criar(id, t1, t2, g1, g2);
        }
    }

    fclose(arquivo);
    return 1;
}

// Função para listar partidas por time
void bdpartidas_listar_por_time(BDPartidas* bd_p, BDTimes* bd_t, int modo, char* prefixo)
{
    if (!bd_p || !bd_t || !prefixo) {
        return;
    }

    int encontrou = 0;
    int tam = strlen(prefixo);

    for (int i = 0; i < bd_p->quantidade; i++) {
        Partida* p = bd_p->partidas[i];
        Time* t1 = bdtimes_buscar_id(bd_t, partida_get_id_t1(p));
        Time* t2 = bdtimes_buscar_id(bd_t, partida_get_id_t2(p));

        if (!t1 || !t2) continue;
        char format[40];
        char* n1 = time_nome(t1);
        char* n2 = time_nome(t2);

        // Verifica se os nomes começam com o prefixo
        int m1 = (strncasecmp(n1, prefixo, tam) == 0);
        int m2 = (strncasecmp(n2, prefixo, tam) == 0);

        if ((modo == 1 && m1) || (modo == 2 && m2) || (modo == 3 && (m1 || m2))) {
            if (!encontrou) {
            printf("\n%-4s %-17s %-12s %s\n", "ID", "Time1", "Placar", "Time2");
                encontrou = 1;
            }
            sprintf(format, "%%-4d %%-%ds %%8d x %%-8d %%s\n", COLS + count_special(n1));

            printf(format,
                partida_get_id(p),
                n1,
                partida_get_g1(p),
                partida_get_g2(p),
                n2);
        }
    }

    if (!encontrou) {
        printf("Nenhuma partida encontrada com o termo pesquisado.\n");
    }
}

// Função para processar partidas e armazenar resultados
void bdpartidas_processar_resultados(BDPartidas* bd_p, BDTimes* bd_t) {
    if (!bd_p || !bd_t){
        return;
    }

    for (int i = 0; i < bd_p->quantidade; i++) {
        Partida* p = bd_p->partidas[i];
        Time* t1 = bdtimes_buscar_id(bd_t, partida_get_id_t1(p));
        Time* t2 = bdtimes_buscar_id(bd_t, partida_get_id_t2(p));

        if (t1 == NULL || t2 == NULL) continue;

        int g1 = partida_get_g1(p);
        int g2 = partida_get_g2(p);

        time_registrar_resultado(t1, g1, g2);
        time_registrar_resultado(t2, g2, g1);
    }
}