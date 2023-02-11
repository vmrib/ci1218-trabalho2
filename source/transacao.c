#include "transacao.h"
#include <stdlib.h>

Transacao *criarTrasicao(unsigned int id)
{
    Transacao *transacao = (Transacao *)malloc(sizeof(Transacao));
    transacao->listaOperacoes = NULL;
    transacao->tamanho = 0;
    transacao->id = id;
    return transacao;
}

void adicionarOperacao(Transacao *transacao, Operacao *operacao)
{
    transacao->listaOperacoes = realloc(transacao->listaOperacoes, (transacao->tamanho + 1) * sizeof(Operacao));
    transacao->listaOperacoes[transacao->tamanho] = *operacao;
    transacao->tamanho++;
}

void destruirTransacao(Transacao *transacao)
{
    free(transacao->listaOperacoes);
    free(transacao);
}

unsigned int tempoCommit(Transacao *transacao)
{
    return transacao->listaOperacoes[transacao->tamanho - 1].tempoChegada;
}

unsigned int tempoInicio(Transacao *transacao)
{
    return transacao->listaOperacoes[0].tempoChegada;
}