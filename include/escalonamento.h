#pragma once

#include <stdbool.h>
#include "transacao.h"

typedef struct Escalonamento
{
    unsigned int **grafoTransacoes; // matriz de adjacência
} Escalonamento;

Escalonamento *criarEscalonamento();

// Tenta adicionar uma transação ao escalonamento.
// Retorna true se a transação foi adicionada com sucesso, false caso contrário.
bool adicionarTransacao(Escalonamento *escalonamento, Transacao *transacao);

void destruirEscalonamento(Escalonamento *escalonamento);