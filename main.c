// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bdtimes.h"
#include "bdpartidas.h"

// Pesquisando o time por meio do prefixo
void menu_consultar_time(BDTimes* bd_t) {
    char prefixo[50];
    printf("\nDigite o nome ou prefixo do time: ");
    scanf(" %49s", prefixo);
    bdtimes_buscar_prefixo(bd_t, prefixo);
}

// Menu para consultar partidas (aberto pela opção inicial 3)
void menu_consultar_partidas(BDPartidas* bd_p, BDTimes* bd_t) {
    int modo;
    printf("\nEscolha o modo de consulta:\n");
    printf("1 - Por time mandante\n");
    printf("2 - Por time visitante\n");
    printf("3 - Por time mandante ou visitante\n"); 
    printf("4 - Retornar ao menu principal\n");
    printf("Opção: ");
    scanf(" %d", &modo);

    if (modo == 4) {
        return;
    }

    if (modo < 1 || modo > 3) {
        printf("Opcão inválida.\n");
        return;
    }

    char prefixo[50];
    printf("\nDigite o nome: ");
    scanf(" %49s", prefixo);
    bdpartidas_listar_por_time(bd_p, bd_t, modo, prefixo);
}

int main() {
    // Carregando times
    BDTimes* bd_t = bdtimes_criar();
    if (bdtimes_carregar_csv(bd_t, "times.csv") == 0) {
        printf("Erro ao carregar times.csv\n");
        bdtimes_free(bd_t); //Desalocando memoria
        return 1;
    }

    // Carregando partidas
    BDPartidas* bd_p = bdpartidas_criar();
    if (bdpartidas_carregar_csv(bd_p, "bd_partidas.csv") == 0) {
        printf("Erro ao carregar bd_partidas.csv\n");

        // Liberando memoria alocada
        bdpartidas_free(bd_p);
        bdtimes_free(bd_t);
        return 1;
    }

    // Processando resultados e atualizando estatísticas dos times
    bdpartidas_processar_resultados(bd_p, bd_t);

    // Loop do menu principal
    char opcao = ' ';
    while (opcao != 'Q' && opcao != 'q') {
        printf("\n=== Sistema de Gerenciamento de Partidas ===\n");
        printf("1 - Consultar time\n");
        printf("2 - Consultar partidas\n");
        printf("3 - Atualizar partida (desabilitado)\n");
        printf("4 - Remover partida (desabilitado)\n");
        printf("5 - Inserir partida (desabilitado)\n");
        printf("6 - Imprimir tabela de classificação\n");
        printf("Q - Sair\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        // Execução da opcao escolhida
        if (opcao == '1') {
            menu_consultar_time(bd_t);
        } else if (opcao == '2') {
            menu_consultar_partidas(bd_p, bd_t);
        } else if (opcao == '6') {
            printf("\nImprimindo classificação...\n");
            bdtimes_imprimir_tabela(bd_t);
        } else if (opcao != 'Q' && opcao != 'q') {
            printf("Opção inválida ou desabilitada.\n");
        }
    }

    printf("Encerrando o sistema.\n");

    // Liberando memória
    bdpartidas_free(bd_p);
    bdtimes_free(bd_t);

    return 0;
}