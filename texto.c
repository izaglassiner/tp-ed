// texto.c
#include "texto.h"

// Função para contar a quantidade de caracteres especiais em uma string
int count_special(const char* str){
    int i = 0, cont = 0;
    while(str[i] != '\0'){
        if((str[i] & 0xC0) == 0x80)
            cont++;
        i++;
    }
    return cont;
}