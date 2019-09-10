#ifndef ERROS_H
#define ERROS_H

typedef enum erro_lexico
	{
	 O_ERRO_LEXICO = 0
	} erro_lexico;

typedef enum erro_gramatical
	{
	 NUMERO_DE_PARAMETROS_INVALIDO = 0,
	 PARAMETROS_INVALIDOS,														 
	 VALOR_FORA_DO_INTERVALO,
	 FORMATO_DE_LINHA_INVALIDO
													
}  erro_gramatical;

extern const char *ERRO_LEXICO;
extern const char *ERRO_GRAMTICAL;

extern const char *ERRO_LEXICO_STRING_MAP[];

extern const char* ERRO_GRAMATICAL_STRING_MAP[];

void get_erro_lexico_string (char *ret, erro_lexico erro, unsigned linha, char *palavra);
void get_erro_gramatical_string (char *ret, erro_gramatical erro, unsigned linha);

#endif
