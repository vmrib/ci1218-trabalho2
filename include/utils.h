#pragma once

#include "listaTransacao.h"
#include "operacao.h"
#include "listaOperacao.h"

ListaOperacao *lerEntrada();
ListaTransacao *operacaoParaTransacao(ListaOperacao *operacao);
