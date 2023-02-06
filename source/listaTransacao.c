#include "listaTransacao.h"
#include <stdlib.h>

ListaTransacao *criarListaTransacao()
{
    ListaTransacao *lista = (ListaTransacao *)malloc(sizeof(ListaTransacao));
    lista->transacao = NULL;
    lista->tamanho = 0;
    return lista;
}

void adicionarListaTransacao(ListaTransacao *lista, Transacao *transacao)
{
    lista->transacao = realloc(lista->transacao, (lista->tamanho + 1) * sizeof(Transacao));
    lista->transacao[lista->tamanho] = *transacao;
    lista->tamanho++;
}

void destruirListaTransacao(ListaTransacao *lista)
{
    for (int i = 0; i < lista->tamanho; i++)
    {
        destruirTransacao(&lista->transacao[i]);
    }

    free(lista->transacao);
    free(lista);
}