#include <stdio.h>
#include "utils.h"
#include "listaOperacao.h"
#include "listaEscalonamento.h"

int main(void)
{
    ListaOperacao *operacao = lerEntrada();
    ListaEscalonamento *listaEscalonamento = criarListaEscalonamento(operacao);
    imprimirListaEscalonamento(listaEscalonamento);

    destruirListaOperacao(operacao);
    destruirListaEscalonamento(listaEscalonamento);

    return 0;
}