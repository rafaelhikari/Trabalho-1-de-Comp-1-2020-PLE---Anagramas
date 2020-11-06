#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define TAM_MAX 7
#define DIC_MAX 47
#define CARAC_MAX 456

int gPontuacao;
int sistema;

void tempo(int segundos);

void puxa_linha(char *db_anagramas, int linha, FILE *anagramas);

int puxa_palavra(char *palavra, FILE *dicionario);

int randomiza_palavra_nova(char *palavra, int i, int *linha_atual, FILE *dicionario);

void embaralha_palavra(char *palavra, char *palavra_embaralhada);

int validar_resposta(char *palavra, char resposta[TAM_MAX], int linha_dicionario, FILE *anagramas);

void cache_linha(FILE *anagramas, int linha);

void imprime_x();

int main(){
    char *palavra = malloc(TAM_MAX * sizeof(char));
    char *palavra_embaralhada = malloc(TAM_MAX*sizeof(char));
    char resposta[TAM_MAX];
    int i, status_resposta, linha[DIC_MAX], *ptrLinha;
    bool vitoria;
    FILE *ptrFile;

    #ifdef _WIN32
        printf("Windows\n");
        sistema = 0;
    #elif WIN64
        printf("Windows\n");
        sistema = 0;
    #elif linux__
        printf("Linux\n");
        sistema = 1;
    #else
        printf("Nao foi possivel identificar o sistema operacional\n");
        sistema = 3;
    #endif

    ptrLinha = &linha[0];

    for(i=0; i<DIC_MAX; i++){

        vitoria = false;

        if((ptrFile = fopen("dicionario.txt", "r")) == NULL){
            printf("Nao foi possivel abrir o dicionario.\n");
            exit(1);
        }

        srand(time(NULL));

        randomiza_palavra_nova(palavra, i, ptrLinha, ptrFile);

        embaralha_palavra(palavra, palavra_embaralhada);

        fclose(ptrFile);

        if((ptrFile = fopen("anagramas.txt", "r")) == NULL){
            printf("Nao foi possivel abrir os anagramas.\n");
            exit(1);
        }

        cache_linha(ptrFile, linha[i]);

        while(1){

            if(vitoria == true)printf("Entre 'p' para jogar com a proxima palavra.\n");
            printf("Pontos: %d\n", gPontuacao);
            printf("%s\n", palavra_embaralhada);
            imprime_x();

            printf("Insira uma resposta: ");
            scanf("%s", &resposta);

            if(strcmp(resposta, "p") == 0){
                if(sistema == 0)system("cls");
                else if(sistema == 1)system("clear");
                break;
            }

            status_resposta = validar_resposta(palavra, resposta, linha[i], ptrFile);

            if(status_resposta == 0){
                gPontuacao += strlen(resposta);
                printf("Acertou!\n");
            }
            else if(status_resposta == 1){
                printf("Errou!\n");
            }
            else if(status_resposta == 2){
                gPontuacao += 10;
                printf("Parabens, acertou a principal!\n");
                vitoria = true;
            }
            tempo(1);
            if(sistema == 0)system("cls");
            else if(sistema == 1)system("clear");
        }

        fclose(ptrFile);
    }
    return 0;
}

void cache_linha(FILE *anagramas, int linha){
    int i;
    char c;
    FILE *cache;

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

    fclose(cache);
}

void imprime_x(){
    FILE *cache;
    char c;

    if((cache = fopen("cache.txt", "r")) == NULL){
            printf("Nao foi possivel criar o cache.\n");
            exit(1);
    }
    c = fgetc(cache);
    while(c != EOF){
        if(c != ' ') putchar('X');
        else putchar(' ');
        c = fgetc(cache);
    }
    putchar('\n');
    fclose(cache);
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

int randomiza_palavra_nova(char *palavra, int i, int *linha, FILE *dicionario){
    int j, verdadeiro = 1;

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

int validar_resposta(char *palavra, char resposta[TAM_MAX], int linha_dicionario, FILE *anagramas){
    char anagrama_atual[TAM_MAX], c;
    int i;

    rewind(anagramas);

    for(i=0; i<linha_dicionario; i++){
        c = fgetc(anagramas);
        while(c != '\n'){
            c = fgetc(anagramas);
        }
    }

    c = fgetc(anagramas);

    while(c != '\n'){
        fseek(anagramas, -1L, SEEK_CUR);
        fscanf(anagramas, "%s", &anagrama_atual);
        if(strcmp(palavra, resposta) == 0) return 2;
        if(strcmp(anagrama_atual, resposta) == 0) return 0;
        c = fgetc(anagramas);
    }
    return 1;
}

void tempo(int segundos){
    int ms = 1000 * segundos;

    clock_t inicio = clock();

    while (clock() < inicio + ms)
        ;
}
