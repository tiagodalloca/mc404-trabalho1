#include "token.h"
#include "montador.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>


char* strappc (char *str, char c) {
	char str_c[2];
	str_c[0] = c;
	str_c[1] = '\0';
	return strcat(str, str_c);
}

char regexMatch(const char *regex_str, char *palavra) {
	regex_t regex;
	int aux;
	char msgbuf[100];

	aux = regcomp(&regex, regex_str, REG_EXTENDED);
	if (aux) {
		fprintf(stderr, "Falha ao compilar regex \"%s\".\n", regex_str);
		return 1;
		return 0;
	}

	aux = regexec(&regex, palavra, 0, NULL, 0);
	if (!aux) return 1;
	if (aux == REG_NOMATCH) return 0;
	else {
		regerror(aux, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Falhar ao fazer match no regex (%s): %s\n", regex_str, msgbuf);
		return 1;
		return 0;
	}
	return 0;
}

char addToken(char *palavra, unsigned linha) {
	for (int i = 0; i < num_tipos_token; i++) {
		if (regexMatch(tokenRegexes[i], palavra)){
			adicionarToken(i + Instrucao, palavra, linha);
			return 1;
		}
	}

	return 0;
}

/*
	---- Você Deve implementar esta função para a parte 1.  ----
	Essa função é chamada pela main em main.c
	Entrada da função: arquivo de texto lido e seu tamanho
	Retorna:
	* 1 caso haja erro na montagem; (imprima o erro em stderr)
	* 0 caso não haja erro.         (Caso não haja erro, na parte 1, ao retornar desta função, a lista de Tokens (adicionados utilizando a função adicionarToken()) é impressa)
	*/
int processarEntrada(char* entrada, unsigned tamanho) {
	char err_str[1000];

	// utilizar regex para fazer a verificação lexica nas palavras e adicionar na lista de tokens

	char *palavra = (char*) malloc(sizeof(char)*256);
	unsigned linha = 1;

	for (unsigned i = 0; i < tamanho; i++) {
		
		char c = entrada[i];

		// COMENTÁRIO
		if (c == '#')
			while ((c = entrada[++i]) != '\n' && i < tamanho) {}
			 

		if (! strchr(" \n", c))
			strappc(palavra, c);
		
		else {
			if (palavra[0]) {
				char *palavra_copia = (char*) malloc(sizeof(char) * 256);
				strcpy(palavra_copia, palavra);
				if (addToken(palavra_copia, linha))
					palavra[0] = '\0';
				else{
					get_erro_lexico_string(err_str, linha);
					fprintf(stderr, "%s\n", err_str);

					free(palavra);
						
					return 1;
				}
			}

			if (c == '\n') linha++;
		}
	}

	free(palavra);

	// recriar o spec para fazer a verificação gramatical

	unsigned token_count = getNumberOfTokens();
	
	if (token_count == 0)
		return 0;

	Token t = *recuperaToken(0);
	Token arg = t;
	unsigned pos = 1;
	char isProxToken = 0;
	char analiseRet = 0;

	if (token_count == 1 && verificarArgumentoToken(t, pos, arg) != 0){
		get_erro_gramatical_string(err_str, t.linha);
		fprintf(stderr, "%s\n", err_str);
		return 1;
	}

	for (unsigned i = 1; i < getNumberOfTokens(); i++){
		if (isProxToken){
			t = *recuperaToken(i);
			isProxToken = 0;
			pos = 1;
			continue;
		}

		arg = *recuperaToken(i);
		analiseRet = verificarArgumentoToken(t, pos, arg);
		if (analiseRet == 0){
			t = arg;
			pos = 1;
			continue;
		}
		else if (analiseRet == 1){
			isProxToken = 1;
			continue;
		}
		else if (analiseRet == 2){
			pos++;
			continue;
		}
		else if (analiseRet == -1) {			
			get_erro_gramatical_string(err_str, t.linha);
			fprintf(stderr, "%s\n", err_str);
			return 1;
		}
	}

	if (analiseRet == 2 || ((analiseRet == 0 || (analiseRet == 1 && !isProxToken)) && verificarArgumentoToken(t, 1, t) != 0)){
		get_erro_gramatical_string(err_str, t.linha);
		fprintf(stderr, "%s\n", err_str);
		return 1;
	}

	// profit ??
	
	return 0;
}
