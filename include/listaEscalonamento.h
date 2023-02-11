#pragma once

#include "escalonamento.h"
#include "listaTransacao.h"
#include "listaOperacao.h"

typedef struct ListaEscalonamento
{
    Escalonamento **escalonamentos;
    unsigned int tamanho;
} ListaEscalonamento;

ListaEscalonamento *criarListaEscalonamento(ListaOperacao *listaOperacao);
void imprimirListaEscalonamento(ListaEscalonamento *lista);
// void adicionarListaEscalonamento(ListaEscalonamento *lista, Escalonamento *escalonamento);
void destruirListaEscalonamento(ListaEscalonamento *lista);