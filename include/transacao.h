#pragma once
#include "operacao.h"

typedef struct Transacao
{
    Operacao *listaOperacoes;
} Transacao;

Transacao *criarTrasicao();
void adicionarOperacao(Transacao *transacao, Operacao *operacao);
void destruirTransacao(Transacao *transacao);