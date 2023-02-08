#include "escalonamento.h"
#include <stdio.h>
#include <stdlib.h>

static bool adicionarPorConflito(Escalonamento *escalonamento, Transacao *transacao);
static bool adicionarPorVisao(Escalonamento *escalonamento, Transacao *transacao);

static bool checarArestaConflito(Transacao *t1, Transacao *t2);
static bool checarArestaWRVisao(Transacao *t1, Transacao *t2);

Escalonamento *criarEscalonamento()
{
    Escalonamento *escalonamento = (Escalonamento *)malloc(sizeof(Escalonamento));

    escalonamento->grafoConflito = criarGrafo();

    escalonamento->grafoVisao = criarGrafo();
    inserirVertice(escalonamento->grafoVisao); // T0
    inserirVertice(escalonamento->grafoVisao); // Tf

    escalonamento->transacoesConflito = NULL;
    escalonamento->transacoesVisao = NULL;

    escalonamento->serializavelConflito = false;
    escalonamento->serializavelVisao = false;

    return escalonamento;
}

// Tenta adicionar uma transação ao escalonamento.
// Retorna true se a transação foi adicionada com sucesso, false caso contrário.
bool adicionarTransacao(Escalonamento *escalonamento, Transacao *transacao)
{
    escalonamento->serializavelVisao = adicionarPorVisao(escalonamento, transacao);
    escalonamento->serializavelConflito = adicionarPorConflito(escalonamento, transacao);

    return escalonamento->serializavelConflito || escalonamento->serializavelVisao;
}

void imprimirEscalonamento(Escalonamento *escalonamento)
{
    for (int i = 0; i < escalonamento->grafoConflito->v; i++)
    {
        printf("%d ", escalonamento->transacoesConflito[i]->id);
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
    free(escalonamento->transacoesConflito);
    free(escalonamento);
}

static bool checarArestaConflito(Transacao *t1, Transacao *t2)
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
    escalonamento->transacoesConflito = realloc(escalonamento->transacoesConflito, escalonamento->grafoConflito->v * sizeof(Transacao *));
    escalonamento->transacoesConflito[escalonamento->grafoConflito->v - 1] = transacao;

    for (int i = 0; i < escalonamento->grafoConflito->v; i++)
    {
        for (int j = 0; j < escalonamento->grafoConflito->v; j++)
        {
            if (i == j)
                continue;

            if (checarArestaConflito(escalonamento->transacoesConflito[i], escalonamento->transacoesConflito[j]))
            {
                inserirAresta(escalonamento->grafoConflito, i, j);
            }
        }
    }

    if (checarCicloGrafo(escalonamento->grafoConflito))
    {
        removerVertice(escalonamento->grafoConflito, escalonamento->grafoConflito->v - 1);
        escalonamento->transacoesConflito = realloc(escalonamento->transacoesConflito, escalonamento->grafoConflito->v * sizeof(Transacao *));
        return false;
    }

    return true;
}

static bool adicionarPorVisao(Escalonamento *escalonamento, Transacao *transacao)
{
    inserirVertice(escalonamento->grafoVisao);
    unsigned int totalVertices = escalonamento->grafoVisao->v - 2; // ignora T0 e Tf

    escalonamento->transacoesVisao = realloc(escalonamento->transacoesVisao, totalVertices * sizeof(Transacao *));
    escalonamento->transacoesVisao[totalVertices - 1] = transacao;

    // Passo 2
    for (int i = 0; i < totalVertices; i++)
    {
        for (int j = 0; j < totalVertices; j++)
        {
            if (i == j)
                continue;

            if (checarArestaWRVisao(escalonamento->transacoesVisao[i], escalonamento->transacoesVisao[j]))
            {
                inserirAresta(escalonamento->grafoVisao, i + 2, j + 2);
            }
        }
    }

    // Passo 1
    inserirArestasT0TfVisao(escalonamento->grafoVisao);

    if (checarCicloGrafo(escalonamento->grafoVisao) || inserirArestasWWVisao(escalonamento)) // Passo 3
    {
        removerVertice(escalonamento->grafoVisao, totalVertices - 1);
        escalonamento->transacoesVisao = realloc(escalonamento->transacoesVisao, (totalVertices - 1) * sizeof(Transacao *));
        return false;
    }

    return true;
}

static bool checarArestaWRVisao(Transacao *t1, Transacao *t2)
{
    for (int i = 0; i < t1->tamanho; i++)
    {
        for (int j = 0; j < t2->tamanho; j++)
        {
            if (t1->listaOperacoes[i].tempoChegada < t2->listaOperacoes[j].tempoChegada &&
                t1->listaOperacoes[i].atributo == t2->listaOperacoes[j].atributo)
            {
                if ((t1->listaOperacoes[i].operacao == 'W' && t2->listaOperacoes[j].operacao == 'R'))
                    return true;
            }
        }
    }

    return false;
}