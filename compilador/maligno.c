void compila_programa(){
    Token token = analex();
    if (token != PROGRAMA){
        printf("Esperava-se a palavra PROGRAM");
        exit(1);
    }
    Token token = analex();
    if (token != IDENTIFICADOR){
        printf("Esperava-se um identificador");
        exit(1);
    }
    Token token = analex();
    if (token != ABRE_PARENTESES){
        printf("Esperava-se um parenteses");
        exit(1);
    }
    while (token!=FECHA_PARENTESES){
        Token token = analex();
        if (token != IDENTIFICADOR){
            printf("Esperava-se um identificador");
            exit(1);
        }
        Token token = analex();
        if (token != VIRGULA && token != FECHA_PARENTESES){
            printf("Esperava-se uma virgula ou um fecha parenteses");
        }
    }
    Token token = analex();
    if (token != PONTO_E_VIRGULA){
        printf("Esperava-se um ponto e virgula");
        exit(1);
    }
    compila_bloco();
    Token token = analex();
    if (token != PONTO){
        printf("Esperava-se a um ponto final");
        exit(1);
    }
    compila_bloco();
    Token token = analex();
    if (token != FIM_DE_ARQUIVO){
        printf("Esperava-se um fim de arquivo");
        exit(1);
    }
    printf("Progama sintaticamente correto");
}