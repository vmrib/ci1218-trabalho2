#include "utils.h"
#include "operacao.h"
#include <stdio.h>
#include <stdlib.h>

ListaTransacao lerEntrada(){
    ListaTransacao *listaTransacao = criarListaTransacao();
    Operacao *operacao = NULL;
    int tam = 0;
    int tempo_chegada, id_transacao;
    char op, atributo;
    // É criada lista de operações
    while (scanf("%d %d %c %c", &tempo_chegada, &id_transacao, &op, &atributo) != EOF){
        tam++;
        operacao = realloc(operacao, tam * sizeof(Operacao));
        if(operacao == NULL){
            printf("Erro ao alocar memoria");
            exit(1);
        }
        operacao[tam-1].tempoChegada = tempo_chegada;
        operacao[tam-1].idTransacao = id_transacao;
        operacao[tam-1].operacao = op;
        operacao[tam-1].atributo = atributo;
    }
    
    for(int i = 0;i < tam;i++){
        printf("%d %d %c %c\n", operacao[i].tempoChegada, operacao[i].idTransacao, operacao[i].operacao, operacao[i].atributo);
    }





    return *listaTransacao;


}