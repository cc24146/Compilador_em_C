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
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    char linha[MAX_COMPRIMENTO];
    int numLinha = 0;

    while (fgets(linha, MAX_COMPRIMENTO, file) != NULL) {
    int pos = 0;
    char buf[MAX_COMPRIMENTO];
    char *palavra;

        while ((palavra = proxToken(linha, &pos, buf)) != NULL) {
            token t = analex(palavra);
            if (t == numero) {
                int val = qualNumero(palavra);
            } else if (t == identificador) {
                char *id = qualId(palavra);
            } else {
                // palavra reservada ou símbolo
            }
        }
    }

    fclose(file);
    return 0;
}


// src é a linha inteira
// pos é o índice do caractere atual da linha
// buf é o buffer onde o token será excrito

char *proxToken(char *src, int *pos, char *buf){

    while (src[*pos] && isspace(src[*pos])) // pula espaços
        (*pos)++;
    
    if(!src[*pos])
        return NULL;
    
    int start = *pos;   // guarda onde o token começa 
    int i = 0;          // índice de escrita no buf
    char c = src[*pos]; // caractere atual
    if (isdigit((unsigned char)c)) {
    while (isdigit((unsigned char)src[*pos])) buf[i++] = src[(*pos)++];
    }

    else if (isalpha(c) || c == '_') {
        while (isalnum(src[*pos]) || src[*pos] == '_'){
            buf[i++] = src[(*pos)++];
        }
    }

    else if ((c == ':' && src[*pos+1] == '=') ||
            (c == '<' && src[*pos+1] == '>') ||
            (c == '<' && src[*pos+1] == '=') ||
            (c == '>' && src[*pos+1] == '=')) {
        buf[i++] = src[(*pos)++];  // lê 1º caractere
        buf[i++] = src[(*pos)++];  // lê 2º caractere
    }


    else {
        buf[i++] = src[(*pos)++];
    }

    buf[i] = '\0';
    return buf;
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



