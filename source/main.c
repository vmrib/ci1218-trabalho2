#include <stdio.h>
#include "listaEscalonamento.h"
#include "utils.h"
#include "listaTransacao.h"
#include "escalonamento.h"
#include "operacao.h"
#include "transacao.h"

int main(void)
{
    int tam = 0;
    ListaTransacao listaTransacao;
    Operacao* operacao = lerEntrada(&tam);
    // listaTransacao = operacaoParaTransacao(operacao, tam);
    ListaEscalonamento *listaEscalonamento = criarListaEscalonamento(operacao, tam);
    imprimirListaEscalonamento(listaEscalonamento);
    printf("fim\n");
    return 0;
}