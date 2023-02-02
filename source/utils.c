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

    Transacao *transacao = criarTrasicao(1);
    Transacao *v = malloc(sizeof(transacao));
    adicionarOperacao(transacao, &operacao[0]);
    v[0] = *transacao;
    int tam_v = 1;
    // Operações são adicionadas às suas respectivas transações
    for(int i = 0;i < tam;i++){
        if((v[i].id == operacao[i].idTransacao)){
            adicionarOperacao(transacao, &operacao[i]);
        }
        else{
            tam_v++;
            v = realloc(v, tam_v * sizeof(Transacao));
            Transacao *novaTransacao = criarTrasicao(operacao[i].idTransacao);
            adicionarOperacao(novaTransacao, &operacao[i]);
            v[tam_v-1] = *novaTransacao;
        }
    }

    // Adiciona transações do vetor à lista
    int trans_anterior = 0;
    for(int i = 0;i < tam_v;i++){
        if(v[i].id != trans_anterior){
            adicionarListaTransacao(listaTransacao, &v[i]);
            trans_anterior = v[i].id;
        }
    }

    for(int i = 0 ; i < tam_v ; i++){
        printf("Transacao %d\n", v[i].id);
        for(int j = 0;j < v[i].tamanho;j++){
            printf("\tTempo de chegada: %d\n", v[i].listaOperacoes[j].tempoChegada);
            printf("\tId da transacao: %d\n", v[i].listaOperacoes[j].idTransacao);
            printf("\tOperacao: %c\n", v[i].listaOperacoes[j].operacao);
            printf("\tAtributo: %c\n", v[i].listaOperacoes[j].atributo);
        }
    }





    return *listaTransacao;


}