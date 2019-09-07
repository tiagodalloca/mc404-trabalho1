#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token tokens[MAXTOKENS];
static unsigned tamTokens = 0;

char *tokenNames [] = {
    "Instrucao",
    "Diretiva",
    "Definicao Rotulo",
    "Hexadecimal",
    "Decimal",
    "Nome"
};

unsigned getNumberOfTokens() {
  return tamTokens;
}

unsigned adicionarToken(TipoDoToken tipo, char* palavra, unsigned int linha) {
  if (tamTokens > MAXTOKENS) {
    printf("Não há mais espaço na lista de tokens!\n");
    exit(1);
  }

  tokens[tamTokens].tipo = tipo;
  tokens[tamTokens].palavra = palavra;
  tokens[tamTokens].linha = linha;
  return tamTokens++;
}

void removerToken(unsigned pos) {
  if (pos > tamTokens) {
    printf("Você tentou acessar uma posição fora dos limites da lista de tokens!\n");
    exit(1);
  }

  for (unsigned i = pos; i < tamTokens-1; i++)
    memcpy(&tokens[i], &tokens[i+1], sizeof(Token));

  --tamTokens;
}

Token* recuperaToken(unsigned pos) {
  if (pos > tamTokens) {
    printf("Você tentou acessar uma posição fora dos limites da lista de tokens!\n");
    exit(1);
  }

  return &tokens[pos];
}

void imprimeListaTokens() {
  printf("Lista de tokens\n");
  for (unsigned i = 0; i < tamTokens; i++) {
    printf("Token %d\n", i);
    printf("\tTipo: %d (%s)\n", (int) tokens[i].tipo, tokenNames[(int) tokens[i].tipo - (int)Instrucao]);
    printf("\tPalavra: \"%s\"\n", tokens[i].palavra);
    printf("\tLinha: %u\n", tokens[i].linha);
  }
}
