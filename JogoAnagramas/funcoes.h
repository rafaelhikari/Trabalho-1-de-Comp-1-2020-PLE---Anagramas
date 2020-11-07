#ifndef FUNCOES_H
#define FUNCOES_H

#include "Defs.h"
#include "Globais.h"

void cache_linha(int linha);
void cria_lista_x(char *palavra);
void imprime_x();
int puxa_palavra(char *palavra, FILE *dicionario);
int randomiza_palavra_nova(char *palavra, int i, int *linha_atual);
void embaralha_palavra(char *palavra, char *palavra_embaralhada);
int validar_resposta(char *palavra, char resposta[TAM_MAX], int linha_dicionario);
void revela_resposta(char resposta[TAM_MAX], int posicao, char *palavra);
int contador();
void tempo(int segundos);

#endif
