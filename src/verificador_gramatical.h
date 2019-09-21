#ifndef VERIFICADOR_GRAMATICAL_H
#define VERIFICADOR_GRAMATICAL_H

#include "token.h"

/*
	-1 - tá errado
	0  - não leva nenhum argumento 
	1  - já leu todos os argumentos 
	2  - ainda há argumentos
*/
char verificarArgumentoToken (Token t, unsigned pos, Token arg);

#endif  /* VERIFICADOR_GRAMATICAL_H */
