#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "btree.h"

#define SEARCH_NUM 1000

/* Esse programa compara o tempo de pesquisa usando Busca Linear
 * e utilizando os indices da B-Tree
 * */
int main ()
{
  srand(time(NULL));

  double medioL = 0.0, medioB = 0.0, tempo_decorrido, maiorL = 0, menorL = 0, maiorB = 0, menorB = 0; 
  int randN;

  Elemento *arrayElemento = processaCarga();
  FILE *f = fopen(FILE_NAME, "r");

  ArvoreB tree;
    tree.raiz = criarNo();

  // Cria a Árvore Binária
  for(int i = 0; i < linhas; i++) {
      inserenaBtree(&tree, arrayElemento[i]);
  }

  
// Busca direta no arquivo.txt linear
// Calcula o maior, menor tempo e o tempo médio para 30 buscas aleatorias
  for(int i = 0; i < SEARCH_NUM; i++) {
    randN = rand() % linhas + 1;

    clock_t begin = clock();
      imprimeLinhaDireto(f, randN);
    clock_t end = clock();

    tempo_decorrido = (double)(end - begin) / CLOCKS_PER_SEC;
    medioL += tempo_decorrido;

    if (tempo_decorrido > maiorL)
      maiorL = tempo_decorrido;

    if (tempo_decorrido < menorL)
      menorL = tempo_decorrido;
  }


// Busca na Btree
// Calcula o maior, menor tempo e o tempo médio para 30 buscas aleatorias
  for(int i = 0; i < SEARCH_NUM; i++) {
    randN = rand() % linhas + 1;

    clock_t begin = clock();
      procuraElemento(tree, randN, f);
    clock_t end = clock();

    tempo_decorrido = (double)(end - begin) / CLOCKS_PER_SEC;
    medioB += tempo_decorrido;

    if (tempo_decorrido > maiorB)
      maiorB = tempo_decorrido;

    if (tempo_decorrido < menorB)
      menorB = tempo_decorrido;
  }


  printf("Exibindo resultados busca linear\n...........................\n");
  printf("Tempo medio da busca Linear: %f\n", (double) medioL / SEARCH_NUM);
  printf("Menor tempo da busca Linear: %f\n", menorL);
  printf("Maior tempo da busca Linear: %f\n\n", maiorL);


  printf("Exibindo resultados busca BTree\n.........................\n");
  printf("Tempo medio da busca Btree: %f\n", (double) medioB / SEARCH_NUM);
  printf("Menor tempo da busca Btree: %f\n", menorB);
  printf("Maior tempo da busca Btree: %f\n", maiorB);


  fclose(f);
  return 0;
}

