#include <stdio.h>
#include "Defs.h"
#include "Globais.h"

void cache_linha(int linha){
    int i;
    char c;
    FILE *cache, *anagramas;

    if((anagramas = fopen("anagramas.txt", "r")) == NULL){
            printf("Nao foi possivel abrir os anagramas.\n");
            exit(1);
        }

    if((cache = fopen("cache.txt", "w")) == NULL){
            printf("Nao foi possivel criar o cache.\n");
            exit(1);
        }

    for(i=0; i<linha; i++){
        c = fgetc(anagramas);
        while(c != '\n'){
            c = fgetc(anagramas);
        }
    }

    c = fgetc(anagramas);
    while(c != '\n'){
        fputc(c, cache);
        c = fgetc(anagramas);
    }

    fclose(anagramas);
    fclose(cache);
}

void cria_lista_x(char *palavra){
    FILE *cache, *lista_x;
    int i;
    char c;

    if((cache = fopen("cache.txt", "r")) == NULL){
            printf("Nao foi possivel criar o cache.\n");
            exit(1);
    }

    if((lista_x = fopen("lista_x.txt", "w")) == NULL){
            printf("Nao foi possivel criar o lista_x.\n");
            exit(1);
    }

    for(i=0; i<strlen(palavra); i++) fputc('X', lista_x);
    fputc(' ', lista_x);

    c = fgetc(cache);
    while(c != EOF){
        if(c != ' ') fputc('X', lista_x);
        else fputc(' ', lista_x);
        c = fgetc(cache);
    }
    fclose(cache);
    fclose(lista_x);
}

void imprime_x(){
    int tamanho_buffer = LINHA_MAX;
    char *buffer = malloc(tamanho_buffer*sizeof(char));
    FILE *lista_x;

    if((lista_x = fopen("lista_x.txt", "r")) == NULL){
            printf("Nao foi possivel abrir o lista_x.\n");
            exit(1);
    }

    getline(&buffer, &tamanho_buffer, lista_x);
    printf("%s\n", buffer);

    fclose(lista_x);
}

int puxa_palavra(char *palavra, FILE *dicionario){
    int i, r = rand()%46;
    char c;

    rewind(dicionario);

    for(i=0; i<r; i++){
        c = fgetc(dicionario);
        while(c != '\n'){
            c = fgetc(dicionario);
        }
    }

    fscanf(dicionario, "%s", palavra);

    return r;
}

int randomiza_palavra_nova(char *palavra, int i, int *linha){
    int j, verdadeiro = 1;
    FILE *dicionario;

    if((dicionario = fopen("dicionario.txt", "r")) == NULL){
            printf("Nao foi possivel abrir o dicionario.\n");
            exit(1);
        }

    if(i == 0) linha[i] = puxa_palavra(palavra, dicionario);
    else{
        while(verdadeiro == 1){
            linha[i] = puxa_palavra(palavra, dicionario);
            verdadeiro = 0;

            for(j=0; j<i; j++){
                if(linha[j] == linha[i]){
                    verdadeiro = 1;
                    break;
                }
            }
        }
    }
    fclose(dicionario);
}

void embaralha_palavra(char *palavra, char *palavra_embaralhada){
    int i, j, tamanho;
    char temp[7];

    strcpy(palavra_embaralhada, palavra);
    tamanho = strlen(palavra);

    for(i = 0; i < tamanho; i++){
        j = (rand()%tamanho);
        temp[i] = palavra_embaralhada[i];
        palavra_embaralhada[i] = palavra_embaralhada[j];
        palavra_embaralhada[j] = temp[i];
    }
}

int validar_resposta(char *palavra, char resposta[TAM_MAX], int linha_dicionario){
    int i, status;
    char anagrama_atual[TAM_MAX], c;
    char *alvo = malloc(TAM_MAX*sizeof(char));
    char *temp_palavra = malloc(TAM_MAX*sizeof(char));
    FILE *cache, *temp;

    if((cache = fopen("cache.txt", "r")) == NULL){
            printf("Nao foi possivel abrir o cache.\n");
            exit(1);
    }

    if((temp = fopen("apaga_acerto.tmp", "w")) == NULL){
            printf("Nao foi possivel criar o arquivo temporario 'apaga_acerto'.\n");
            exit(1);
    }

    rewind(cache);

    i = 0;
    status = 2;

    if(strcmp(palavra, resposta) == 0){
        strcpy(temp_palavra, palavra);
        palavra[0] = '\0';
        palavra[1] = '\0';

        status = 0;
    }
    else{
        for(i=1; c != EOF; i++){
            c = fgetc(cache);
            if(c == ' '){
                fputc(' ', temp);
                continue;
            }
            else {
                fseek(cache, -1L, SEEK_CUR);
                fscanf(cache, "%s", &anagrama_atual);
                if(strcmp(anagrama_atual, resposta) == 0){

                    c = fgetc(cache);
                    while(c != EOF){
                        fputc(c, temp);
                        c = fgetc(cache);
                    }

                    fclose(cache);
                    fclose(temp);

                    remove("cache.txt");
                    rename("apaga_acerto.tmp", "cache.txt");

                    status = 1;
                }
                else{
                    fputs(anagrama_atual, temp);
                    fputc(' ', temp);
                }
                c = fgetc(cache);
            }
        }
    }
    fclose(cache);
    fclose(temp);

    if(status == 0 || status == 1) revela_resposta(resposta, i, temp_palavra);

    return status;
}

void revela_resposta(char resposta[TAM_MAX], int posicao, char *palavra){
    FILE *lista_x, *temp;
    int i, tamanho_alvo;
    char c, alvo_x[TAM_MAX];

    if((lista_x = fopen("lista_x.txt", "r")) == NULL){
            printf("Nao foi possivel abrir o lista_x.\n");
            exit(1);
    }

    if((temp = fopen("revela_lista_x.tmp", "w")) == NULL){
            printf("Nao foi possivel criar o revela_lista_x.\n");
            exit(1);
    }

    if(posicao == 0){
        fprintf(temp, "%s", resposta);
        fgets(alvo_x, strlen(palavra), lista_x);
        c = fgetc(lista_x);
        while(c != EOF){
            c = fgetc(lista_x);
            fputc(c, temp);
        }
    }

    else{
        for(i=0; i<posicao-1; i++){
            c = fgetc(lista_x);
            while(c != ' '){
                fputc(c, temp);
                c = fgetc(lista_x);
            }
            fputc(c, temp);
        }
        fprintf(temp, "%s", resposta);
        fscanf(lista_x, "%s", alvo_x);

        c = fgetc(lista_x);
        while(c != EOF){
            fputc(c, temp);
            c = fgetc(lista_x);
        }

    }

    fclose(lista_x);
    fclose(temp);

    remove("lista_x.txt");
    rename("revela_lista_x.tmp", "lista_x.txt");
}

int contador(){
    char c;
    int i, contador = 1;
    FILE *cache;

    if((cache = fopen("cache.txt", "r")) == NULL){
            printf("Nao foi possivel abrir o cache.\n");
            exit(1);
    }

    c = fgetc(cache);
    for(i=0; c != EOF; i++){
        if(c == ' ') contador++;
        c = fgetc(cache);
    }
    fclose(cache);
    return contador;
}

void tempo(int segundos){
    int ms = 1000 * segundos;

    clock_t inicio = clock();

    while (clock() < inicio + ms);
}
