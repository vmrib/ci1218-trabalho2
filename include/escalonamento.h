#pragma once

#include <stdbool.h>
#include "transacao.h"

typedef struct Escalonamento
{
    unsigned int *grafoTransacoes; // matriz de adjacência
    unsigned int quantidadeTransacoes;
} Escalonamento;

Escalonamento *criarEscalonamento();

// Tenta adicionar uma transação ao escalonamento.
// Retorna true se a transação foi adicionada com sucesso, false caso contrário.
bool adicionarTransacao(Escalonamento *escalonamento, Transacao *transacao);

void imprimirEscalonamento(Escalonamento *escalonamento);

void destruirEscalonamento(Escalonamento *escalonamento);