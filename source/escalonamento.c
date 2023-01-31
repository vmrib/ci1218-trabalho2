#include "escalonamento.h"
#include <stdio.h>
#include <stdlib.h>

Escalonamento *criarEscalonamento()
{
    Escalonamento *escalonamento = (Escalonamento *)malloc(sizeof(Escalonamento));

    escalonamento->grafoTransacoes = NULL;
    escalonamento->quantidadeTransacoes = 0;

    return escalonamento;
}

// Tenta adicionar uma transação ao escalonamento.
// Retorna true se a transação foi adicionada com sucesso, false caso contrário.
bool adicionarTransacao(Escalonamento *escalonamento, Transacao *transacao)
{
    escalonamento->grafoTransacoes = realloc(escalonamento->grafoTransacoes,
                                             (escalonamento->quantidadeTransacoes + 1) *
                                                 (escalonamento->quantidadeTransacoes + 1) *
                                                 sizeof(unsigned int *));
    // TODO: implementar checagem
    return false;
}

void imprimirEscalonamento(Escalonamento *escalonamento)
{
    for (int i = 0; i < escalonamento->quantidadeTransacoes; i++)
    {
        for (int j = 0; j < escalonamento->quantidadeTransacoes; j++)
        {
            printf("%d ", escalonamento->grafoTransacoes[i * escalonamento->quantidadeTransacoes + j]);
        }
        printf("\n");
    }
}

void destruirEscalonamento(Escalonamento *escalonamento)
{
    free(escalonamento->grafoTransacoes);
    free(escalonamento);
}