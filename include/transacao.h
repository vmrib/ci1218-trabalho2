#pragma once
#include "operacao.h"

typedef struct Transacao
{
    unsigned int id;
    Operacao *listaOperacoes;
    unsigned int tamanho;
} Transacao;

Transacao *criarTrasicao(unsigned int id);
void adicionarOperacao(Transacao *transacao, Operacao *operacao);
void destruirTransacao(Transacao *transacao);