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
unsigned int tempoCommit(Transacao *transacao);
unsigned int tempoInicio(Transacao *transacao);
void destruirTransacao(Transacao *transacao);