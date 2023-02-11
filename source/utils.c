#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "operacao.h"
#include "listaOperacao.h"
#include "listaTransacao.h"

ListaOperacao *lerEntrada()
{
    ListaOperacao *operacao = criarListaOperacao();
    int tempo_chegada, id_transacao;
    char op, atributo;
    int maior_id = 0;
    // É criada lista de operações
    while (scanf("%d %d %c %c", &tempo_chegada, &id_transacao, &op, &atributo) != EOF)
    {
        Operacao operacaoAtual;

        if (id_transacao > maior_id)
            maior_id = id_transacao;

        operacaoAtual.tempoChegada = tempo_chegada;
        operacaoAtual.idTransacao = id_transacao;
        operacaoAtual.operacao = op;
        operacaoAtual.atributo = atributo;

        adicionarListaOperacao(operacao, &operacaoAtual);
    }

    return operacao;
}

ListaTransacao *operacaoParaTransacao(ListaOperacao *operacao)
{
    int maior_id = 0;
    for (int i = 0; i < operacao->tamanho; i++)
        if (operacao->listaOperacoes[i].idTransacao > maior_id)
            maior_id = operacao->listaOperacoes[i].idTransacao;

    ListaTransacao *listaTransacao = criarListaTransacao();
    Transacao **v = malloc(maior_id * sizeof(Transacao *));
    int tam_v = maior_id;
    // É criada uma transação para cada posição no vetor
    for (int i = 0; i < tam_v; i++)
    {
        v[i] = criarTrasicao(i + 1);
    }
    // Operações são adicionadas às suas respectivas transações
    for (int i = 0; i < operacao->tamanho; i++)
    {
        adicionarOperacao(v[operacao->listaOperacoes[i].idTransacao - 1], &operacao->listaOperacoes[i]);
    }
    // Transações são adicionadas à lista de transações
    for (int i = 0; i < tam_v; i++)
    {
        adicionarListaTransacao(listaTransacao, v[i]);
    }

    free(v);

    return listaTransacao;
}