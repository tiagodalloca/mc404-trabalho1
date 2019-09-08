#ifndef ERROS_H
#define ERROS_H

#include "token.h"

typedef enum erro_lexico {
													ROTULO_INVALIDO = 0,
													NUMERO_DECIMAL_INVALIDO,
													NUMERO_HEXADECIMAL_INVALIDO,
													DIRETIVA_INVALIDA,
													PALAVRA_INVALIDA
													
}  erro_lexico;

typedef enum erro_gramatical {
															NUMERO_DE_PARAMETROS_INVALIDO = 0,
															PARAMETROS_INVALIDOS,														 
															VALOR_FORA_DO_INTERVALO,
															FORMATO_DE_LINHA_INVALIDO
													
}  erro_gramatical;

const char* get_erro_lexico_string (erro_lexico erro, Token token);
const char* get_erro_gramatical_string (erro_gramatical erro, unsigned linha);

const char *ERRO_LEXICO = "ERRO LEXICO";
const char *ERRO_GRAMTICAL = "ERRO GRAMATICAL";

const char** ERRO_LEXICO_STRING_MAP = {
																			 "rótulo \"%s\" inválido",
																			 "decimal \"%s\" inválido",
																			 "hexadecimal \"%s\" inválido",
																			 "diretiva \"%s\" inválida",
																			 "palavra \"%s\" inválida"
};

const char** ERRO_GRAMATICAL_STRING_MAP = {
																					 "número de parâmetros inválido",
																					 "parâmetros inválidos",
																					 "valor fora do intervalo",
																					 "formato de linha invalido"																				 
};

#endif
