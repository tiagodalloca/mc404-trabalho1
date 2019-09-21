#include "verificador_gramatical.h"
#include "token.h"
#include <string.h>
#include <stdlib.h>

#define DOIS_ELEVADO_A_31_MENOS_1 2147483647
#define MENOS_DOIS_ELEVADO_A_31 -2147483648

char strigual (const char *str1, const char *str2) {
	return strcmp(str1, str2) == 0;
}

char decimalNoRange (const char *str, int n1, int n2) {
	char* _;
	long dec = strtol(str, &_, 10);
	return dec <= n2 && dec >= n1;
}

/*
	-1 - tá errado
	0  - não leva nenhum argumento 
	1  - já leu todos os argumentos 
	2  - ainda há argumentos
*/
char verificarArgumentoToken (Token t, unsigned pos, Token arg) {
	
	if (t.tipo == Instrucao){
		if (strigual(t.palavra, "LSH") ||
				strigual(t.palavra, "RSH")){
			return pos > 1 ? -1 : 0;
		}
		else if (strigual(t.palavra, "LD")     ||
						 strigual(t.palavra, "LDABS")  ||
					   strigual(t.palavra, "LDMQ")   ||
						 strigual(t.palavra, "LDMQMX") ||
						 strigual(t.palavra, "STORE")  ||
						 strigual(t.palavra, "ADD")    ||
						 strigual(t.palavra, "ADDABS") ||
						 strigual(t.palavra, "SUB")    ||
						 strigual(t.palavra, "SUBASS") ||
						 strigual(t.palavra, "MULT")   ||
						 strigual(t.palavra, "DIV")    ||
						 strigual(t.palavra, "JUMP")   ||
						 strigual(t.palavra, "JGE")){
			if (pos > 1) return 0;
			else if (arg.tipo == Nome || arg.tipo == Hexadecimal) return 1;
			else if (arg.tipo == Decimal) {
				return decimalNoRange(arg.palavra, 0, 1023) ? 1 : -1;
			}
		}
	}
	else if (t.tipo == Diretiva){
		if (strigual(t.palavra, ".set")){
			if (pos > 2) return 0;
			else if (pos == 1) {
				return arg.tipo == Nome ? 2 : -1;
			}
			else if (pos == 2) {
				if (arg.tipo == Hexadecimal) return 1;
				else if (arg.tipo == Decimal) {
					return decimalNoRange(arg.palavra, 0, DOIS_ELEVADO_A_31_MENOS_1) ? 1 : -1;
				}
			}
		}
		else if (strigual(t.palavra, ".org")){
			if (pos > 1) return 0;
			if (arg.tipo == Hexadecimal) return 1;
			else if (arg.tipo == Decimal) {
				return decimalNoRange(arg.palavra, 0, 1023) ? 1 : -1;
			}
		}
		else if (strigual(t.palavra, ".align")){
			if (pos > 1) return 0;
			return decimalNoRange(arg.palavra, 0, 1023) ? 1 : -1;
		}
		else if (strigual(t.palavra, ".wfill")){
			if (pos > 2) return 0;
			else if (pos == 1) {
				return decimalNoRange(arg.palavra, 0, 1023) ? 2 : -1;
			}
			else if (pos == 2) {
				if (arg.tipo == Hexadecimal || arg.tipo == Nome) return 1;
				else if (arg.tipo == Decimal) {
					return decimalNoRange(arg.palavra, MENOS_DOIS_ELEVADO_A_31, DOIS_ELEVADO_A_31_MENOS_1) ? 1 : -1;
				}
			}
		}
		else if (strigual(t.palavra, ".word")){
			if (pos > 1) return 0;
			else if (arg.tipo == Hexadecimal || arg.tipo == Nome) return 1;
			else if (arg.tipo == Decimal) {
				return decimalNoRange(arg.palavra, MENOS_DOIS_ELEVADO_A_31, DOIS_ELEVADO_A_31_MENOS_1) ? 1 : -1;
			}
		}
	}
	
	else if (t.tipo == DefRotulo)
		return 0;

	return -1;
}
