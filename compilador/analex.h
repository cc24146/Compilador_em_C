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
    abrecolchetes,virgula,pontoevirgula,doispontos,ponto,numero,identificador,fimdearquivo
} token;

typedef struct {    // struct para não ter que mandar tudo individualmente por parâmetros toda vez
    FILE *arquivo;
    char linha[MAX_COMPRIMENTO];
    int pos;
    char buf[MAX_COMPRIMENTO];
    char palavraAtual[MAX_COMPRIMENTO]; 
} Analisador;

extern token tokens[];
extern char *palavras[];

token analex(char *palavra);
int qualNumero(char *str);
char *qualId(char *id);
char *separaToken(Analisador *an);
void compila_programa(Analisador *an);  
token proximoToken(Analisador *an);
void compila_bloco(Analisador *an);
void compila_parametros_formais(Analisador *an);
void compila_comando(Analisador *an, token t);
void compila_comando_sem_rotulo(Analisador *an);
void compila_expressao(Analisador *an);
void compila_expressao_simples(Analisador *an);
void compila_termo(Analisador *an);
void compila_fator(Analisador *an);

#endif