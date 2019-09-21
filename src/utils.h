#ifndef UTILS_H
#define UTILS_H

#include "token.h"

extern const short num_tipos_token;
extern const char *tokenRegexes[];

void get_erro_lexico_string (char *ret, unsigned linha);
void get_erro_gramatical_string (char *ret, unsigned linha);

/*
	-1 - tá errado
	0  - não leva nenhum argumento 
	1  - já leu todos os argumentos 
	2  - ainda há argumentos
*/
char verificarArgumentoToken (Token t, unsigned pos, Token arg);

#endif  /* UTILS_H */
