#include "erros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char* get_erro_lexico_string (erro_lexico erro, Token token){
	char* ret = (char*) malloc(sizeof(char)*1000);
	char* str = (char*) malloc(sizeof(char)*1000);
	strcat(strcat(strcat(strcat(ret, ERRO_LEXICO), ": "),
									ERRO_LEXICO_STRING_MAP[erro]),
				 ", linha %d");
	sprintf(ret, str, token.palavra, token.linha);
	return ret;
}

const char* get_erro_gramatical_string (erro_gramatical erro, unsigned linha){
	char* ret = (char*) malloc(sizeof(char)*1000);
	char* str = (char*) malloc(sizeof(char)*1000);
	strcat(strcat(strcat(strcat(ret, ERRO_GRAMTICAL), ": "),
								ERRO_GRAMATICAL_STRING_MAP[erro]),
				 ", linha %d");
	sprintf(ret, str, linha);
	return ret;
}

