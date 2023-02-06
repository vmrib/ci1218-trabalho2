#include "escalonamento.h"
#include <stdio.h>
#include <stdlib.h>

static bool adicionarPorConflito(Escalonamento *escalonamento, Transacao *transacao);
static bool checarArestaSerialidade(Transacao *t1, Transacao *t2);

Escalonamento *criarEscalonamento()
{
    Escalonamento *escalonamento = (Escalonamento *)malloc(sizeof(Escalonamento));

    escalonamento->grafoConflito = criarGrafo();
    escalonamento->grafoVisao = criarGrafo();
    escalonamento->transacoes = NULL;
    escalonamento->serializavelConflito = false;
    escalonamento->serializavelVisao = false;

    return escalonamento;
}

// Tenta adicionar uma transação ao escalonamento.
// Retorna true se a transação foi adicionada com sucesso, false caso contrário.
bool adicionarTransacao(Escalonamento *escalonamento, Transacao *transacao)
{
    escalonamento->serializavelConflito = adicionarPorConflito(escalonamento, transacao);

    return escalonamento->serializavelConflito || escalonamento->serializavelVisao;
}

void imprimirEscalonamento(Escalonamento *escalonamento)
{
    for (int i = 0; i < escalonamento->grafoConflito->v; i++)
    {
        printf("%d ", escalonamento->transacoes[i]->id);
    }

    if (escalonamento->serializavelConflito)
        printf("SS");
    else
        printf("NS");

    printf(" ");

    if (escalonamento->serializavelVisao)
        printf("SV");
    else
        printf("NV");

    printf("\n");
}

void destruirEscalonamento(Escalonamento *escalonamento)
{
    destruirGrafo(escalonamento->grafoConflito);
    destruirGrafo(escalonamento->grafoVisao);
    free(escalonamento->transacoes);
    free(escalonamento);
}

static bool checarArestaSerialidade(Transacao *t1, Transacao *t2)
{
    for (int i = 0; i < t1->tamanho; i++)
    {
        for (int j = 0; j < t2->tamanho; j++)
        {
            if (t1->listaOperacoes[i].tempoChegada < t2->listaOperacoes[j].tempoChegada &&
                t1->listaOperacoes[i].atributo == t2->listaOperacoes[j].atributo)
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

static bool adicionarPorConflito(Escalonamento *escalonamento, Transacao *transacao)
{
    inserirVertice(escalonamento->grafoConflito);
    escalonamento->transacoes = realloc(escalonamento->transacoes, escalonamento->grafoConflito->v * sizeof(Transacao *));
    escalonamento->transacoes[escalonamento->grafoConflito->v - 1] = transacao;

    for (int i = 0; i < escalonamento->grafoConflito->v; i++)
    {
        for (int j = 0; j < escalonamento->grafoConflito->v; j++)
        {
            if (i == j)
                continue;

            if (checarArestaSerialidade(escalonamento->transacoes[i], escalonamento->transacoes[j]))
            {
                inserirAresta(escalonamento->grafoConflito, i, j);
            }
        }
    }

    if (checarCicloGrafo(escalonamento->grafoConflito))
    {
        removerVertice(escalonamento->grafoConflito, escalonamento->grafoConflito->v - 1);
        escalonamento->transacoes = realloc(escalonamento->transacoes, escalonamento->grafoConflito->v * sizeof(Transacao *));
        return false;
    }

    return true;
}