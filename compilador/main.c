#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "analex.h"

#define MAX_LINHAS 100
#define MAX_COMPRIMENTO 256
#define NUM_PALAVRAS 36

token tokens[NUM_PALAVRAS + 3] = {
    programa,rotulo,tipo,variavel,procedimento,
    funcao,inicio,fim,atribuicao,se,entao,senao,
    enquanto,faca,vapara,mais,menos,vezes,dividir,
    igual,diferente,menor,menorouigual,maior,maiorouigual,
    e,ou,nao,abreparenteses,fechaparenteses,fechacolchetes,
    abrecolchetes,virgula,pontoevirgula,doispontos,ponto,numero,identificador,fimdearquivo
};

char *palavras[NUM_PALAVRAS] = {
    "program","label","type","var",
    "procedure","function","begin","end",
    ":=","if","then","else","while","do",
    "goto","+","-","*","div","=","<>","<",
    "<=",">",">=","and","or","not","(",")",
    "]","[",",",";",":","."
};

int main() {
    Analisador an;
    an.pos = 0;
    an.linha[0] = '\0';
    an.palavraAtual[0] = '\0';

    an.arquivo = fopen("exemplo.txt", "r");
    if (an.arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    // Carrega a primeira linha antes de começar
    fgets(an.linha, MAX_COMPRIMENTO, an.arquivo);

    // Dispara o analisador sintático
    compila_programa(&an);

    fclose(an.arquivo);
    return 0;
}

// linha é a linha inteira
// pos é o índice do caractere atual da linha
// buf é o buffer onde o token será excrito
// as setinhas são ponteiros usados para acessar cada membro da struct já que ela foi passada como ponteiro no parâmetro

char *separaToken(Analisador *an) {
    while (an->linha[an->pos] && isspace((unsigned char)an->linha[an->pos]))    // pula espaços até acahr qualquer caractere
        an->pos++;

    if (!an->linha[an->pos])                                                    // se não tiver caracteres e só tiver espaços, retorna null
        return NULL;

    int i = 0;  // índice de escrita no buf
    char c = an->linha[an->pos];    // caractere atual

    if (isdigit((unsigned char)c)) {                        // se deixar só c por algum motivo não reconhece os números
        while (isdigit((unsigned char)an->linha[an->pos]))
            an->buf[i++] = an->linha[an->pos++];            // pega todos números um do lado do outro
    }
    else if (isalpha((unsigned char)c) || c == '_') {       // se começa com letra ou com underline lê até não ser nenhum dos dois ou algum número
        while (isalnum((unsigned char)an->linha[an->pos]) || an->linha[an->pos] == '_')
            an->buf[i++] = an->linha[an->pos++];
    }
    else if (                                           // verifica se é literalmente qualquer operador lógico de dois caracteres em c (pelo menso eu acho que coloquei todos)
        (c == ':' && an->linha[an->pos+1] == '=') ||
        (c == '<' && an->linha[an->pos+1] == '>') ||
        (c == '<' && an->linha[an->pos+1] == '=') ||
        (c == '>' && an->linha[an->pos+1] == '=') ||
        (c == '=' && an->linha[an->pos+1] == '=') ||
        (c == '!' && an->linha[an->pos+1] == '=') ||
        (c == '&' && an->linha[an->pos+1] == '&') ||
        (c == '|' && an->linha[an->pos+1] == '|') ||
        (c == '+' && an->linha[an->pos+1] == '+') ||
        (c == '-' && an->linha[an->pos+1] == '-') ||
        (c == '+' && an->linha[an->pos+1] == '=') ||
        (c == '-' && an->linha[an->pos+1] == '=') ||
        (c == '*' && an->linha[an->pos+1] == '=') ||
        (c == '/' && an->linha[an->pos+1] == '=') ||
        (c == '%' && an->linha[an->pos+1] == '=') ||
        (c == '<' && an->linha[an->pos+1] == '<') ||
        (c == '>' && an->linha[an->pos+1] == '>') ||
        (c == '-' && an->linha[an->pos+1] == '>')
    ) {
        an->buf[i++] = an->linha[an->pos++];                // se for avaça duas posições
        an->buf[i++] = an->linha[an->pos++];
    }
    else {                                                  // só sobra token de 1 caractere
        an->buf[i++] = an->linha[an->pos++];
    }

    an->buf[i] = '\0';                                      // fecha a string para não dar problema depois 
    return an->buf;                                         // retorna só uma "palavra"
}

token proximoToken(Analisador *an) {
    char *palavra;

    while ((palavra = separaToken(an)) == NULL) {
        if (fgets(an->linha, MAX_COMPRIMENTO, an->arquivo) == NULL)
            return fimdearquivo;
        an->pos = 0;
    }

    strcpy(an->palavraAtual, palavra);
    return analex(palavra);
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


void compila_programa(Analisador *an) {
    token t = proximoToken(an);
    if (t != programa) {
        printf("Esperava-se a palavra PROGRAM\n");
        exit(1);
    }

    t = proximoToken(an);
    if (t != identificador) {
        printf("Esperava-se um identificador após PROGRAM\n");
        exit(1);
    }

    t = proximoToken(an);
    if (t != abreparenteses) {
        printf("Esperava-se '('\n");
        exit(1);
    }

    do {
        t = proximoToken(an);
        if (t != identificador) {
            printf("Esperava-se um identificador\n");
            exit(1);
        }
        t = proximoToken(an);
        if (t != virgula && t != fechaparenteses) {
            printf("Esperava-se ',' ou ')'\n");
            exit(1);
        }
    } while (t == virgula);

    t = proximoToken(an);
    if (t != pontoevirgula) {
        printf("Esperava-se ';'\n");
        exit(1);
    }

    compila_bloco(an);

    t = proximoToken(an);
    if (t != ponto) {
        printf("Esperava-se '.'\n");
        exit(1);
    }

    // Verifica fim de arquivo após o ponto final
    t = proximoToken(an);
    if (t != fimdearquivo) {
        printf("Esperava-se fim de arquivo após '.'\n");
        exit(1);
    }

    printf("Programa sintaticamente correto!\n");
}
void compila_bloco(Analisador *an) {
    token t = proximoToken(an);
    if (t == rotulo){
        t = proximoToken(an);
        if(t != numero){
            printf("Esperava-se um número após 'label'!\n");
            exit(1);
        }
        t = proximoToken(an);
        while (t == virgula){
            t = proximoToken(an);
            if (t != numero){
                printf("Esperava-se um número após ','!\n");
                exit(1);
            }
            t = proximoToken(an);
        }

        if(t != pontoevirgula){
            printf("Esperava-se um ';'!\n");
            exit(1);
        }
        t = proximoToken(an);
    }

    if (t == tipo){
        do{
            t = proximoToken(an);
    
            if ( t != identificador){
                printf("Esperava-se um identificador após 'type'!\n");
                exit(1);
            }
            t = proximoToken(an);
            if ( t != igual){
                printf("Esperava-se um '=' após um identificador!\n");
                exit(1);
            }
            t = proximoToken(an);
            if ( t != identificador){
                printf("Esperava-se um tipo!\n");
                exit(1);
            }
            t = proximoToken(an);
            if (t != pontoevirgula) {
                printf("Esperava-se ';'\n");
                exit(1);
            }
            t = proximoToken(an);
        }while(t == identificador);
    }

    if (t == variavel){
        do{
            t = proximoToken(an);
            if(t != identificador){
                printf("Esperava-se um identificador após 'var'!\n");
                exit(1);
            }
            t = proximoToken(an);
            while (t == virgula){
                t = proximoToken(an);
                if (t != identificador){
                    printf("Esperava-se um identificador após ','!\n");
                    exit(1);
                }
                t = proximoToken(an);
            }
            if(t != doispontos){
                printf("Esperava-se ':'!\n");
                exit(1);
            }
            t = proximoToken(an);
            if ( t != identificador){
                printf("Esperava-se um tipo!\n");
                exit(1);
            }
            t = proximoToken(an);
            if (t != pontoevirgula) {
                printf("Esperava-se ';'\n");
                exit(1);
            }
            t = proximoToken(an);

        }while(t == identificador);
    }

    while (t == procedimento || t == funcao) {
        if (t == procedimento){
            t = proximoToken(an);
            if(t != identificador){
                printf("Esperava-se um identificador após 'procedure'!\n");
                exit(1);
            }
            t = proximoToken(an);
            if(t == abreparenteses){
                compila_parametros_formais(an);
                t = proximoToken(an);
            }
            if (t != pontoevirgula) {
                printf("Esperava-se ';' após 'procedure'!\n");
                exit(1);
            }
            compila_bloco(an);
            t = proximoToken(an);
            if (t != pontoevirgula) {
                printf("Esperava-se ';' após bloco do 'procedure'!\n");
                exit(1);
            }
            t = proximoToken(an);
        } else {
            t = proximoToken(an);
            if(t != identificador){
                printf("Esperava-se um identificador após 'function'!\n");
                exit(1);
            }
            t = proximoToken(an);
            if(t == abreparenteses){
                compila_parametros_formais(an);
                t = proximoToken(an);
            }
            if (t != doispontos) {
                printf("Esperava-se ':' após 'function'!\n");
                exit(1);
            }
            t = proximoToken(an);
            if(t != identificador){
                printf("Esperava-se um identificador após ':'!\n");
                exit(1);
            }
            t = proximoToken(an);
            if (t != pontoevirgula) {
                printf("Esperava-se ';' após 'function'!\n");
                exit(1);
            }
            compila_bloco(an);
            t = proximoToken(an);
            if (t != pontoevirgula) {
                printf("Esperava-se ';' após bloco do 'function'!\n");
                exit(1);
            }
            t = proximoToken(an);
        }
    }

    if (t != inicio) {
    printf("Esperava-se 'begin'\n");
    exit(1);
    }
    t = proximoToken(an);
    while (t != fim) {
        if (t == fimdearquivo) {
            printf("Esperava-se 'end'\n");
            exit(1);
        }
        compila_comando(an, &t);
        if (t == pontoevirgula) {
            t = proximoToken(an);
        }
    }
}

