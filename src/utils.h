#ifndef UTILS_H
#define UTILS_H

#include "token.h"

typedef struct {
	char* Sym;
	unsigned Value;
} SymbolValue;

typedef struct {
	char* Nome;
	unsigned Valor;
	char IsEsquerda;
} RotuloDefinition;

typedef struct {
	unsigned NumLinha;
	unsigned Value;
	char PendendoRotulo;
	char* RotuloPendendoNome;
	char IsPendendoEsquerda;
} LinhaProtoMapa;

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

// PARTE 2

void printarProtoMapa (LinhaProtoMapa* protoMapa, unsigned len);
void processarTokens (LinhaProtoMapa* ret, unsigned* len);

#endif  /* UTILS_H */
