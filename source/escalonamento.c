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
            if (operacoes->listaOperacoes[i].atributo == atributos[j])
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

    /** DUVIDA IMPORTANTE: CONSIDERAR O WRITE MAIS RECENTE OU TODOS OS WRITES? */
    /** Depois de pensar um pouco cheguei a conclusao que eh o write mais recente.
     *  Se nao for esse algoritmo por visao eh todo cagado por natureza.
     *
     */

    // considerando soh write recente
    unsigned int Ti = 0;
    for (int i = 0; i < operacoes->tamanho; i++)
    {
        Operacao *operacaoAtual = &operacoes->listaOperacoes[i];
        unsigned int Tj = operacaoAtual->idTransacao + 1;

        if (operacaoAtual->atributo != atributo)
            continue;

        if (operacaoAtual->operacao == 'R')
            inserirAresta(grafo, Ti, Tj);

        else if (operacaoAtual->operacao == 'W')
            Ti = Tj;
    }

    // considerando todos os writes
    // enquanto nao houver escrita do atributo, cria aresta T0 -> Tx
    /*
    int i = 0;
    for (; i < operacoes->tamanho; i++)
    {
        Operacao *operacaoAtual = &operacoes->listaOperacoes[i];
        unsigned int Tatual = operacaoAtual->transacao + 1;

        if (operacaoAtual->atributo != atributo)
            continue;

        if (operacaoAtual->operacao == 'R')
            inserirAresta(grafo, 0, Tatual);
        else
            break;
    }

    // Passo 2 algoritmo
    for (; operacoes->tamanho; i++)
    {
        if (operacoes->listaOperacoes[i].atributo != atributo)
            continue;

        if (operacoes->listaOperacoes[i].operacao == 'W')
        {
            for (int j = i + 1; j < operacoes->tamanho; j++)
            {
                if (operacoes->listaOperacoes[j].atributo != atributo)
                    continue;

                if (operacoes->listaOperacoes[j].operacao == 'R')
                {
                    unsigned int Ti = operacoes->listaOperacoes[i].transacao + 1;
                    unsigned int Tj = operacoes->listaOperacoes[j].transacao + 1;
                    inserirAresta(grafo, Ti, Tj);
                }
            }
        }
    }

    // Cria aresta da transacao que fez a ultima escrita para Tf
    for (i = operacoes->tamanho - 1; i >= 0; i--)
    {
        if (operacoes->listaOperacoes[i].atributo != atributo)
            continue;

        if (operacoes->listaOperacoes[i].operacao == 'W')
        {
            unsigned int Ti = operacoes->listaOperacoes[i].transacao + 1;
            inserirAresta(grafo, Ti, 1);
            break;
        }
    } */
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
            if (operacoes->listaOperacoes[i].atributo == atributos[j])
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
    // Considerando que o passo 1 e 2 eh com os writes mais recentes

    // algoritmo ta extremamente lixoso, mas funciona (na teoria) (eu acho)

    // TODO: eliminar o for do meio, e fazer o primeiro for pegar o write mais recente
    // prototipo:
    // 1. encontra primeiro write, depois segundo (no mesmo loop)
    // 2. pra cada read depois faz o passo 3 tlg
    // 3. seta primeiro write = segundo e repete passo 1.

    for (int i = 0; i < operacoes->tamanho; i++)
    {
        Operacao *operacaoAtual = &operacoes->listaOperacoes[i];
        unsigned int Tk = operacaoAtual->idTransacao + 1;

        if (operacaoAtual->atributo != atributo || operacaoAtual->operacao != 'W')
            continue;

        for (int j = i + 1; j < operacoes->tamanho; j++)
        {
            Operacao *operacaoAtual2 = &operacoes->listaOperacoes[j];
            unsigned int Ti = operacaoAtual2->idTransacao + 1;

            if (operacaoAtual2->atributo != atributo || operacaoAtual2->operacao != 'W')
                continue;

            for (int k = j + 1; k < operacoes->tamanho; k++)
            {
                Operacao *operacaoAtual3 = &operacoes->listaOperacoes[k];
                unsigned int Tj = operacaoAtual3->idTransacao + 1;

                if (operacaoAtual3->atributo != atributo)
                    continue;

                if (operacaoAtual3->operacao == 'R')
                {
                    inserirAresta(grafo, Tk, Ti);
                    if (checarCicloGrafo(grafo))
                    {
                        removerAresta(grafo, Tk, Ti);
                        inserirAresta(grafo, Tj, Tk);

                        if (checarCicloGrafo(grafo))
                        {
                            removerAresta(grafo, Tj, Tk);
                            return false;
                        }
                    }
                }

                else if (operacaoAtual3->operacao == 'W')
                    break;
            }
            break;
        }
    }

    return true;
}