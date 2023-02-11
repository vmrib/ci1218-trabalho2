#include <stdio.h>
#include "utils.h"
#include "listaOperacao.h"
#include "listaEscalonamento.h"

int main(void)
{
    ListaOperacao *listaOperacoes = lerEntrada();
    ListaEscalonamento *listaEscalonamento = criarListaEscalonamento(listaOperacoes);
    imprimirListaEscalonamento(listaEscalonamento);

    destruirListaOperacao(listaOperacoes);
    destruirListaEscalonamento(listaEscalonamento);

    return 0;
}