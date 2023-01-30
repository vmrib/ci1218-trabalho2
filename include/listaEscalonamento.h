#pragma once

#include "Escalonamento.h"
#include "listaTransacao.h"

typedef struct ListaEscalonamento
{
    Escalonamento *escalonamento;
} ListaEscalonamento;

ListaEscalonamento *criarListaEscalonamento(ListaEscalonamento *lista, ListaTransacao *listaTransacao);
void imprimirListaEscalonamento(ListaEscalonamento *lista);
// void adicionarListaEscalonamento(ListaEscalonamento *lista, Escalonamento *escalonamento);
void destruirListaEscalonamento(ListaEscalonamento *lista);