#include "listaEscalonamento.h"
#include <stdio.h>
#include <stdlib.h>

ListaEscalonamento *criarListaEscalonamento(ListaTransacao *listaTransacao)
{
    ListaEscalonamento *listaEsc = (ListaEscalonamento *)malloc(sizeof(ListaEscalonamento));
    listaEsc->escalonamento = (Escalonamento **)malloc(sizeof(Escalonamento *));
    listaEsc->escalonamento[0] = criarEscalonamento();
    listaEsc->tamanho = 1;

    for (int i = 0; i < listaTransacao->tamanho; i++)
    {
        bool adicionou = false;
        for (int j = 0; j < listaEsc->tamanho; j++)
        {
            if (adicionarTransacao(listaEsc->escalonamento[j], &listaTransacao->transacao[i]))
            {
                adicionou = true;
                break;
            }
        }

        if (!adicionou)
        {
            listaEsc->escalonamento = realloc(listaEsc->escalonamento, (listaEsc->tamanho + 1) * sizeof(Escalonamento *));
            listaEsc->escalonamento[listaEsc->tamanho] = criarEscalonamento();
            adicionarTransacao(listaEsc->escalonamento[listaEsc->tamanho], &listaTransacao->transacao[i]);
            listaEsc->tamanho++;
        }
    }

    return listaEsc;
}

void imprimirListaEscalonamento(ListaEscalonamento *lista)
{
    for (int i = 0; i < lista->tamanho; i++)
    {
        printf("%d ", i);
        imprimirEscalonamento(lista->escalonamento[i]);
    }
}

void destruirListaEscalonamento(ListaEscalonamento *lista)
{
    for (int i = 0; i < lista->tamanho; i++)
    {
        destruirEscalonamento(lista->escalonamento[i]);
    }

    free(lista->escalonamento);
    free(lista);
}