#ifndef TOKEN_H
#define TOKEN_H

#define MAXTOKENS 4096

/*
    * Este arquivo contém as definições da estrutura Token.
    * Uma lista estatica (vetor), de 4096 Tokens, já esta implementada (token.c) e pronta para ser utilizada.
    * Para manipular a lista de tokens, utilize as funções abaixo adicionarToken, removerToken, recuperaToken, imprimeListaTokens, getNumberOfTokens
    * Este arquivo também será testado como é, portanto, não altere as funções e a estrutura deste arquivo e do seu respectivo código fonte (token.c)
*/

typedef enum TipoDoToken {Instrucao=1000, Diretiva, DefRotulo, Hexadecimal, Decimal, Nome} TipoDoToken;

typedef struct Token {
  TipoDoToken tipo;
  char* palavra;
  unsigned linha;
} Token;

/*
    * Adiciona um token a lista estatica de Tokens
    * Parametro: Token a ser adicionado
    * Retorno:  Posição deste novo token adicionado na lista de Tokens
                Em caso de erro, aborta o programa
*/
unsigned adicionarToken(TipoDoToken tipo, char* palavra, unsigned int linha);

/**
    * Remove um token da lista estatica de Tokens
    * Parametro: posição do token na lista, ao ser removido
*/
void removerToken(unsigned pos);

/*
    * Recupera o Token armazenado na posição pos da lista
    * Retorno:  Token na posição pos, da lista estática
*/
Token* recuperaToken(unsigned pos);

/*
    * Imprime os tokens adicionados a lista, dado sua posição.
*/
void imprimeListaTokens();

/*
    * Retorna o numero de tokens adicionados a Lista de Tokens
    * Retorno:  Numero de tokens adicionados a lista estática
*/
unsigned getNumberOfTokens();

#endif  /* TOKEN_H */
