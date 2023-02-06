#include "listaEscalonamento.h"
#include <stdio.h>
#include <stdlib.h>

ListaEscalonamento *criarListaEscalonamento(ListaTransacao *listaTransacao)
{
    ListaEscalonamento *listaEsc = (ListaEscalonamento *)malloc(sizeof(ListaEscalonamento));
    listaEsc->escalonamentos = (Escalonamento **)malloc(sizeof(Escalonamento *));
    listaEsc->escalonamentos[0] = criarEscalonamento();
    listaEsc->tamanho = 1;

    for (int i = 0; i < listaTransacao->tamanho; i++)
    {
        bool adicionou = false;
        for (int j = 0; j < listaEsc->tamanho; j++)
        {
            if (adicionarTransacao(listaEsc->escalonamentos[j], &listaTransacao->transacao[i]))
            {
                adicionou = true;
                break;
            }
        }

        if (!adicionou)
        {
            listaEsc->escalonamentos = realloc(listaEsc->escalonamentos, (listaEsc->tamanho + 1) * sizeof(Escalonamento *));
            listaEsc->escalonamentos[listaEsc->tamanho] = criarEscalonamento();
            adicionarTransacao(listaEsc->escalonamentos[listaEsc->tamanho], &listaTransacao->transacao[i]);
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
        imprimirEscalonamento(lista->escalonamentos[i]);
    }
}

void destruirListaEscalonamento(ListaEscalonamento *lista)
{
    for (int i = 0; i < lista->tamanho; i++)
    {
        destruirEscalonamento(lista->escalonamentos[i]);
    }

    free(lista->escalonamentos);
    free(lista);
}