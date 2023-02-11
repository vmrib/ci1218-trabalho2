#include "escalonamento.h"
#include <stdio.h>
#include <stdlib.h>

#include "operacao.h"
#include "listaOperacao.h"
#include "listaTransacao.h"

// Pilha de arestas utilizada no passo 3 da serialização por visão.
// A pilha é um vetor de pares de arestas. Cada aresta é um par de vértices.
typedef unsigned int PilhaAresta[2][2];

static bool checarSerialidadeConflito(Escalonamento *escalonamento);
static bool checarSerialidadeVisao(Escalonamento *escalonamento);

static bool checarArestaConflito(Transacao *t1, Transacao *t2);

static void inserirArestasWRVisao(Escalonamento *esc, ListaOperacao *operacoes);
static bool inserirArestasWWVisao(Escalonamento *esc, ListaOperacao *operacoes);
static void inserirArestasWRVisaoAtributo(Escalonamento *esc, ListaOperacao *operacoes, char atributo);
static bool inserirArestasWWVisaoAtributo(Escalonamento *esc, ListaOperacao *operacoes, char atributo);

static bool encontrarCombinacaoArestasVisao(Escalonamento *esc, PilhaAresta pilha[], unsigned int topo);

static unsigned int popularVetorAtributos(char vetorAtributos[], ListaOperacao *operacoes);
static ListaOperacao *recriarListaOperacoes(ListaTransacao *transacoes);
static void ordenarListaOperacoes(ListaOperacao *lista);
static unsigned int idTransacaoParaIdGrafoVisao(Escalonamento *escalonamento, unsigned int idTransacao);

Escalonamento *criarEscalonamento()
{
    Escalonamento *escalonamento = (Escalonamento *)malloc(sizeof(Escalonamento));

    escalonamento->grafoConflito = criarGrafo();
    escalonamento->grafoVisao = criarGrafo();
    inserirVertice(escalonamento->grafoVisao); // T0
    inserirVertice(escalonamento->grafoVisao); // Tf

    escalonamento->transacoes = criarListaTransacao();

    return escalonamento;
}

// Tenta adicionar uma transação ao escalonamento.
// Retorna false caso a transacao nao possa entrar no escalonamento atual.
bool adicionarTransacao(Escalonamento *escalonamento, Transacao *transacao)
{
    if (escalonamento->transacoes->tamanho == 0)
    {
        adicionarListaTransacao(escalonamento->transacoes, transacao);
        return true;
    }

    for (int i = 0; i < escalonamento->transacoes->tamanho; i++)
    {
        if (tempoInicio(transacao) < tempoCommit(escalonamento->transacoes->transacao[i]))
        {
            adicionarListaTransacao(escalonamento->transacoes, transacao);
            return true;
        }
    }

    return false;
}

void imprimirEscalonamento(Escalonamento *escalonamento)
{
    for (int i = 0; i < escalonamento->transacoes->tamanho; i++)
    {
        printf("%d ", escalonamento->transacoes->transacao[i]->id);
    }

    if (checarSerialidadeConflito(escalonamento))
        printf("SS");
    else
        printf("NS");

    printf(" ");

    if (checarSerialidadeVisao(escalonamento))
        printf("SV");
    else
        printf("NV");

    printf("\n");
}

void destruirEscalonamento(Escalonamento *escalonamento)
{
    destruirGrafo(escalonamento->grafoConflito);
    destruirGrafo(escalonamento->grafoVisao);
    destruirListaTransacao(escalonamento->transacoes);
    free(escalonamento);
}

static bool checarSerialidadeConflito(Escalonamento *escalonamento)
{
    for (int i = 0; i < escalonamento->transacoes->tamanho; i++)
        inserirVertice(escalonamento->grafoConflito);

    removerTodasArestas(escalonamento->grafoConflito);

    for (int i = 0; i < escalonamento->grafoConflito->v; i++)
    {
        for (int j = 0; j < escalonamento->grafoConflito->v; j++)
        {
            if (i == j)
                continue;

            if (checarArestaConflito(escalonamento->transacoes->transacao[i], escalonamento->transacoes->transacao[j]))
                inserirAresta(escalonamento->grafoConflito, i, j);
        }
    }

    return !checarCicloGrafo(escalonamento->grafoConflito);
}

static bool checarSerialidadeVisao(Escalonamento *escalonamento)
{
    for (int i = 0; i < escalonamento->transacoes->tamanho; i++)
        inserirVertice(escalonamento->grafoVisao);

    removerTodasArestas(escalonamento->grafoVisao);

    ListaOperacao *listaOperacoes = recriarListaOperacoes(escalonamento->transacoes);

    // Possos 1 e 2
    inserirArestasWRVisao(escalonamento, listaOperacoes);

    // Passo 3
    bool serializavel = inserirArestasWWVisao(escalonamento, listaOperacoes);

    destruirListaOperacao(listaOperacoes);

    return serializavel;
}

// Verifica se existe uma aresta de conflito entre duas transações.
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

// Passo 1 e 2
static void inserirArestasWRVisao(Escalonamento *esc, ListaOperacao *operacoes)
{
    char atributos[operacoes->tamanho];
    unsigned int totalAtributos = popularVetorAtributos(atributos, operacoes);

    // cria as arestas para cada atributo
    for (int i = 0; i < totalAtributos; i++)
        inserirArestasWRVisaoAtributo(esc, operacoes, atributos[i]);
}

// Passo 1 e 2 (considerando soh um atributo)
static void inserirArestasWRVisaoAtributo(Escalonamento *esc, ListaOperacao *operacoes, char atributo)
{
    unsigned int Ti = 0;
    for (int i = 0; i < operacoes->tamanho; i++)
    {
        Operacao *operacaoAtual = &operacoes->listaOperacoes[i];
        unsigned int Tj = operacaoAtual->idTransacao;

        if (operacaoAtual->atributo != atributo)
            continue;

        if (operacaoAtual->operacao == 'R' && Ti != Tj)
            inserirAresta(esc->grafoVisao,
                          idTransacaoParaIdGrafoVisao(esc, Ti),
                          idTransacaoParaIdGrafoVisao(esc, Tj));

        else if (operacaoAtual->operacao == 'W')
            Ti = Tj;
    }

    inserirAresta(esc->grafoVisao, idTransacaoParaIdGrafoVisao(esc, Ti), 1);
}

// Passo 3
static bool inserirArestasWWVisao(Escalonamento *esc, ListaOperacao *operacoes)
{
    char atributos[operacoes->tamanho];
    unsigned int totalAtributos = popularVetorAtributos(atributos, operacoes);

    // cria as arestas para cada atributo
    for (int i = 0; i < totalAtributos; i++)
        if (!inserirArestasWWVisaoAtributo(esc, operacoes, atributos[i]))
            return false;

    return true;
}

// Passo 3 (considerando um atributo)
static bool
inserirArestasWWVisaoAtributo(Escalonamento *esc, ListaOperacao *operacoes, char atributo)
{
    PilhaAresta pilha[operacoes->tamanho * 2];
    unsigned int topo = 0;
    unsigned int W1 = 0;
    int k = 0;

    for (int i = k; i < operacoes->tamanho; i++)
    {
        Operacao *operacaoAtual = &operacoes->listaOperacoes[i];
        unsigned int W2 = operacaoAtual->idTransacao;

        if (operacaoAtual->atributo != atributo)
            continue;

        if (operacaoAtual->operacao == 'W')
        {
            for (int j = k; j < i; j++)
            {
                unsigned int Tx = operacoes->listaOperacoes[j].idTransacao;

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

            bool encontrouOutroWrite = false;

            for (int j = i + 1; j < operacoes->tamanho; j++)
            {
                unsigned int Tx = operacoes->listaOperacoes[j].idTransacao;

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
                    encontrouOutroWrite = true;
                    break;
                }
            }

            // se nao encontrou outro write, entao W2 eh o ultimo write
            // considerar a aresta W2 -> Tf
            if (!encontrouOutroWrite)
            {
                pilha[topo][0][0] = W1;
                pilha[topo][0][1] = W2;
                pilha[topo][1][0] = 1;
                pilha[topo][1][1] = W1;
                topo++;
            }

            W1 = W2;
            k = i + 1;
        }
    }

    return encontrarCombinacaoArestasVisao(esc, pilha, topo);
}

static bool encontrarCombinacaoArestasVisao(Escalonamento *esc, PilhaAresta pilha[], unsigned int topo)
{

    if (topo == 0)
        return !checarCicloGrafo(esc->grafoVisao);

    topo--;

    unsigned int T0 = 0;
    unsigned int Tf = 1;

    unsigned int Ti = pilha[topo][0][0];
    unsigned int Tj = pilha[topo][0][1];

    inserirAresta(esc->grafoVisao,
                  idTransacaoParaIdGrafoVisao(esc, Ti),
                  idTransacaoParaIdGrafoVisao(esc, Tj));

    if (Ti == Tf || Tj == T0 || !encontrarCombinacaoArestasVisao(esc, pilha, topo))
    {
        removerAresta(esc->grafoVisao,
                      idTransacaoParaIdGrafoVisao(esc, Ti),
                      idTransacaoParaIdGrafoVisao(esc, Tj));

        Ti = pilha[topo][1][0];
        Tj = pilha[topo][1][1];
        inserirAresta(esc->grafoVisao,
                      idTransacaoParaIdGrafoVisao(esc, Ti),
                      idTransacaoParaIdGrafoVisao(esc, Tj));

        if (Ti == Tf || Tj == T0 || !encontrarCombinacaoArestasVisao(esc, pilha, topo))
        {
            removerAresta(esc->grafoVisao,
                          idTransacaoParaIdGrafoVisao(esc, Ti),
                          idTransacaoParaIdGrafoVisao(esc, Tj));
            return false;
        }
    }

    return true;
}

static unsigned int popularVetorAtributos(char vetorAtributos[], ListaOperacao *operacoes)
{
    unsigned int totalAtributos = 0;

    // agrupa todos os atributos no vetor atributos
    for (int i = 0; i < operacoes->tamanho; i++)
    {
        bool existe = false;

        for (int j = 0; j < totalAtributos; j++)
        {
            if (operacoes->listaOperacoes[i].atributo == vetorAtributos[j] || operacoes->listaOperacoes[i].atributo == '-')
            {
                existe = true;
                break;
            }
        }

        if (!existe)
        {
            vetorAtributos[totalAtributos] = operacoes->listaOperacoes[i].atributo;
            totalAtributos++;
        }
    }

    return totalAtributos;
}

// Retorna uma lista de operacoes ordenada por tempo de chegada, dado uma lista de transacoes
static ListaOperacao *recriarListaOperacoes(ListaTransacao *transacoes)
{
    ListaOperacao *operacoes = criarListaOperacao();

    for (int i = 0; i < transacoes->tamanho; i++)
    {
        for (int j = 0; j < transacoes->transacao[i]->tamanho; j++)
        {
            adicionarListaOperacao(operacoes, &transacoes->transacao[i]->listaOperacoes[j]);
        }
    }

    ordenarListaOperacoes(operacoes);

    return operacoes;
}

static int comparar(const void *a, const void *b)
{
    return ((Operacao *)a)->tempoChegada - ((Operacao *)b)->tempoChegada;
}

static void ordenarListaOperacoes(ListaOperacao *lista)
{
    qsort(lista->listaOperacoes, lista->tamanho, sizeof(Operacao), comparar);
}

static unsigned int idTransacaoParaIdGrafoVisao(Escalonamento *escalonamento, unsigned int idTransacao)
{
    if (idTransacao == 0)
        return idTransacao;

    for (int i = 0; i < escalonamento->transacoes->tamanho; i++)
        if (escalonamento->transacoes->transacao[i]->id == idTransacao)
            return i + 2;

    return 0;
}