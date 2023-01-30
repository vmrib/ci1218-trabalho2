#pragma once

#include "transacao.h"

typedef struct ListaTransacao
{
    Transacao *transacao; // vetor de transacoes
} ListaTransacao;

ListaTransacao *criarListaTransacao();
void adicionarListaTransacao(ListaTransacao *lista, Transacao *transacao);
void destruirListaTransacao(ListaTransacao *lista);