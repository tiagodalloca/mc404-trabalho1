#include "erros.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *ERRO_LEXICO = "ERRO LEXICO";
const char *ERRO_GRAMTICAL = "ERRO GRAMATICAL";

const char *ERRO_LEXICO_STRING_MAP [] =
	{
	 "não foi possível realizar o parse da palavra \"%s\""
	};

const char* ERRO_GRAMATICAL_STRING_MAP [] =
	{
	 "número de parâmetros inválido",
	 "parâmetros inválidos",
	 "valor fora do intervalo",
	 "formato de linha invalido"																				 
	};

void get_erro_lexico_string (char *ret, erro_lexico erro, unsigned linha, char *palavra){
	char str[1000];
	strcat(strcat(strcat(strcat(str, ERRO_LEXICO), ": "),
								ERRO_LEXICO_STRING_MAP[erro]),
				 ", linha %d\n");
	sprintf(ret, str, palavra, linha);
}

void get_erro_gramatical_string (char *ret, erro_gramatical erro, unsigned linha){
	char str[1000];
	strcat(strcat(strcat(strcat(str, ERRO_GRAMTICAL), ": "),
								ERRO_GRAMATICAL_STRING_MAP[erro]),
				 ", linha %d\n");
	sprintf(ret, str, linha);
}

