#ifndef ANALEX_H
#define ANALEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINHAS 100
#define MAX_COMPRIMENTO 256
#define NUM_PALAVRAS 36

typedef enum {
    programa,rotulo,tipo,variavel,procedimento,
    funcao,inicio,fim,atribuicao,se,entao,senao,
    enquanto,faca,vapara,mais,menos,vezes,dividir,
    igual,diferente,menor,menorouigual,maior,maiorouigual,
    e,ou,nao,abreparenteses,fechaparenteses,fechacolchetes,
    abrecolchetes,virgula,pontoevirgula,doispontos,ponto,numero,identificador
} token;

extern token tokens[];
extern char *palavras[];

token analex(char *palavra);
int qualNumero(char *str);
char *qualId(char *id);

#endif