#include <stdio.h>
#include "listaEscalonamento.h"
#include "utils.h"
#include "operacao.h"

int main(void)
{
    int tam = 0;
    Operacao *operacao = lerEntrada(&tam);
    ListaEscalonamento *listaEscalonamento = criarListaEscalonamento(operacao, tam);
    imprimirListaEscalonamento(listaEscalonamento);
    return 0;
}