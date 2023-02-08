#pragma once

#include "listaTransacao.h"
#include "transacao.h"
#include "operacao.h"

Operacao* lerEntrada(int *tamVetor);
ListaTransacao operacaoParaTransacao(Operacao* operacao, int tam);
