#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

unsigned power (unsigned val, unsigned exp) {
	if (exp < 1) return val;
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
		return 0x00000110;
	if (strigual(t.palavra, "SUBABS"))
		return 00001000;
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
}

void printLine(unsigned value, unsigned linhaNum) {
	char buffer[10];
	char toPrint[13];
	sprintf(buffer, "%010x", value);

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

	printf("%03x %s", linhaNum, toPrint);
}

unsigned parseValueToken(Token t) {
	if (t.tipo == Decimal)
		return (unsigned) atoi(t.palavra);
	if (t.tipo == Hexadecimal)
		return (unsigned) strtol(t.palavra, (void*)0, 0);

	fprintf(stderr, "%s \"%s\"", "ERRO: não foi possível fazer parde do valor ", t.palavra);
	exit(1);
}

void getSymbolsValues (SymbolValue* ret, unsigned* len) {
	*len = 0;
	for (unsigned i = 0; i < getNumberOfTokens(); i++) {
		Token* t = recuperaToken(i);
		if (t->tipo == Diretiva && strigual(t->palavra, ".set")){
			ret[(*len)++] = (SymbolValue) { recuperaToken(i+1)->palavra, parseValueToken(*recuperaToken(i+2)) };
		}
	}
}

char retrieveSymbolValue (SymbolValue* svs, unsigned len, char* sym, unsigned* ret) {
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
	unsigned linhaAtual = 0;
	unsigned numTokens = getNumberOfTokens();

	RotuloDefinition rotulosDefinitions[1000];
	unsigned rotulosDefinitions_len = 0;

	for (unsigned i = 0; i < numTokens; i++) {
		Token t = *recuperaToken(i);
		
		if (t.tipo == DefRotulo) {
			rotulosDefinitions[rotulosDefinitions_len++].IsEsquerda = isEsq;
			t.palavra[strlen(t.palavra)-1] = 0;
			rotulosDefinitions[rotulosDefinitions_len].Nome = t.palavra;
			rotulosDefinitions[rotulosDefinitions_len].Valor = linhaAtual;
		}

		else if (t.tipo == Instrucao) {
			unsigned val = 0;
			LinhaProtoMapa linha = {linhaAtual, 0, 0, (char*) 0, 0};

			Token arg = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };

			unsigned codigoInstrucao = getCodigoInstrucao(t);

			if (arg.tipo != Instrucao && arg.tipo != Diretiva) {

				if (arg.tipo == Nome){
					unsigned argVal = 0;
					if (retrieveSymbolValue(symbolsValues, symbolsValues_len, arg.palavra, &argVal)) {
						val = codigoInstrucao*power(16, 3) + argVal;
					}
					else if (isRotuloDefinido(rotulosDefinidos, rotulosDefinidos_len, arg.palavra)) {
						val = codigoInstrucao*power(16, 3);
						linha.RotuloPendendoNome = arg.palavra;
						linha.PendendoRotulo = 1;
						linha.IsPendendoEsquerda = isEsq;
					}
					else {
						fprintf(stderr, "%s \"%s\"", "ERRO: Rótulo ou símbolo usado mas não definido:", arg.palavra);
					}
				}

				else {
					val = codigoInstrucao*power(16, 3) + parseValueToken(t);
				}
			}
			 
			else {
				val = codigoInstrucao*power(16, 3);
			}

			if (isEsq) {
				linha.Value = val*power(16, 5);
				ret[(*len)] = linha;
			}	
			else {
				ret[*len].Value = ret[*len].Value + val;
				linhaAtual++;
				(*len)++;
			}

			isEsq = !isEsq;
		}

		else if (t.tipo == Diretiva) {
			if (strigual(t.palavra, ".word")){

				if (!isEsq) {
					isEsq = 1;
					linhaAtual++;
					len++;
				}

				Token arg = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };
				unsigned val;
				LinhaProtoMapa linha = { linhaAtual, 0, 0, (char*) 0, 0 };
				
				if (arg.tipo == Nome){
					unsigned argVal = 0;
					if (retrieveSymbolValue(symbolsValues, symbolsValues_len, arg.palavra, &argVal)) {
						linha.Value = argVal;
					}
					else if (isRotuloDefinido(rotulosDefinidos, rotulosDefinidos_len, arg.palavra)) {
						linha.Value = 0;
						linha.PendendoRotulo = 1;
						linha.IsPendendoEsquerda = 0;
						linha.RotuloPendendoNome = arg.palavra;
					}
				}

				else {
					unsigned val = parseValueToken(t);
				}

				ret[*len] = linha;
				linhaAtual++;
				(*len)++;
			}
		}

		else if (strigual(t.palavra, ".org")){
			Token arg = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };
			unsigned val = parseValueToken(arg);
			linhaAtual = val;
		}

		else if (strigual(t.palavra, ".wfill")) {
			Token arg1 = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };
			Token arg2 = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };
			unsigned val1 = parseValueToken(arg1);
			unsigned val2 = 0;

			if (arg2.tipo == Nome) {
				if (! retrieveSymbolValue(symbolsValues, symbolsValues_len, arg2.palavra, &val2)) {
					fprintf(stderr, "%s \"%s\"", "ERRO: Rótulo ou símbolo usado mas não definido:", arg2.palavra);
					exit(1);
				}
			}
			else {
				unsigned val2 = parseValueToken(arg2);
			}

			for (unsigned j = 0; j < val1; j++) {
				ret[(*len)++] = (LinhaProtoMapa) { linhaAtual, val2, 0, (char*) 0, 0 };
				linhaAtual++;
			}
		}

		else if (strigual(t.palavra, ".align")) {
			Token arg = i < numTokens - 1 ? *recuperaToken(i + 1) : (Token) { Instrucao, (char*) 0, 0 };
			unsigned argVal = parseValueToken(arg);

			while (!isEsq && linhaAtual % argVal != 0){
				linhaAtual++;
				isEsq = 1;
			}
		}
	}

	for (unsigned i = 0; i < *len; i++) {
		if (ret[i].PendendoRotulo){
			RotuloDefinition* rotulo = getRotuloDefinition(rotulosDefinitions, *len, ret[i].RotuloPendendoNome);
			ret[i].Value += rotulo->Valor*(ret[i].IsPendendoEsquerda ? power(16,5) : 1);
		}
	}
}

void printarProtoMapa (LinhaProtoMapa* protoMapa, unsigned len) {
	for (int i = 0; i < len; i++) 
		printLine(protoMapa[i].Value, protoMapa[i].NumLinha);
}

