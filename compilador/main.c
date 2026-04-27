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
    "program","label","type","var","procedure",
    "function","begin","end",":=","if","then","else",
    "while","do","goto","+","-","*","div",
    "=","<>","<","<=",">",">=",
    "and","or","not","(",")","]",
    "[",",",";",":","."
};

TabelaSimbolos tabela;

int main() {
    // guarda as informações na struct
    estruturaAux an;
    an.pos = 0;
    an.linha[0] = '\0';
    an.palavraAtual[0] = '\0';

    inicializaTabela(&tabela);
    tabela.escopoAtual = 1;

    an.arquivo = fopen("exemplo.txt", "r");
    if (an.arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    // Carrega a primeira linha antes de começar
    fgets(an.linha, MAX_COMPRIMENTO, an.arquivo);

    // Dispara o analisador sintático
    tabela.escopoAtual++;
    compila_programa(&an, &tabela);
    descartaEscopo(&tabela);

    fclose(an.arquivo);
    return 0;
}

void inicializaTabela(TabelaSimbolos *tabela) {
    tabela->topo = NULL;
    tabela->escopoAtual = 0;
}

void insereSimbolo(TabelaSimbolos *tabela, char *nome, Tipo tipo, Natureza natureza, int endereco) {
    Simbolo *novoSimbolo = (Simbolo *)malloc(sizeof(Simbolo));
    strcpy(novoSimbolo->nome, nome);
    novoSimbolo->tipo = tipo;
    novoSimbolo->natureza = natureza;
    novoSimbolo->escopo = tabela->escopoAtual;
    novoSimbolo->endereco = endereco;
    novoSimbolo->prox = tabela->topo;
    tabela->topo = novoSimbolo;
}

Simbolo *buscaSimbolo(TabelaSimbolos *tabela, char *nome) {
    Simbolo *atual = tabela->topo;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void descartaEscopo(TabelaSimbolos *tabela) {
    Simbolo *atual = tabela->topo;
    while (atual != NULL && atual->escopo == tabela->escopoAtual) {
        Simbolo *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    tabela->topo = atual;
    tabela->escopoAtual--;
}

// linha é a linha inteira
// pos é o índice do caractere atual da linha
// buf é o buffer onde o token será excrito
// as setinhas são ponteiros usados para acessar cada membro da struct já que ela foi passada como ponteiro no parâmetro

char *copiaUmToken(estruturaAux *an) {
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
    else if (isalpha((unsigned char)c)) {       // se começa com letra lê até não ser ou algum número
        while (isalnum((unsigned char)an->linha[an->pos]))
            an->buf[i++] = an->linha[an->pos++];
    }
    // else if (                                           // verifica se é literalmente qualquer operador lógico de dois caracteres em c (pelo menso eu acho que coloquei todos)
    //     (c == ':' && an->linha[an->pos+1] == '=') ||
    //     (c == '<' && an->linha[an->pos+1] == '>') ||
    //     (c == '<' && an->linha[an->pos+1] == '=') ||
    //     (c == '>' && an->linha[an->pos+1] == '=') ||
    //     (c == '=' && an->linha[an->pos+1] == '=') ||
    //     (c == '!' && an->linha[an->pos+1] == '=') ||
    //     (c == '&' && an->linha[an->pos+1] == '&') ||
    //     (c == '|' && an->linha[an->pos+1] == '|') ||
    //     (c == '+' && an->linha[an->pos+1] == '+') ||
    //     (c == '-' && an->linha[an->pos+1] == '-') ||
    //     (c == '+' && an->linha[an->pos+1] == '=') ||
    //     (c == '-' && an->linha[an->pos+1] == '=') ||
    //     (c == '*' && an->linha[an->pos+1] == '=') ||
    //     (c == '/' && an->linha[an->pos+1] == '=') ||
    //     (c == '%' && an->linha[an->pos+1] == '=') ||
    //     (c == '<' && an->linha[an->pos+1] == '<') ||
    //     (c == '>' && an->linha[an->pos+1] == '>') ||
    //     (c == '-' && an->linha[an->pos+1] == '>') 
    // ) {
    //     an->buf[i++] = an->linha[an->pos++];                // se for avaça duas posições
    //     an->buf[i++] = an->linha[an->pos++];
    // }
    else if (
        (c == ':' && an->linha[an->pos+1] == '=') ||
        (c == '<' && an->linha[an->pos+1] == '>') ||
        (c == '<' && an->linha[an->pos+1] == '=') ||
        (c == '>' && an->linha[an->pos+1] == '=')
    ) {
        an->buf[i++] = an->linha[an->pos++];
        an->buf[i++] = an->linha[an->pos++];
    }
    else {                                                  // só sobra token de 1 caractere
        an->buf[i++] = an->linha[an->pos++];
    }
    an->buf[i] = '\0';                                      // fecha a string para não dar problema depois 
    return an->buf;                                         // retorna só uma "palavra"
}

token proximoToken(estruturaAux *an) {
    char *palavra;

    while ((palavra = copiaUmToken(an)) == NULL) { // separa o próximo token do arquivo
        if (fgets(an->linha, MAX_COMPRIMENTO, an->arquivo) == NULL)
            return fimdearquivo;
        an->pos = 0;
    }

    strcpy(an->palavraAtual, palavra); // copia a string do struct para uma variavel temporaria
    return analex(palavra);             // verifica que token que é a string selecionada
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


void compila_programa(estruturaAux *an, TabelaSimbolos *tabela) {
    token t = proximoToken(an);
    if (t != programa) {
        printf("Esperava-se a palavra PROGRAM! erro em compila_programa\n");
        exit(1);
    }

    t = proximoToken(an);
    if (t != identificador) {
        printf("Esperava-se um identificador após PROGRAM! erro em compila_programa\n");
        exit(1);
    }

    t = proximoToken(an);
    if (t != abreparenteses) {
        printf("Esperava-se '(' erro em compila_programa\n");
        exit(1);
    }

    do {
        t = proximoToken(an);
        if (t != identificador) {
            printf("Esperava-se um identificador! erro em compila_programa\n");
            exit(1);
        }
        t = proximoToken(an);
        if (t != virgula && t != fechaparenteses) {
            printf("Esperava-se ',' ou ')' erro em compila_programa\n");
            exit(1);
        }
    } while (t == virgula);

    t = proximoToken(an);
    if (t != pontoevirgula) {
        printf("Esperava-se ';' erro em compila_programa\n");
        exit(1);
    }

    compila_bloco(an, tabela);

    t = proximoToken(an);
    if (t != ponto) {
        printf("Esperava-se '.' erro em compila_programa\n");
        exit(1);
    }

    t = proximoToken(an);
    if (t != fimdearquivo) {
        printf("Esperava-se fim de arquivo após '.' erro em compila_programa\n");
        exit(1);
    }

    printf("Programa sintaticamente correto!\n");
}

void compila_bloco(estruturaAux *an, TabelaSimbolos *tabela) {
    token t = proximoToken(an);
    if (t == rotulo){
        t = proximoToken(an);
        if(t != numero){
            printf("Esperava-se um numero apos 'label'! erro em compila_bloco\n");
            exit(1);
        }
        t = proximoToken(an);
        while (t == virgula){
            t = proximoToken(an);
            if (t != numero){
                printf("Esperava-se um numero apos ','! erro em compila_bloco\n");
                exit(1);
            }
            t = proximoToken(an);
        }

        if(t != pontoevirgula){
            printf("Esperava-se um ';'! erro em compila_bloco\n");
            exit(1);
        }
        t = proximoToken(an);
    }

    if (t == tipo) {
        t = proximoToken(an);

        while (t == identificador) {
            
            t = proximoToken(an);
            if (t != igual) {
                printf("Esperava-se um '=' apos um identificador! erro em compila_bloco\n");
                exit(1);
            }

            t = proximoToken(an);
            if (t != identificador) {
                printf("Esperava-se um tipo! erro em compila_bloco\n");
                exit(1);
            }

            t = proximoToken(an);
            if (t != pontoevirgula) {
                printf("Esperava-se ';' erro em compila_bloco\n");
                exit(1);
            }

            t = proximoToken(an);
        }
    }

    if (t == variavel) {
        t = proximoToken(an);

        while (t == identificador) {

            while (1) {
                t = proximoToken(an);

                if (t == virgula) {
                    t = proximoToken(an);
                    if (t != identificador) {
                        printf("Esperava-se um identificador apos ','! erro em compila_bloco\n");
                        exit(1);
                    }
                } else {
                    break;
                }
            }

            if (t != doispontos) {
                printf("Esperava-se ':'! erro em compila_bloco\n");
                exit(1);
            }

            t = proximoToken(an);
            if (t != identificador) {
                printf("Esperava-se um tipo! erro em compila_bloco\n");
                exit(1);
            }

            t = proximoToken(an);
            if (t != pontoevirgula) {
                printf("Esperava-se ';' erro em compila_bloco\n");
                exit(1);
            }

            t = proximoToken(an);
        }
    }

    while (t == procedimento || t == funcao) {
        if (t == procedimento){
            t = proximoToken(an);
            if(t != identificador){
                printf("Esperava-se um identificador apos 'procedure'! erro em compila_bloco\n");
                exit(1);
            }
            t = proximoToken(an);
            if(t == abreparenteses){
                compila_parametros_formais(an, tabela);
                t = proximoToken(an);
            }
            if (t != pontoevirgula) {
                printf("Esperava-se ';' apos 'procedure'! erro em compila_bloco\n");
                exit(1);
            }
            compila_bloco(an, tabela);
            t = proximoToken(an);
            if (t != pontoevirgula) {
                printf("Esperava-se ';' apos bloco do 'procedure'! erro em compila_bloco\n");
                exit(1);
            }
            t = proximoToken(an);
        } else {
            t = proximoToken(an);
            if(t != identificador){
                printf("Esperava-se um identificador apos 'function'! erro em compila_bloco\n");
                exit(1);
            }
            t = proximoToken(an);
            if(t == abreparenteses){
                compila_parametros_formais(an, tabela);
                t = proximoToken(an);
            }
            if (t != doispontos) {
                printf("Esperava-se ':' apos 'function'! erro em compila_bloco\n");
                exit(1);
            }
            t = proximoToken(an);
            if(t != identificador){
                printf("Esperava-se um identificador apos ':'! erro em compila_bloco\n");
                exit(1);
            }
            t = proximoToken(an);
            if (t != pontoevirgula) {
                printf("Esperava-se ';' apos 'function'! erro em compila_bloco\n");
                exit(1);
            }
            compila_bloco(an, tabela);
            t = proximoToken(an);
            if (t != pontoevirgula) {
                printf("Esperava-se ';' apos bloco do 'function'! erro em compila_bloco\n");
                exit(1);
            }
            t = proximoToken(an);
        }
    }

    if (t != inicio) {
    printf("Esperava-se 'begin' erro em compila_bloco\n");
    exit(1);
    }
    t = proximoToken(an);
    while (t != fim) {
        if (t == fimdearquivo) {
            printf("Esperava-se 'end' erro em compila_bloco\n");
            exit(1);
        }
        compila_comando(an, &t, tabela);
        if (t == pontoevirgula) {
            t = proximoToken(an);
        }else if (t != fim) {
            printf("Esperava-se ';' ou 'end' erro em compila_bloco\n");
            exit(1);
        }
    }
}

void compila_parametros_formais(estruturaAux *an, TabelaSimbolos *tabela){
    token t;
    do {
        t = proximoToken(an);

        if (t == variavel) {
            t = proximoToken(an);
        }

        if (t == identificador) {
            t = proximoToken(an);
            while (t == virgula) {
                t = proximoToken(an);
                if (t != identificador) {
                    printf("Esperava-se um identificador apos ','! erro em compila_parametros_formais\n");
                    exit(1);
                }
                t = proximoToken(an);
            }
            if (t != doispontos) {
                printf("Esperava-se ':' apos identificadores! erro em compila_parametros_formais\n");
                exit(1);
            }
            t = proximoToken(an);
            if (t != identificador) {
                printf("Esperava-se um identificador! erro em compila_parametros_formais\n");
                exit(1);
            }
            t = proximoToken(an);
        }

        else if (t == funcao) {
            t = proximoToken(an);
            if (t != identificador) {
                printf("Esperava-se um identificador apos 'function'! erro em compila_parametros_formais\n");
                exit(1);
            }
            t = proximoToken(an);
            while (t == virgula) {
                t = proximoToken(an);
                if (t != identificador) {
                    printf("Esperava-se um identificador apos ','! erro em compila_parametros_formais\n");
                    exit(1);
                }
                t = proximoToken(an);
            }
            if (t != doispontos) {
                printf("Esperava-se ':' apos identificadores! erro em compila_parametros_formais\n");
                exit(1);
            }
            t = proximoToken(an);
            if (t != identificador) {
                printf("Esperava-se um identificador apos ':'! erro em compila_parametros_formais\n");
                exit(1);
            }
            t = proximoToken(an);
        }

        else if (t == procedimento) {
            t = proximoToken(an);
            if (t != identificador) {
                printf("Esperava-se um identificador apos 'procedure'! erro em compila_parametros_formais\n");
                exit(1);
            }
            t = proximoToken(an);
            while (t == virgula) {
                t = proximoToken(an);
                if (t != identificador) {
                    printf("Esperava-se um identificador apos ','! erro em compila_parametros_formais\n");
                    exit(1);
                }
                t = proximoToken(an);
            }
        }

        else {
            printf("Esperava-se um parametro formal! erro em compila_parametros_formais\n");
            exit(1);
        }

    } while (t == pontoevirgula);

    if (t != fechaparenteses) {
        printf("Esperava-se ')' apos o fim dos parametros! erro em compila_parametros_formais\n");
        exit(1);
    }
}

void compila_comando(estruturaAux *an, token *t, TabelaSimbolos *tabela){
    while (*t == numero){
        *t = proximoToken(an);
        if (*t != doispontos){
            printf("Esperava-se ':' apos numero! erro em compila_comando\n");
            exit(1);
        }
        *t = proximoToken(an);
    }
    compila_comando_sem_rotulo(an, t, tabela);
}

void compila_comando_sem_rotulo(estruturaAux *an, token *t, TabelaSimbolos *tabela){
    if (*t == identificador){
        *t = proximoToken(an);
        if (*t == abrecolchetes){
            *t = proximoToken(an);
            compila_expressao(an, t, tabela);
            while (*t == virgula){
                *t = proximoToken(an);
                compila_expressao(an, t, tabela);
            }
            if (*t != fechacolchetes){
                printf("Esperava-se um ']' apos a expressao! erro em compila_comando_sem_rotulo\n");
                exit(1);
            }
            *t = proximoToken(an);
        }
        if (*t == atribuicao){
            *t = proximoToken(an);
            compila_expressao(an, t, tabela);
        }
        else if (*t == abreparenteses) {
            do {
                *t = proximoToken(an);
                compila_expressao(an, t, tabela);
            } while (*t == virgula);
            if (*t != fechaparenteses) {
                printf("Esperava-se ')'!\n");
                exit(1);
            }
            *t = proximoToken(an);
        }
        else {
            printf("Comando invalido apos identificador! erro em compila_comando_sem_rotulo\n");
            exit(1);
        }
    }
    else if (*t == vapara){
        *t = proximoToken(an);
        if (*t != numero){
            printf("Esperava-se um numero apos 'goto'! erro em compila_comando_sem_rotulo\n");
            exit(1);
        }
        *t = proximoToken(an);
    }
    else if (*t == inicio) {
        *t = proximoToken(an);
        while (*t != fim) {
            if (*t == fimdearquivo) {
                printf("Esperava-se 'end'! erro em compila_comando_sem_rotulo\n");
                exit(1);
            }
            compila_comando(an, t, tabela);
            if (*t == pontoevirgula) {
                *t = proximoToken(an);
            }
        }
        *t = proximoToken(an);  
    }
    else if (*t == se){
        *t = proximoToken(an);
        compila_expressao(an, t, tabela);
        if (*t != entao){
            printf("Esperava-se 'then' apos expressao! erro em compila_comando_sem_rotulo\n");
            exit(1);
        }
        *t = proximoToken(an);
        compila_comando_sem_rotulo(an, t, tabela);
        if (*t == senao){
            *t = proximoToken(an);
            compila_comando_sem_rotulo(an, t, tabela);
        }
    }
    else if (*t == enquanto){
        *t = proximoToken(an);
        compila_expressao(an, t, tabela);
        if (*t != faca){
            printf("Esperava-se 'do' apos expressao! erro em compila_comando_sem_rotulo\n");
            exit(1);
        }
        *t = proximoToken(an);
        compila_comando_sem_rotulo(an, t, tabela);
    }
}

void compila_expressao(estruturaAux *an, token *t, TabelaSimbolos *tabela) {
    compila_expressao_simples(an, t, tabela);

    if (*t == igual    || *t == diferente ||
        *t == menor    || *t == menorouigual ||
        *t == maior    || *t == maiorouigual) {

        *t = proximoToken(an);

        compila_expressao_simples(an, t, tabela);
    }
}

void compila_expressao_simples(estruturaAux *an, token *t, TabelaSimbolos *tabela){
    if (*t == mais || *t == menos){
        *t = proximoToken(an);
    }
    compila_termo(an, t, tabela);
    while (*t == mais || *t == menos || *t == ou){
        *t = proximoToken(an);
        compila_termo(an, t, tabela);
    }
}

void compila_termo(estruturaAux *an, token *t, TabelaSimbolos *tabela){
    compila_fator(an, t, tabela);

    while (*t == vezes || *t == dividir || *t == e){

        *t = proximoToken(an);
        compila_fator(an, t, tabela);
    }
}

void compila_fator(estruturaAux *an, token *t, TabelaSimbolos *tabela){
    if (*t == identificador){
        *t = proximoToken(an);
        if (*t == abrecolchetes){
            *t = proximoToken(an);
            compila_expressao(an, t, tabela);
            while (*t == virgula){
                *t = proximoToken(an);
                compila_expressao(an, t, tabela);
            }
            if (*t != fechacolchetes){
                printf("Esperava-se um ']' apos a expressao! erro em compila_fator\n");
                exit(1);
            }
            *t = proximoToken(an);
        }
        else if (*t == abreparenteses) {
            do {
                *t = proximoToken(an);
                compila_expressao(an, t, tabela);
            } while (*t == virgula);
            if (*t != fechaparenteses) {
                printf("Esperava-se ')'! erro em compila_fator\n");
                exit(1);
            }
            *t = proximoToken(an);
        }
    } else if (*t == abreparenteses){
        *t = proximoToken(an);
        compila_expressao(an, t, tabela);
        if(*t != fechaparenteses){
            printf("Esperava-se ')' apos expressao! erro em compila_fator\n");
            exit(1);
        }
        *t = proximoToken(an);
    } else if (*t == nao){
        *t = proximoToken(an);
        compila_fator(an, t, tabela);
    } else if (*t == numero){       
        *t = proximoToken(an);
    } else {
        printf("Esperava-se um numero! erro em compila_fator\n");
        exit(1);
    }
}
