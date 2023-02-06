#include <stdio.h>
#include "listaEscalonamento.h"
#include "utils.h"
#include "listaTransacao.h"
#include "escalonamento.h"
#include "operacao.h"
#include "transacao.h"

int main(void)
{
    ListaTransacao listaTransacao = lerEntrada();
    ListaEscalonamento *listaEscalonamento = criarListaEscalonamento(&listaTransacao);
    imprimirListaEscalonamento(listaEscalonamento);
    printf("fim\n");
    return 0;
}