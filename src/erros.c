#include "erros.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *ERRO_LEXICO = "ERRO LEXICO";
const char *ERRO_GRAMTICAL = "ERRO GRAMATICAL";


void get_erro_lexico_string (char *ret, unsigned linha, char *palavra){
	char str[1000];
	strcat(strcat(strcat(strcat(str, ERRO_LEXICO), ": "),
								"não foi possível realizar o parse da palavra \"%s\""),
				 ", linha %d\n");
	sprintf(ret, str, palavra, linha);
}

void get_erro_gramatical_string (char *ret, unsigned linha){
	char str[1000];
	strcat(strcat(strcat(strcat(str, ERRO_GRAMTICAL), ": "),
								"houve um erro gramatical"),
				 ", linha %d\n");
	sprintf(ret, str, linha);
}

