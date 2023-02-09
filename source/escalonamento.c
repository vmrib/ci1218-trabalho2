#include "escalonamento.h"
#include <stdio.h>
#include <stdlib.h>

#include "operacao.h"
#include "listaOperacao.h"

static bool adicionarPorConflito(Escalonamento *escalonamento, Transacao *transacao);
static bool adicionarPorVisao(Escalonamento *escalonamento, Transacao *transacao);

static bool checarArestaConflito(Transacao *t1, Transacao *t2);

static ListaOperacao *recriarListaOperacoes(Transacao **transacoes, unsigned int totalTransacoes);
static void inserirArestasWRVisao(Grafo *grafo, ListaOperacao *operacoes);
static bool inserirArestasWWVisao(Grafo *grafo, ListaOperacao *operacoes);
static void inserirArestasWRVisaoAtributo(Grafo *grafo, ListaOperacao *operacoes, char atributo);
static bool inserirArestasWWVisaoAtributo(Grafo *grafo, ListaOperacao *operacoes, char atributo);

static void ordenarListaOperacoes(ListaOperacao *lista);

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
    // escalonamento->serializavelVisao = false;
    escalonamento->serializavelConflito = adicionarPorConflito(escalonamento, transacao);

    return escalonamento->serializavelConflito || escalonamento->serializavelVisao;
}

void imprimirEscalonamento(Escalonamento *escalonamento)
{
    for (int i = 0; i < escalonamento->grafoVisao->v - 2; i++)
    {
        printf("%d ", escalonamento->transacoesVisao[i]->id);
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
    unsigned int totalTransacoes = escalonamento->grafoVisao->v - 2; // ignora T0 e Tf

    escalonamento->transacoesVisao = realloc(escalonamento->transacoesVisao, totalTransacoes * sizeof(Transacao *));
    escalonamento->transacoesVisao[totalTransacoes - 1] = transacao;

    ListaOperacao *operacoes = recriarListaOperacoes(escalonamento->transacoesVisao, totalTransacoes);

    inserirArestasWRVisao(escalonamento->grafoVisao, operacoes);

    bool serializavel = inserirArestasWWVisao(escalonamento->grafoVisao, operacoes);

    destruirListaOperacao(operacoes);

    return serializavel;
}

// sus, remover
static ListaOperacao *recriarListaOperacoes(Transacao **transacoes, unsigned int totalTransacoes)
{
    ListaOperacao *operacoes = criarListaOperacao();

    for (int i = 0; i < totalTransacoes; i++)
    {
        for (int j = 0; j < transacoes[i]->tamanho; j++)
        {
            adicionarListaOperacao(operacoes, &transacoes[i]->listaOperacoes[j]);
        }
    }

    ordenarListaOperacoes(operacoes);

    return operacoes;
}

// Passo 1 e 2
static void inserirArestasWRVisao(Grafo *grafo, ListaOperacao *operacoes)
{
    char atributos[operacoes->tamanho];
    unsigned int totalAtributos = 0;

    // agrupa todos os atributos no vetor atributos
    for (int i = 0; i < operacoes->tamanho; i++)
    {
        bool existe = false;

        for (int j = 0; j < totalAtributos; j++)
        {
            if (operacoes->listaOperacoes[i].atributo == atributos[j] || operacoes->listaOperacoes[i].atributo == '-')
            {
                existe = true;
                break;
            }
        }

        if (!existe)
        {
            atributos[totalAtributos] = operacoes->listaOperacoes[i].atributo;
            totalAtributos++;
        }
    }

    // cria as arestas para cada atributo
    for (int i = 0; i < totalAtributos; i++)
        inserirArestasWRVisaoAtributo(grafo, operacoes, atributos[i]);
}

// Passo 1 e 2 (considerando soh um atributo)
static void inserirArestasWRVisaoAtributo(Grafo *grafo, ListaOperacao *operacoes, char atributo)
{
    unsigned int Ti = 0;
    for (int i = 0; i < operacoes->tamanho; i++)
    {
        Operacao *operacaoAtual = &operacoes->listaOperacoes[i];
        unsigned int Tj = operacaoAtual->idTransacao + 1;

        if (operacaoAtual->atributo != atributo)
            continue;

        if (operacaoAtual->operacao == 'R' && Ti != Tj)
            inserirAresta(grafo, Ti, Tj);

        else if (operacaoAtual->operacao == 'W')
            Ti = Tj;
    }
}

// Passo 3
static bool inserirArestasWWVisao(Grafo *grafo, ListaOperacao *operacoes)
{
    char atributos[operacoes->tamanho];
    unsigned int totalAtributos = 0;

    // agrupa todos os atributos no vetor atributos
    for (int i = 0; i < operacoes->tamanho; i++)
    {
        bool existe = false;

        for (int j = 0; j < totalAtributos; j++)
        {
            if (operacoes->listaOperacoes[i].atributo == atributos[j] || operacoes->listaOperacoes[i].atributo == '-')
            {
                existe = true;
                break;
            }
        }

        if (!existe)
        {
            atributos[totalAtributos] = operacoes->listaOperacoes[i].atributo;
            totalAtributos++;
        }
    }

    // cria as arestas para cada atributo
    for (int i = 0; i < totalAtributos; i++)
        if (!inserirArestasWWVisaoAtributo(grafo, operacoes, atributos[i]))
            return false;

    return true;
}

// Passo 3 (copnsiderando um atributo)
static bool inserirArestasWWVisaoAtributo(Grafo *grafo, ListaOperacao *operacoes, char atributo)
{
    // linha da pilha x qual aresta x qual vertice da aresta
    unsigned int pilha[operacoes->tamanho * 2][2][2];
    unsigned int topo = 0;
    unsigned int W1 = 0;
    int k = 0;

    for (int i = 0; i < operacoes->tamanho; i++)
    {
        Operacao *operacaoAtual = &operacoes->listaOperacoes[i];
        unsigned int W2 = operacaoAtual->idTransacao + 1;

        if (operacaoAtual->atributo != atributo)
            continue;

        if (operacaoAtual->operacao == 'W')
        {
            for (int j = k; j < i; j++)
            {
                unsigned int Tx = operacoes->listaOperacoes[j].idTransacao + 1;

                if (operacoes->listaOperacoes[j].atributo != atributo)
                    continue;

                if (operacoes->listaOperacoes[j].operacao == 'R')
                {
                    if (W1 != Tx && W2 != Tx)
                    {
                        pilha[topo][0][0] = W2;
                        pilha[topo][0][1] = W1;
                        pilha[topo][1][0] = Tx;
                        pilha[topo][1][1] = W2;
                        topo++;
                    }
                }
            }

            for (int j = i; j < operacoes->tamanho; j++)
            {
                unsigned int Tx = operacoes->listaOperacoes[j].idTransacao + 1;

                if (operacoes->listaOperacoes[j].atributo != atributo)
                    continue;

                if (operacoes->listaOperacoes[j].operacao == 'R')
                {
                    if (W1 != Tx && W2 != Tx)
                    {
                        pilha[topo][0][0] = W1;
                        pilha[topo][0][1] = W2;
                        pilha[topo][1][0] = Tx;
                        pilha[topo][1][1] = W1;
                        topo++;
                    }
                }

                else if (operacoes->listaOperacoes[j].operacao == 'W')
                {
                    break;
                }
            }

            W1 = W2;
            k = i;
        }
    }

    return encontrarCombinacaoArestasVisao(grafo, pilha, topo);
}

static int comparar(const void *a, const void *b)
{
    return ((Operacao *)a)->tempoChegada - ((Operacao *)b)->tempoChegada;
}

static void ordenarListaOperacoes(ListaOperacao *lista)
{
    qsort(lista->listaOperacoes, lista->tamanho, sizeof(Operacao), comparar);
}