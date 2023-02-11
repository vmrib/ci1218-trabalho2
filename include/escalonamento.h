#pragma once

#include <stdbool.h>
#include "transacao.h"
#include "grafo.h"
#include "listaTransacao.h"

typedef struct Escalonamento
{
    Grafo *grafoConflito;
    Grafo *grafoVisao;
    ListaTransacao *transacoes;

} Escalonamento;

Escalonamento *criarEscalonamento();

bool adicionarTransacao(Escalonamento *escalonamento, Transacao *transacao);

void imprimirEscalonamento(Escalonamento *escalonamento);

void destruirEscalonamento(Escalonamento *escalonamento);