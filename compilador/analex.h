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
    char linha[MAX_COMPRIMENTO];
    int pos;
    char palavraAtual[MAX_COMPRIMENTO]; 
    FILE *arquivo;
    char buf[MAX_COMPRIMENTO];
} estruturaAux;

typedef enum { VARIAVEL, PROCEDIMENTO, FUNCAO, PARAMETRO } Natureza;

typedef struct Simbolo {
    char nome[50];
    char tipo[50];
    Natureza natureza;
    int escopo;
    int endereco;
    struct Simbolo *prox;
} Simbolo;

typedef struct {
    Simbolo *topo;
    int escopoAtual;
} TabelaSimbolos;

extern token tokens[];
extern char *palavras[];

token analex(char *palavra);
int qualNumero(char *str);
char *qualId(char *id);
char *separaToken(estruturaAux *an);
void compila_programa(estruturaAux *an, TabelaSimbolos *tabela);  
token proximoToken(estruturaAux *an);
void compila_bloco(estruturaAux *an, TabelaSimbolos *tabela);
void compila_parametros_formais(estruturaAux *an, TabelaSimbolos *tabela);
void compila_comando(estruturaAux *an, token *t, TabelaSimbolos *tabela);
void compila_comando_sem_rotulo(estruturaAux *an, token *t, TabelaSimbolos *tabela);
void compila_expressao(estruturaAux *an, token *t, TabelaSimbolos *tabela);
void compila_expressao_simples(estruturaAux *an, token *t, TabelaSimbolos *tabela);
void compila_termo(estruturaAux *an, token *t, TabelaSimbolos *tabela);
void compila_fator(estruturaAux *an, token *t, TabelaSimbolos *tabela);

void inicializaTabela(TabelaSimbolos *tabela);
void insereSimbolo(TabelaSimbolos *tabela, char *nome, char *tipo, Natureza natureza, int endereco);
Simbolo *buscaSimbolo(TabelaSimbolos *tabela, char *nome);
void descartaEscopo(TabelaSimbolos *tabela);

#endif