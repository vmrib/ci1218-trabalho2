#pragma once

#include <stdbool.h>
#include "transacao.h"
#include "grafo.h"

typedef struct Escalonamento
{
    Grafo *grafoConflito;
    Grafo *grafoVisao;
    Transacao **transacoesConflito;
    Transacao **transacoesVisao;
    bool serializavelConflito;
    bool serializavelVisao;
} Escalonamento;

Escalonamento *criarEscalonamento();

// Tenta adicionar uma transação ao escalonamento.
// Retorna true se a transação foi adicionada com sucesso, false caso contrário.
ListaEscalonamento *criarListaEscalonamento(Operacao *listaOperacao, int tamanho)

    void imprimirEscalonamento(Escalonamento *escalonamento);

void destruirEscalonamento(Escalonamento *escalonamento);