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
int menu_consultar_partidas(BDPartidas* bd_p, BDTimes* bd_t) {
    if (bdpartidas_is_empty(bd_p)){
        printf("\nNenhuma partida foi registrada ainda.\n");
        return 0;
    }

    int modo;
    printf("\nEscolha o modo de consulta:\n");
    printf("1 - Por time mandante\n");
    printf("2 - Por time visitante\n");
    printf("3 - Por time mandante ou visitante\n"); 
    printf("4 - Retornar ao menu principal\n");
    printf("Opção: ");
    scanf(" %d", &modo);

    if (modo == 4) {
        return 0;
    }

    if (modo < 1 || modo > 3) {
        printf("Opcão inválida.\n");
        return 0;
    }

    char prefixo[50];
    printf("\nDigite o nome: ");
    scanf(" %49s", prefixo);
    return bdpartidas_listar_por_time(bd_p, bd_t, modo, prefixo);
}

// Reprocessa as estatísticas dos times do zero e salva as partidas
void sincronizar_dados(BDPartidas* bd_p, BDTimes* bd_t) {
    bdtimes_resetar_todos(bd_t);
    bdpartidas_processar_resultados(bd_p, bd_t);
    bdpartidas_salvar_csv(bd_p, "bd_partidas.csv");
}

// Atualiza o placar de uma partida já existente
void menu_atualizar_partida(BDPartidas* bd_p, BDTimes* bd_t) {
    int encontrou = menu_consultar_partidas(bd_p, bd_t);

    if (bdpartidas_is_empty(bd_p) || !encontrou){
        return;
    }

    int id;
    printf("\nDigite o ID do registro a ser atualizado: ");
    scanf(" %d", &id);

    Partida* p = bdpartidas_buscar_id(bd_p, id);
    if (p == NULL) {
        printf("Partida com ID %d não encontrada.\n", id);
        return;
    }

    char texto_g1[10], texto_g2[10];
    printf("\nDigite o novo valor para os campos Placar1 e Placar2\n");
    printf("**(para manter o valor atual de um campo, digite '-')**\n\n");
    printf("Placar1: ");
    scanf(" %9s", texto_g1);
    printf("Placar2: ");
    scanf(" %9s", texto_g2);

    // o caractere "-" significa "manter o valor atual": se o usuario digitar, -1 é guardado como "sentinela"
    int novo_g1 = (strcmp(texto_g1, "-") == 0) ? -1 : atoi(texto_g1);
    int novo_g2 = (strcmp(texto_g2, "-") == 0) ? -1 : atoi(texto_g2);

    Time* t1 = bdtimes_buscar_id(bd_t, partida_get_id_t1(p));
    Time* t2 = bdtimes_buscar_id(bd_t, partida_get_id_t2(p));
    int mostrar_g1 = (novo_g1 >= 0) ? novo_g1 : partida_get_g1(p);
    int mostrar_g2 = (novo_g2 >= 0) ? novo_g2 : partida_get_g2(p);

    printf("\nConfirma os novos valores para o registro abaixo? (S/N)\n");
    printf("======================================================\n");
    printf("%-4s %-14s %-14s %-8s %-8s\n", "ID", "Time1", "Time2", "Placar1", "Placar2");
    printf("%-4d %-14s %-14s %-8d %-8d\n", id, time_nome(t1), time_nome(t2), mostrar_g1, mostrar_g2);
    printf("======================================================\n");

    char confirmacao;
    printf("Resposta: ");
    scanf(" %c", &confirmacao);

    if (confirmacao == 'S' || confirmacao == 's') {
        bdpartidas_atualizar(bd_p, id, novo_g1, novo_g2);
        sincronizar_dados(bd_p, bd_t);
        printf("Registro atualizado com sucesso.\n");
    } else {
        printf("Atualização cancelada.\n");
    }
}

// Remove uma partida existente
void menu_remover_partida(BDPartidas* bd_p, BDTimes* bd_t) {
    int encontrou = menu_consultar_partidas(bd_p, bd_t);

    if (bdpartidas_is_empty(bd_p) || !encontrou) return;

    int id;
    printf("\nDigite o ID do registro a ser removido: ");
    scanf(" %d", &id);

    Partida* p = bdpartidas_buscar_id(bd_p, id);
    if (p == NULL) {
        printf("Partida com ID %d não encontrada.\n", id);
        return;
    }

    Time* t1 = bdtimes_buscar_id(bd_t, partida_get_id_t1(p));
    Time* t2 = bdtimes_buscar_id(bd_t, partida_get_id_t2(p));

    printf("\nTem certeza de que deseja excluir o registro abaixo? (S/N)\n");
    printf("======================================================\n");
    printf("%-4s %-14s %-14s %-8s %-8s\n", "ID", "Time1", "Time2", "Placar1", "Placar2");
    printf("%-4d %-14s %-14s %-8d %-8d\n", id, time_nome(t1), time_nome(t2), partida_get_g1(p), partida_get_g2(p));
    printf("======================================================\n");


    char confirmacao;
    printf("Resposta: ");
    scanf(" %c", &confirmacao);

    if (confirmacao == 'S' || confirmacao == 's') {
        bdpartidas_remover(bd_p, id);
        sincronizar_dados(bd_p, bd_t);
        printf("Registro removido com sucesso.\n");
    } else {
        printf("Remoção cancelada.\n");
    }
}

// Insere uma nova partida
void menu_inserir_partida(BDPartidas* bd_p, BDTimes* bd_t) {
    int id_t1, id_t2, g1, g2;

    // Mostra os times e IDs disponíveis antes de pedir a digitação
    printf("\n--- Times Cadastrados ---\n");
    bdtimes_listar_ids_nomes(bd_t);
    printf("-------------------------\n");

    printf("\nPara inserir um novo registro, digite os valores para\n");
    printf("os campos Time1, Time2, Placar1 e Placar2 (apenas inteiros):\n\n");
    printf("Time1: ");
    scanf(" %d", &id_t1);
    printf("Time2: ");
    scanf(" %d", &id_t2);
    printf("Placar1: ");
    scanf(" %d", &g1);
    printf("Placar2: ");
    scanf(" %d", &g2);

    Time* t1 = bdtimes_buscar_id(bd_t, id_t1);
    Time* t2 = bdtimes_buscar_id(bd_t, id_t2);

    if (t1 == NULL || t2 == NULL) {
        printf("Time1 ou Time2 informado não existe.\n");
        return;
    }

    printf("\nConfirma a inserção do registro abaixo? (S/N)\n");
    printf("======================================================\n");
    printf("%-4s %-14s %-14s %-8s %-8s\n", "ID", "Time1", "Time2", "Placar1", "Placar2");
    printf("%-4d %-14s %-14s %-8d %-8d\n", bdpartidas_last_id(bd_p), time_nome(t1), time_nome(t2), g1, g2);
    printf("======================================================\n");

    char confirmacao;
    printf("Resposta: ");
    scanf(" %c", &confirmacao);

    if (confirmacao == 'S' || confirmacao == 's') {
        bdpartidas_inserir(bd_p, id_t1, id_t2, g1, g2);
        sincronizar_dados(bd_p, bd_t);
        printf("O registro foi inserido com sucesso.\n");
    } else {
        printf("Inserção cancelada.\n");
    }
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
        printf("Erro ao carregar bd_partidas.csv.\n");

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
        printf("\n--------------------------------------------------\n");
        printf(" * Sistema de Gerenciamento de Partidas * \n");
        printf("1 - Consultar time\n");
        printf("2 - Consultar partidas\n");
        printf("3 - Atualizar partida\n");
        printf("4 - Remover partida\n");
        printf("5 - Inserir partida\n");
        printf("6 - Imprimir tabela de classificação\n");
        printf("Q - Sair\n");
        printf("--------------------------------------------------\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        // Execução da opcao escolhida
        if (opcao == '1') {
            menu_consultar_time(bd_t);
        } else if (opcao == '2') {
            menu_consultar_partidas(bd_p, bd_t);
        } else if (opcao == '3') {
            menu_atualizar_partida(bd_p, bd_t);
        } else if (opcao == '4') {
            menu_remover_partida(bd_p, bd_t);
        } else if (opcao == '5') {
            menu_inserir_partida(bd_p, bd_t);
        } else if (opcao == '6') {
            printf("\nImprimindo classificação...\n");
            bdtimes_imprimir_tabela(bd_t);
            bdtimes_salvar_classificacao_csv(bd_t, "bd_classificacao.csv");
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