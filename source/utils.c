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
    int maior_id = 0;
    // É criada lista de operações
    while (scanf("%d %d %c %c", &tempo_chegada, &id_transacao, &op, &atributo) != EOF){
        tam++;
        operacao = realloc(operacao, tam * sizeof(Operacao));
        if(operacao == NULL){
            printf("Erro ao alocar memoria");
            exit(1);
        }
        if(id_transacao > maior_id)
            maior_id = id_transacao;
        operacao[tam-1].tempoChegada = tempo_chegada;
        operacao[tam-1].idTransacao = id_transacao;
        operacao[tam-1].operacao = op;
        operacao[tam-1].atributo = atributo;
    }
    
    Transacao *transacao = criarTrasicao(1);
    Transacao *v = malloc(maior_id * sizeof(transacao));
    int tam_v = maior_id;
    // É criada uma transação para cada posição no vetor
    for(int i = 0;i < tam_v;i++){
        v[i] = *criarTrasicao(i+1);
    }
    // Operações são adicionadas às suas respectivas transações
    for(int i = 0;i < tam;i++){
        adicionarOperacao(&v[operacao[i].idTransacao-1], &operacao[i]);
    }
    // Transações são adicionadas à lista de transações
    for(int i = 0;i < tam_v;i++){
        adicionarListaTransacao(listaTransacao, &v[i]);
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