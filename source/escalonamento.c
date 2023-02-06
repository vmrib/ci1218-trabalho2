#include "escalonamento.h"
#include <stdio.h>
#include <stdlib.h>

static bool checarArestaSerialidade(Transacao *t1, Transacao *t2);

Escalonamento *criarEscalonamento()
{
    Escalonamento *escalonamento = (Escalonamento *)malloc(sizeof(Escalonamento));

    escalonamento->grafo = criarGrafo();
    escalonamento->transacoes = NULL;

    return escalonamento;
}

// Tenta adicionar uma transação ao escalonamento.
// Retorna true se a transação foi adicionada com sucesso, false caso contrário.
bool adicionarTransacao(Escalonamento *escalonamento, Transacao *transacao)
{
    inserirVertice(escalonamento->grafo);
    escalonamento->transacoes = realloc(escalonamento->transacoes, escalonamento->grafo->v * sizeof(Transacao *));
    escalonamento->transacoes[escalonamento->grafo->v - 1] = transacao;

    for (int i = 0; i < escalonamento->grafo->v; i++)
    {
        for (int j = 0; j < escalonamento->grafo->v; j++)
        {
            if (i == j)
                continue;

            if (checarArestaSerialidade(escalonamento->transacoes[i], escalonamento->transacoes[j]))
            {
                inserirAresta(escalonamento->grafo, i, j);
            }
        }
    }

    if (checarCicloGrafo(escalonamento->grafo))
    {
        removerVertice(escalonamento->grafo, escalonamento->grafo->v - 1);
        escalonamento->transacoes = realloc(escalonamento->transacoes, escalonamento->grafo->v * sizeof(Transacao *));
        return false;
    }

    return false;
}

void imprimirEscalonamento(Escalonamento *escalonamento)
{
    for (int i = 0; i < escalonamento->grafo->v; i++)
    {
        printf("%d ", escalonamento->transacoes[i]->id);
    }
    printf("\n");
}

void destruirEscalonamento(Escalonamento *escalonamento)
{
    destruirGrafo(escalonamento->grafo);
    free(escalonamento->transacoes);
    free(escalonamento);
}

static bool checarArestaSerialidade(Transacao *t1, Transacao *t2)
{
    for (int i = 0; i < t1->tamanho; i++)
    {
        for (int j = 0; j < t2->tamanho; j++)
        {
            if (t1->listaOperacoes[i].tempoChegada < t2->listaOperacoes[j].tempoChegada)
            {
                if ((t1->listaOperacoes[i].operacao == 'W' && t2->listaOperacoes[j].operacao == 'R') ||
                    (t1->listaOperacoes[i].operacao == 'R' && t2->listaOperacoes[j].operacao == 'W') ||
                    (t1->listaOperacoes[i].operacao == 'W' && t2->listaOperacoes[j].operacao == 'W'))
                    return true;
            }
        }
    }

    return false;
}