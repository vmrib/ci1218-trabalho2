#pragma once

#include <stdbool.h>
#include "transacao.h"
#include "grafo.h"

typedef struct Escalonamento
{
    Grafo *grafo;
    Transacao **transacoes;
} Escalonamento;

Escalonamento *criarEscalonamento();

// Tenta adicionar uma transação ao escalonamento.
// Retorna true se a transação foi adicionada com sucesso, false caso contrário.
bool adicionarTransacao(Escalonamento *escalonamento, Transacao *transacao);

void imprimirEscalonamento(Escalonamento *escalonamento);

void destruirEscalonamento(Escalonamento *escalonamento);