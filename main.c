#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "btree.h"

int main(){

    int opcao = 0, elem, flag = 0;
    char consent[1];
    ArvoreB arvore;
    arvore.raiz = criarNo();
    FILE *f = fopen(FILE_NAME, "r");

    Elemento *arrayElemento = processaCarga();

    printf("O numero maximo de chaves nessa arvore esta colocado para %d "
           "chaves e %d filhos por node, voce esta de acordo com esses parametros?"
           "[s/n]\nResposta:", chaveMax, chaveMax + 1);

    fgets(consent, 2, stdin);
    fflush(stdin);


    if(strcmp(consent,"n") == 0){

        printf("\nPorfavor mude o codigo fonte para que a ordem da arvore seja "
               "alterada, va no arquivo btree.h e mude os defines chaveMax e chaveMin\n");

        return 0;
    }else if(strcmp(consent,"s") == 0){

        printf("\nOtimo, prosseguindo com o codigo...\n");
        sleep(1);
    }else{

        printf("\nResposta invalida, porfavor informe uma resposta valida na "
               "proxima vez que for rodar o codigo...\n");

        return 0;

    }

    /*While que alimenta o menu e que permite que o usuario de comandos ao programa
     * ate se cansar e encerrar o programa
     * */
    while(opcao != 4){


        // Menu
        printf("\n==============================");
        printf("\nMenu: \n");
        printf("1. Criar indice (arvore)\n"
               "2. Procurar elementos\n"
               "3. Remover registro\n"
               "4. Sair\n"
               "Opcao: ");

        // Leitura da opção escolhida
        scanf("%d",&opcao);

        // Criação e população da btree
        if(opcao == 1){
            printf("Processando...\n");
            sleep(1);

            for(int i = 0; i < linhas; i++) {
              inserenaBtree(&arvore, arrayElemento[i]);
            }
            flag = 1;

            printf("Indices criados com sucesso");
        }

        // Opção para procurar elemento na btree e obter linha do arquivo txt
        if((opcao == 2) && (flag == 1)){
          printf("Insira indice do registro: ");
          scanf("%d", &elem);
          printf("\n");
          
          procuraElemento(arvore, elem, f);
          sleep(2);

        }else if((opcao == 2) && (flag == 0)){

            printf("\nArvore de indices nao criada, por favor, crie a arvore"
                   "antes de fazer alguma pesquisa na mesma\n");

            sleep(2);

        }

        // Opção para remover uma linha da btree
        if((opcao == 3) && (flag == 1)){
          printf("Insira indice do registro: ");
          scanf("%d", &elem);

          if(elem <= 0 || elem > linhas){
            printf("\nIndice invalido\n");
            sleep(1);
            continue;
          }

          removeChaveBTree(&arvore, elem);
          printf("\nRegistro deletado com sucesso!\n");
          sleep(1);

        }else if((opcao == 3) && (flag == 0)){

            printf("\nArvore de indices nao criada, por favor, crie a arvore"
                   "antes de fazer alguma remocao na mesma\n");

            sleep(2);
        }

        // Mensagem de despedida
        if(opcao == 4){
            printf("\nSaindo do programa...");
            sleep(1);
        }
    }

//Fecha arquivo
free(arrayElemento);
fclose(f);
    return 0;
}
