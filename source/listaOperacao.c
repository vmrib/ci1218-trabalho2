#include "listaOperacao.h"
#include <stdlib.h>

ListaOperacao *criarListaOperacao()
{
    ListaOperacao *lista = (ListaOperacao *)malloc(sizeof(ListaOperacao));

    lista->listaOperacoes = NULL;
    lista->tamanho = 0;

    return lista;
}

void adicionarListaOperacao(ListaOperacao *lista, Operacao *operacao)
{
    lista->listaOperacoes = (Operacao *)realloc(lista->listaOperacoes, (lista->tamanho + 1) * sizeof(Operacao));
    lista->listaOperacoes[lista->tamanho] = *operacao;
    lista->tamanho++;
}

void destruirListaOperacao(ListaOperacao *lista)
{
    free(lista->listaOperacoes);
    free(lista);
}