#include "transacao.h"
#include <stdlib.h>

Transacao *criarTrasicao()
{
    Transacao *transacao = (Transacao *)malloc(sizeof(Transacao));
    transacao->listaOperacoes = NULL;
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