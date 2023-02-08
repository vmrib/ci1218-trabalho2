#pragma once

#include "operacao.h"

typedef struct ListaOperacao
{
    Operacao *listaOperacoes;
    unsigned int tamanho;
} ListaOperacao;

ListaOperacao *criarListaOperacao();
void adicionarListaOperacao(ListaOperacao *lista, Operacao *operacao);
void destruirListaOperacao(ListaOperacao *lista);