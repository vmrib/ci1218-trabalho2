#pragma once

#include "escalonamento.h"
#include "listaTransacao.h"

typedef struct ListaEscalonamento
{
    Escalonamento **escalonamentos;
    unsigned int tamanho;
} ListaEscalonamento;

ListaEscalonamento *criarListaEscalonamento(ListaTransacao *listaTransacao);
void imprimirListaEscalonamento(ListaEscalonamento *lista);
// void adicionarListaEscalonamento(ListaEscalonamento *lista, Escalonamento *escalonamento);
void destruirListaEscalonamento(ListaEscalonamento *lista);