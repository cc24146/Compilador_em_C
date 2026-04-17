#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "analex.h"

#define MAX_LINHAS 100
#define MAX_COMPRIMENTO 256
#define NUM_PALAVRAS 36

token tokens[NUM_PALAVRAS + 2] = {
    programa,rotulo,tipo,variavel,procedimento,
    funcao,inicio,fim,atribuicao,se,entao,senao,
    enquanto,faca,vapara,mais,menos,vezes,dividir,
    igual,diferente,menor,menorouigual,maior,maiorouigual,
    e,ou,nao,abreparenteses,fechaparenteses,fechacolchetes,
    abrecolchetes,virgula,pontoevirgula,doispontos,ponto,numero,identificador
};

char *palavras[NUM_PALAVRAS] = {
    "program","label","type","var",
    "procedure","function","begin","end",
    ":=","if","then","else","while","do",
    "goto","+","-","*","div","=","<>","<",
    "<=",">",">=","and","or","not","(",")",
    "]","[",",",";",":","."
};

int main(){
    FILE *file;
    file = fopen("exemplo.txt", "r");

    if (file == NULL){
        printf("Erro ao abrir o arquivo");
        return 1;
    }
    char linha[MAX_COMPRIMENTO];

    while (fgets(linha, MAX_COMPRIMENTO, file) != NULL) {
        char *palavra = strtok(linha, " \t\n");

        while (palavra != NULL) {
            token t = analex(palavra);

            if (t == numero) {
                int val = qualNumero(palavra);  // eh numero
            } else if (t == identificador) {    // eh identificador
                char *id = qualId(palavra);
            } else {                            // eh palavra

            }
            palavra = strtok(NULL, " \t\n");
        }
    }

    fclose(file);
    return 0;
}

token analex(char *palavra) {
    for (int i = 0; i < NUM_PALAVRAS; i++) {
        if (strcmp(palavra, palavras[i]) == 0) {
            return tokens[i];
        }
    }
    int eNumero = 1;                                // true
    for (int i = 0; palavra[i] != '\0'; i++) {
        if (!isdigit(palavra[i])) {
            eNumero = 0;                            // false
            break;
        }
    }
    if (eNumero) return numero;
    return identificador;
}

int qualNumero(char *str){
    return atoi(str);
}

char *qualId(char *id) {
    return id;
}



