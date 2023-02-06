#include "grafo.h"
#include <stdlib.h>
#include <stdio.h>

static bool checarCicloGrafoAux(Grafo *grafo, unsigned int v, bool visitados[], bool pilha[]);

Grafo *criarGrafo()
{
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));

    grafo->v = 0;
    grafo->adj = NULL;

    return grafo;
}

void inserirVertice(Grafo *g)
{
    g->adj = realloc(g->adj, (g->v + 1) * (g->v + 1) * sizeof(unsigned int));
    g->v++;
}

void removerVertice(Grafo *g, unsigned int v)
{
    for (int i = 0; i < g->v; i++)
    {
        g->adj[v * g->v + i] = 0;
        g->adj[i * g->v + v] = 0;
    }
}

void inserirAresta(Grafo *g, unsigned int v1, unsigned int v2)
{
    g->adj[v1 * g->v + v2] = 1;
}

void removerAresta(Grafo *g, unsigned int v1, unsigned int v2)
{
    g->adj[v1 * g->v + v2] = 0;
}

void imprimirGrafo(Grafo *g)
{
    for (int i = 0; i < g->v; i++)
    {
        for (int j = 0; j < g->v; j++)
        {
            printf("%d ", g->adj[i * g->v + j]);
        }
        printf("\n");
    }
}

void destruirGrafo(Grafo *g)
{
    free(g->adj);
    free(g);
}

bool checarCicloGrafo(Grafo *grafo)
{
    bool visitados[grafo->v];
    bool pilha[grafo->v];

    for (int i = 0; i < grafo->v; i++)
    {
        visitados[i] = false;
        pilha[i] = false;
    }

    for (int i = 0; i < grafo->v; i++)
    {
        if (checarCicloGrafoAux(grafo, i, visitados, pilha))
            return true;
    }

    return false;
}

static bool checarCicloGrafoAux(Grafo *grafo, unsigned int v, bool visitados[], bool pilha[])
{
    if (pilha[v])
        return true;

    if (visitados[v])
        return false;

    visitados[v] = true;
    pilha[v] = true;

    for (int i = 0; i < grafo->v; i++)
    {
        if (grafo->adj[v * grafo->v + i] && checarCicloGrafoAux(grafo, i, visitados, pilha))
            return true;
    }

    pilha[v] = false;

    return false;
}