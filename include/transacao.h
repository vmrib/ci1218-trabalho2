#pragma once
#include "operacao.h"

typedef struct Transacao
{
    Operacao *listaOperacoes;
    unsigned int tamanho;
} Transacao;

Transacao *criarTrasicao();
void adicionarOperacao(Transacao *transacao, Operacao *operacao);
void destruirTransacao(Transacao *transacao);