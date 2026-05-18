// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bdtimes.h"
#include "bdpartidas.h"

void menu_consultar_time(BDTimes* bd_t) {
    char prefixo[50];
    printf("\nDigite o nome ou prefixo do time: ");
    scanf(" %49s", prefixo);
    bdtimes_buscar_prefixo(bd_t, prefixo);
}

void menu_consultar_partidas(BDPartidas* bd_p, BDTimes* bd_t) {
    int modo;
    printf("\nEscolha o modo de consulta:\n");
    printf("1 - Por time mandante\n");
    printf("2 - Por time visitante\n");
    printf("3 - Por time mandante ou visitante\n");
    printf("4 - Retornar ao menu principal\n");
    scanf(" %d", &modo);

    if (modo == 4) {
        return;
    }

    if (modo < 1 || modo > 3) {
        printf("Opcao invalida.\n");
        return;
    }

    char prefixo[50];
    printf("Digite o nome: ");
    scanf(" %49s", prefixo);
    bdpartidas_listar_por_time(bd_p, bd_t, modo, prefixo);
}

int main() {
    // 1. Carrega os times
    BDTimes* bd_t = bdtimes_criar();
    if (bdtimes_carregar_csv(bd_t, "bd_time.csv") == 0) {
        printf("Erro ao carregar bd_time.csv\n");
        bdtimes_destruir(bd_t);
        return 1;
    }

    // 2. Carrega as partidas
    BDPartidas* bd_p = bdpartidas_criar();
    if (bdpartidas_carregar_csv(bd_p, "bd_partidas.csv") == 0) {
        printf("Erro ao carregar bd_partidas.csv\n");
        bdpartidas_destruir(bd_p);
        bdtimes_destruir(bd_t);
        return 1;
    }

    // 3. Processa os resultados e atualiza as estatísticas dos times
    bdpartidas_processar_resultados(bd_p, bd_t);

    // 4. Loop do menu
    char opcao = ' ';
    while (opcao != 'Q' && opcao != 'q') {
        printf("\n=== Sistema de Gerenciamento de Partidas ===\n");
        printf("1 - Consultar time\n");
        printf("2 - Consultar partidas\n");
        printf("3 - Atualizar partida (desabilitado)\n");
        printf("4 - Remover partida (desabilitado)\n");
        printf("5 - Inserir partida (desabilitado)\n");
        printf("6 - Imprimir tabela de classificacao\n");
        printf("Q - Sair\n");
        printf("Opcao: ");
        scanf(" %c", &opcao);

        if (opcao == '1') {
            menu_consultar_time(bd_t);
        } else if (opcao == '2') {
            menu_consultar_partidas(bd_p, bd_t);
        } else if (opcao == '6') {
            printf("\nImprimindo classificacao...\n");
            bdtimes_imprimir_tabela(bd_t);
        } else if (opcao != 'Q' && opcao != 'q') {
            printf("Opcao invalida ou desabilitada.\n");
        }
    }

    printf("Encerrando o sistema.\n");

    // 5. Libera memória
    bdpartidas_destruir(bd_p);
    bdtimes_destruir(bd_t);

    return 0;
}