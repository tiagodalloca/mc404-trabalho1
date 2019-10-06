#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

unsigned long power (unsigned long val, unsigned exp) {
	if (exp < 1) return 1;
	return val*power(val, exp - 1);
}

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

// PARTE 2!

void getDefinicoesRotuloes (Token** ret, unsigned* qtde) {
	*qtde = 0;
	for (unsigned i = 0; i < getNumberOfTokens(); i++) {
		Token* t = recuperaToken(i);
		if (t->tipo == DefRotulo){
			ret[(*qtde)++] = t;
		}
	}
}

char isRotuloDefinido (Token** rotulos, unsigned len, char* nome) {
	for (unsigned i = 0; i < len; i++) 
		if (strigual(rotulos[i]->palavra, nome)) return 1;
		
	return 0;
}

RotuloDefinition* getRotuloDefinition (RotuloDefinition* rotulos, unsigned len, char* nome) {
	for (unsigned i = 0; i < len; i++) if (strigual(rotulos[i].Nome, nome)) return &rotulos[i];
	return (RotuloDefinition*)0;
}

unsigned getCodigoInstrucao (Token t) {
	if (strigual(t.palavra, "LD"))
		return 0b00000001;
	if (strigual(t.palavra, "LDINV"))
		return 0b00000010;
	if (strigual(t.palavra, "LDINV"))
		return 0b00000100;
	if (strigual(t.palavra, "LDABS"))
		return 0b00000011;
	if (strigual(t.palavra, "LDMQ"))
		return 0b00001010;
	if (strigual(t.palavra, "LDMQMX"))
		return 0b00001001;
	if (strigual(t.palavra, "STORE"))
		return 0b00100001;
	if (strigual(t.palavra, "ADD"))
		return 0b00000101;
	if (strigual(t.palavra, "ADDABS"))
		return 0b00000111;
	if (strigual(t.palavra, "SUB"))
		return 0b00000110;
	if (strigual(t.palavra, "SUBABS"))
		return 0b00001000;
	if (strigual(t.palavra, "MULT"))
		return 0b00001011;
	if (strigual(t.palavra, "DIV"))
		return 0b00001100;
	if (strigual(t.palavra, "LSH"))
		return 0b00010100;
	if (strigual(t.palavra, "RSH"))
		return 0b00010101;
	if (strigual(t.palavra, "STOREND"))
		return 0b00010010;
	if (strigual(t.palavra, "JUMP"))
		return 0b00001101;
	if (strigual(t.palavra, "JGE"))
		return 0b00001111;
	return 0;
}

void printLine(unsigned long value, unsigned linhaNum) {
	char buffer[11];
	char toPrint[14];
	sprintf(buffer, "%010lX", value);

	toPrint[0] = buffer[0];
	toPrint[1] = buffer[1];

	toPrint[2] = ' ';

	toPrint[3] = buffer[2];
	toPrint[4] = buffer[3];
	toPrint[5] = buffer[4];

	toPrint[6] = ' ';

	toPrint[7] = buffer[5];
	toPrint[8] = buffer[6];

	toPrint[9] = ' ';

	toPrint[10] = buffer[7];
	toPrint[11] = buffer[8];
	toPrint[12] = buffer[9];

	toPrint[13] = 0;

	printf("%03X %s\n", linhaNum, toPrint);
}

unsigned parseValueToken(Token t) {
	unsigned ret = 0;
	if (t.tipo == Decimal)
		ret = (unsigned) atoi(t.palavra);
	else if (t.tipo == Hexadecimal)
		ret = (unsigned) strtol(t.palavra, (void*)0, 0);
	else {
		fprintf(stderr, "%s \"%s\"", "ERRO: não foi possível fazer parse do valor ", t.palavra);
		exit(1);
	}
	return ret;
}

void getSymbolsValues (SymbolValue* ret, unsigned* len) {
	*len = 0;
	for (unsigned i = 0; i < getNumberOfTokens(); i++) {
		Token* t = recuperaToken(i);
		char* p = t->palavra;
		for ( ; *p; ++p) *p = tolower(*p);
		if (t->tipo == Diretiva && strigual(t->palavra, ".set")){
			ret[(*len)++] = (SymbolValue) { recuperaToken(i+1)->palavra, parseValueToken(*recuperaToken(i+2)) };
		}
	}
}

char retrieveSymbolValue (SymbolValue* svs, unsigned len, char* sym, unsigned long* ret) {
	for (unsigned i = 0; i < len; i++) {
		if (strigual(svs[i].Sym, sym)) {
			*ret = svs[i].Value;
			return 1;
		}
	}

	return 0;
}

void processarTokens (LinhaProtoMapa* ret, unsigned* len) {
	SymbolValue symbolsValues[1000];
	unsigned symbolsValues_len = 0;
	getSymbolsValues(symbolsValues, &symbolsValues_len);

	Token* rotulosDefinidos[1000];
	unsigned rotulosDefinidos_len = 0;
	getDefinicoesRotuloes(rotulosDefinidos, &rotulosDefinidos_len);

	*len = 0;
	char isEsq = 1;
	unsigned long linhaAtual = 0;
	unsigned numTokens = getNumberOfTokens();

	RotuloDefinition rotulosDefinitions[1000];
	unsigned rotulosDefinitions_len = 0;

	for (unsigned i = 0; i < numTokens; i++) {
		Token t = *recuperaToken(i);
		
		if (t.tipo == DefRotulo) {
			rotulosDefinitions[rotulosDefinitions_len].IsEsquerda = isEsq;
			t.palavra[strlen(t.palavra)-1] = 0;
			rotulosDefinitions[rotulosDefinitions_len].Nome = t.palavra;
			rotulosDefinitions[rotulosDefinitions_len].Valor = linhaAtual;
			rotulosDefinitions_len++;
		}

		else if (t.tipo == Instrucao) {

			char* p = t.palavra;
			for ( ; *p; ++p) *p = toupper(*p);

			Token arg = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };

			char ehJump = strigual(t.palavra, "JUMP");
			char ehJge = strigual(t.palavra, "JGE");
			char ehSte = strigual(t.palavra, "STOREND");
			if ((ehJump || ehJge || ehSte) &&
					arg.tipo == Nome) {
				ret[*len].NumLinha = linhaAtual;
				if (isEsq) {
					ret[*len].IsJSEEsquerda = ehJump ? 1 : ehJge ? 2 : 3;
					ret[*len].IsPendendoEsquerda = 1;
					ret[*len].RotuloPendendoNomeEsquerda = arg.palavra;
				}
				else {
					ret[*len].IsJSEDireita = ehJump ? 1 : ehJge ? 2 : 3;
					ret[*len].IsPendendoDireita = 1;
					ret[*len].RotuloPendendoNomeDireita = arg.palavra;
				}
				if (!isEsq) {
					linhaAtual++;
					(*len)++;
				}
				isEsq = !isEsq;
				continue;
			}

			unsigned long val = 0;

			unsigned codigoInstrucao = getCodigoInstrucao(t);

			if (arg.tipo != Instrucao && arg.tipo != Diretiva) {

				if (arg.tipo == Nome){
					unsigned long argVal = 0;
					if (retrieveSymbolValue(symbolsValues, symbolsValues_len, arg.palavra, &argVal)) {
						val = codigoInstrucao*power(16, 3) + argVal;
					}
					else if (isRotuloDefinido(rotulosDefinidos, rotulosDefinidos_len, arg.palavra)) {
						val = codigoInstrucao*power(16, 3);
						if (isEsq){
							ret[*len].RotuloPendendoNomeEsquerda = arg.palavra;
							ret[*len].IsPendendoEsquerda = 1;
						}	
						else {
							ret[*len].RotuloPendendoNomeDireita = arg.palavra;
							ret[*len].IsPendendoDireita = 1;
						}
					}
					else {
						fprintf(stderr, "%s %s\n", "ERRO: Rótulo ou símbolo usado mas não definido:", arg.palavra);
						exit(1);
					}
				}

				else {
					val = codigoInstrucao*power(16, 3) + parseValueToken(arg);
				}
			}
			 
			else {
				val = codigoInstrucao*power(16, 3);
			}

			if (isEsq) {
				ret[(*len)].NumLinha = linhaAtual;
				ret[(*len)].Value = val*power(16, 5);
			}	
			else {
				ret[(*len)].NumLinha = linhaAtual;
				ret[*len].Value += val;
				linhaAtual++;
				(*len)++;
			}

			isEsq = !isEsq;
		}

		else if (t.tipo == Diretiva) {
			char* p = t.palavra;
			for ( ; *p; ++p) *p = tolower(*p);

			if (strigual(t.palavra, ".word")) {

				if (!isEsq) {
					isEsq = 1;
					linhaAtual++;
					len++;
				}

				Token arg = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };
				LinhaProtoMapa linha = { linhaAtual, 0, (char*) 0, 0, 0, (char*) 0, 0, 0};
				
				if (arg.tipo == Nome){
					unsigned long argVal = 0;
					if (retrieveSymbolValue(symbolsValues, symbolsValues_len, arg.palavra, &argVal)) {
						linha.Value = argVal;
					}
					else if (isRotuloDefinido(rotulosDefinidos, rotulosDefinidos_len, arg.palavra)) {
						linha.Value = 0;
						linha.IsPendendoDireita = 1;
						linha.RotuloPendendoNomeDireita = arg.palavra;
					}
				}

				else {
					linha.Value = parseValueToken(arg);
				}

				ret[*len] = linha;
				linhaAtual++;
				(*len)++;
			}

			else if (strigual(t.palavra, ".org")){
				Token arg = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };
				unsigned val = parseValueToken(arg);
				linhaAtual = val;
			}

			else if (strigual(t.palavra, ".wfill")) {
				Token arg1 = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };
				Token arg2 = i < numTokens - 1 ? *recuperaToken(i + 2) : (Token) { Instrucao, (char*) 0, 0 };
				unsigned long val1 = parseValueToken(arg1);
				unsigned long val2 = 0;

				if (arg2.tipo == Nome) {
					if (! retrieveSymbolValue(symbolsValues, symbolsValues_len, arg2.palavra, &val2)) {
						fprintf(stderr, "%s %s\n", "ERRO: Rótulo ou símbolo usado mas não definido:", arg2.palavra);
						exit(1);
					}
				}
				else {
					val2 = parseValueToken(arg2);
				}

				for (unsigned j = 0; j < val1; j++) {
					ret[(*len)++] = (LinhaProtoMapa) { linhaAtual, val2, (char*) 0, 0, 0, (char*) 0, 0, 0 };
					linhaAtual++;
				}
			}

			else if (strigual(t.palavra, ".align")) {
				Token arg = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };
				unsigned long argVal = parseValueToken(arg);

				while (!isEsq || linhaAtual % argVal != 0){
					linhaAtual++;
					isEsq = 1;
				}
			}
		}
	}

	if (ret[*len].Value != 0) (*len)++;

	for (unsigned i = 0; i < *len; i++) {
		if (ret[i].IsJSEEsquerda){
			RotuloDefinition* rotulo = getRotuloDefinition(rotulosDefinitions, *len, ret[i].RotuloPendendoNomeEsquerda);
			if (ret[i].IsJSEEsquerda == 1)
				ret[i].Value += (rotulo->IsEsquerda ? 0b00001101 : 0b00001110)*power(16,8);
			else if (ret[i].IsJSEEsquerda == 2)
				ret[i].Value += (rotulo->IsEsquerda ? 0b00001111 : 0b00010000)*power(16,8);
			else if (ret[i].IsJSEEsquerda == 3)
				ret[i].Value += (!rotulo->IsEsquerda ? 0b00010011 : 0b00010010)*power(16,8);
		}

		if (ret[i].IsJSEDireita){
			RotuloDefinition* rotulo = getRotuloDefinition(rotulosDefinitions, *len, ret[i].RotuloPendendoNomeDireita);
			if (ret[i].IsJSEDireita == 1)
				ret[i].Value += (rotulo->IsEsquerda ? 0b00001101 : 0b00001110)*power(16,3);
			else if (ret[i].IsJSEDireita == 2)
				ret[i].Value += (rotulo->IsEsquerda ? 0b00001111 : 0b00010000)*power(16,3);
			else if (ret[i].IsJSEDireita == 3)
				ret[i].Value += (!rotulo->IsEsquerda ? 0b00010011 : 0b00010010)*power(16,3);
		}
		
		if (ret[i].IsPendendoEsquerda){
			RotuloDefinition* rotulo = getRotuloDefinition(rotulosDefinitions, *len, ret[i].RotuloPendendoNomeEsquerda);
			ret[i].Value += rotulo->Valor*power(16,5);
		}
		if (ret[i].IsPendendoDireita){
			RotuloDefinition* rotulo = getRotuloDefinition(rotulosDefinitions, *len, ret[i].RotuloPendendoNomeDireita);
			ret[i].Value += rotulo->Valor;
		}
	}
}

void printarProtoMapa (LinhaProtoMapa* protoMapa, unsigned len) {
	for (int i = 0; i < len; i++) 
		printLine(protoMapa[i].Value, protoMapa[i].NumLinha);
}

