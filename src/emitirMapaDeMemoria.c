#include "utils.h"
#include "montador.h"
#include "token.h"
#include <stdio.h>


/*
    ---- Voce deve implementar essa função para a Parte 2! ----
    Utilize os tokens da estrutura de tokens para montar seu código!
    Retorna:
        *  1 caso haja erro na montagem; (imprima o erro em stderr)
        *  0 caso não haja erro.
 */
int emitirMapaDeMemoria()
{
	LinhaProtoMapa protoMapa[1024];
	unsigned len;
	processarTokens(protoMapa, &len);
	printarProtoMapa(protoMapa, len);
	return 0;
}
