#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DOIS_ELEVADO_A_31_MENOS_1 2147483647
#define MENOS_DOIS_ELEVADO_A_31 -2147483648

const short num_tipos_token = 6;
const char *tokenRegexes[] =
	{
	 "^(LD|LDINV|LDABS|LDMQ|LDMQMX|STORE|JUMP|JGE|ADD|ADDABS|SUB|SUBABS|MULT|DIV|LSH|RSH|STOREND|ld|ldinv|ldabs|ldmq|ldmqmx|store|jump|jge|add|addabs|sub|subabs|mult|div|lsh|rsh|storend)$",
	 "^\\.(set|org|align|wfill|word|SET|ORG|ALIGN|WFILL|WORD)$",
	 "^[A-Za-z_][A-Za-z0-9_]*\\:$",
	 "^\\0[x|X][0-9|A-F|a-f]{1,12}$", 
	 "^[\\-|\\=+]?[0-9]+$",
	 "^[A-Za-z_][A-Za-z0-9_]*$"
	};

void get_erro_lexico_string (char *ret, unsigned linha){
	sprintf(ret, "ERRO LEXICO: palavra inválida na linha %d!", linha);
}

void get_erro_gramatical_string (char *ret, unsigned linha){
	sprintf(ret, "ERRO GRAMATICAL: palavra na linha %d!", linha);
}

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
		char t_palavra_upper[100];

		unsigned short i = 0;
		for (; t.palavra[i]; i++){
			t_palavra_upper[i] = toupper(t.palavra[i]);
		}
		t_palavra_upper[i] = '\0';

		if (strigual(t_palavra_upper, "LSH")  ||
				strigual(t_palavra_upper, "RSH")  ||
				strigual(t_palavra_upper, "LDMQ")){
			return pos > 1 ? -1 : 0;
		}
		else if (strigual(t_palavra_upper, "LD")     ||
						 strigual(t_palavra_upper, "LDINV")  ||
						 strigual(t_palavra_upper, "LDABS")  ||
						 strigual(t_palavra_upper, "LDMQMX") ||
						 strigual(t_palavra_upper, "STORE")  ||
						 strigual(t_palavra_upper, "ADD")    ||
						 strigual(t_palavra_upper, "ADDABS") ||
						 strigual(t_palavra_upper, "SUB")    ||
						 strigual(t_palavra_upper, "SUBABS") ||
						 strigual(t_palavra_upper, "MULT")   ||
						 strigual(t_palavra_upper, "DIV")    ||
						 strigual(t_palavra_upper, "JUMP")   ||
						 strigual(t_palavra_upper, "JGE")    ||
						 strigual(t_palavra_upper, "STOREND")){
			if (pos > 1) return 0;
			else if (arg.tipo == Nome || arg.tipo == Hexadecimal) return 1;
			else if (arg.tipo == Decimal) {
				return decimalNoRange(arg.palavra, 0, 1023) ? 1 : -1;
			}
		}
	}
	else if (t.tipo == Diretiva){
		char t_palavra_lower[100];

		unsigned short i = 0;
		for (; t.palavra[i]; i++){
			t_palavra_lower[i] = tolower(t.palavra[i]);
		}
		t_palavra_lower[i] = '\0';
		
		if (strigual(t_palavra_lower, ".set")){
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
		else if (strigual(t_palavra_lower, ".org")){
			if (pos > 1) return 0;
			if (arg.tipo == Hexadecimal) return 1;
			else if (arg.tipo == Decimal) {
				return decimalNoRange(arg.palavra, 0, 1023) ? 1 : -1;
			}
		}
		else if (strigual(t_palavra_lower, ".align")){
			if (pos > 1) return 0;
			return decimalNoRange(arg.palavra, 0, 1023) ? 1 : -1;
		}
		else if (strigual(t_palavra_lower, ".wfill")){
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
		else if (strigual(t_palavra_lower, ".word")){
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
