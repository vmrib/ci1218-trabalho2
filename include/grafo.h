#pragma once

#include <stdbool.h>

typedef struct grafo
{
    unsigned int v;    // numero de vertices
    unsigned int *adj; // matriz de adjacencia
} Grafo;

Grafo *criarGrafo();

void inserirVertice(Grafo *g);
void removerVertice(Grafo *g, unsigned int v);

void inserirAresta(Grafo *g, unsigned int v1, unsigned int v2);
void removerAresta(Grafo *g, unsigned int v1, unsigned int v2);

void removerTodasArestas(Grafo *g);

bool checarCicloGrafo(Grafo *grafo);

void imprimirGrafo(Grafo *g);

void destruirGrafo(Grafo *g);