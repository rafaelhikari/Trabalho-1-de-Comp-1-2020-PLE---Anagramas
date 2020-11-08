#include <stdio.h>
#include <stdlib.h>
#include "Defs.h"
#include "Globais.h"
#include "funcoes.h"

int main(){
    char *palavra = malloc(TAM_MAX * sizeof(char));
    char *palavra_embaralhada = malloc(TAM_MAX*sizeof(char));
    char resposta[TAM_MAX];
    int i, status_resposta, anagramas_restantes, linha[DIC_MAX], *ptrLinha;
    bool vitoria;

    ptrLinha = &linha[0];

    printf("Ola! Bem vindo ao jogo Anagramas!\n");
    printf("Feito por Lucas Giovani e Rafael Nascimento\n");
    printf("Regras:\n");
    printf("Use as letras da palavra embaralhada para formar novas palavras\n");
    printf("As respostas validas restantes sao exibidas censuradas com 'X'\n");
    printf("Verbos so valem se estiverem no infinitivo.\n");
    printf("Cada palavra vale sua quantidade de letras, exceto a principal, que vale 10 pontos.\n");
    printf("\nBoa jogatina!\n");
    printf("Pressione qualquer tecla para continuar");
    getchar();
    system(CLS);

    for(i=0; i<DIC_MAX; i++){

        vitoria = false;

        srand(time(NULL));

        randomiza_palavra_nova(palavra, i, ptrLinha);
        embaralha_palavra(palavra, palavra_embaralhada);
        cache_linha(linha[i]);
        cria_lista_x(palavra);

        while(1){

            anagramas_restantes = contador();

            printf("Pontos: %d\n", gPontuacao);
            printf("%s\n", palavra_embaralhada);
            imprime_x();
            if(vitoria == true)printf("Entre 'p' para jogar com a proxima palavra.\n");

            printf("Insira uma resposta: ");
            scanf("%s", &resposta);

            if(vitoria == true && strcmp(resposta, "p") == 0){
                system(CLS);
                system(CLS);
                break;
            }

            status_resposta = validar_resposta(palavra, resposta, linha[i]);

            if(status_resposta == 0){
                gPontuacao += 10;
                printf("Parabens, acertou a principal!\n");
                vitoria = true;
            }
            else if(status_resposta == 1){
                gPontuacao += strlen(resposta);
                printf("Acertou!\n");
            }
            else if(status_resposta == 2){
                printf("Errou!\n");
            }
            tempo(1);
            system(CLS);
            system(CLS);
        }
    }
    return 0;
}
