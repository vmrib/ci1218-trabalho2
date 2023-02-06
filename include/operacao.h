#pragma once

#define READ 'R'
#define WRITE 'W'
#define COMMIT 'C'

typedef struct Operacao
{
    unsigned short tempoChegada;
    unsigned short idTransacao;
    char operacao;
    char atributo;
} Operacao;