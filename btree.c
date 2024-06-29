#include <stdio.h>
#include <stdlib.h>

#include "btree.h"


/*Essa função pega todas as linhas do arquivo txt e colocam num vetor
 * gigantesco (10000 posicoes) do mesmo struct da chave dos nos da
 * Btree, e por ultimo essa funcao retorna o vetor gigante para 
 * outro lugar, para ser inserido na Btree posteriormente*/
Elemento* processaCarga() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        return NULL;
    }

    // Criação do vetor gigantesco
    int maxLinhas = linhas;  // Número máximo de linhas
    Elemento *arrayElemento = (Elemento *)malloc(maxLinhas * sizeof(Elemento));
    if (arrayElemento == NULL) {
        fclose(file);
        return NULL;
    }

    /*Percorrendo o arquivo linha por linha até chegar na ultima linha
     *
     * Por cada linha que se passa, coloca o indice do registro do arquivo txt
     * em um elemento do array gigante de chaves (linha 1 do arquivo vai para
     * o elemento 0 do vetor, linha 2 do arquivo vai para elemento 1 do vetor,
     * e por ai vai) e por cada linha que se passa pega o indice e o numero da linha
     * e coloca no array gigante de chaves no elemento de posição certa
     *
     * */
    int linhaNumero = 0;
    char linha[100];
    while (fgets(linha, sizeof(linha), file) != NULL) {
        sscanf(linha, "%d", &(arrayElemento[linhaNumero].valor));
        arrayElemento[linhaNumero].linha = linhaNumero;

        linhaNumero++;
    }

    fclose(file);
    return arrayElemento;
}

/* Função responsável por pegar a linha que se encontra o registro que o usuario
 * deseja e exibir ela na tela
 * */
void imprimeLinhaBtree(FILE *f, int linha) {
  char linhaLida[100];

  /* fseek: pega o "cursor" do arquivo que estava no começo do arquivo e move
   * para a linha que se deseja imprimir
   *
   * fgets: Com o cursor do arquivo já colocado no lugar certo, usamos o fgets
   * para pegar aquela linha e guardar todo o seu conteudo em formato de string
   * em uma variavel (linhaLida)
   *
   * rewind: Volta o cursor do arquivo para o início
   * */
  fseek(f, (tamLinha * linha), SEEK_SET);
  fgets(linhaLida, sizeof(linhaLida), f);
  printf("%s\n", linhaLida);
  rewind(f);

}

//Criação de um no da Btree vazio
NoArvoreB* criarNo() {

    NoArvoreB *novoNo = (NoArvoreB *)malloc(sizeof(NoArvoreB));
    if (novoNo == NULL) {
        printf("A alocacao de memória falhou.\n");
        exit(1);
    }

    //no recem criado nao tem chaves ainda
    novoNo->numChaves = 0;
    novoNo->folha = 1;

    // Instancia todos os ponteiros para filhos como NULL
    for (int i = 0; i < chaveMax + 1; i++) {
        novoNo->filhos[i] = NULL;
    }

    return novoNo;
}

/*Função que realiza o split caso tenha que ser feito uma inserção em um nó
 * que já esta cheio
 *
 * O no filho vai ser o filho da esquerda e o novoNo vai ser o filho da direita
 *
 * Depois nós abrimos espaço nos filhos e nas chaves do pai para incluir o valor
 * que foi para cima e apontar para o novo no que foi criado
 * */
void splitNoFilho(NoArvoreB *pai, int indice, NoArvoreB *filho) {
    NoArvoreB *novoNo = criarNo();
    novoNo->folha = filho->folha;
    novoNo->numChaves = chaveMax / 2;

    /*Pega as chaves da metade pra frente do nó criança e passa pro novo
     * no irmao*/
    for (int i = 0; i < novoNo->numChaves; i++) {
        novoNo->chaves[i] = filho->chaves[i + chaveMax / 2];
    }

    /*Se o nó criança nao for folha, pega a metade da frente dos filhos do
     * nó criança e passa pro novo no irmao (pros primeiros ponteiro de
     * filho)*/
    if (!filho->folha) {
        for (int i = 0; i < (chaveMax / 2) + 1; i++) {
            novoNo->filhos[i] = filho->filhos[i + chaveMax / 2];
        }
    }

    filho->numChaves = chaveMax / 2;

    //Ajusta os filhos do nó pai (arrasta pra direita os filhos)
    for (int i = pai->numChaves; i > indice; i--) {
        pai->filhos[i + 1] = pai->filhos[i];
    }

    // Faz o nó pai apontar para o novo no criado
    pai->filhos[indice + 1] = novoNo;

    //Ajusta as chaves do pai (arrasta pra direita as chaves)
    for (int i = pai->numChaves - 1; i >= indice; i--) {
        pai->chaves[i + 1] = pai->chaves[i];
    }

    // Faz a chave que costumava ser do filho subir pro nó pai
    pai->chaves[indice] = filho->chaves[chaveMax / 2 - 1];
    pai->numChaves++;
}


/*Insere um struct que contem o indice e a linha em um dos nos
 * nao cheios da btree.
 *
 * É responsável por percorrer a altura da btree pra poder adicionar
 * num nó folha
 * */
void inserirEmNaoCheia(NoArvoreB *no, Elemento elemento) {
    int i = no->numChaves - 1;

    //Se o no for folha, insere normalmente
    if (no->folha) {
        while (i >= 0 && elemento.valor < no->chaves[i].valor) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }

        no->chaves[i + 1] = elemento;
        no->numChaves++;
     /*Se o no nao for folha, nós passamos a inserção para o filho da esquerda
      * da onde a chave deveria ser colocada
      *
      * Caso o filho da esquerda estiver cheio, nos damos split nesse filho e caso
      * a chave que subiu pro nó pai empaque a chave que queremos inserir nós vamos
      * adicionar a chave no proximo nó filho
      * */
    } else {
        while (i >= 0 && elemento.valor < no->chaves[i].valor) {
            i--;
        }

        i++;

        if (no->filhos[i]->numChaves == chaveMax) {
            splitNoFilho(no, i, no->filhos[i]);

            if (elemento.valor > no->chaves[i].valor) {
                i++;
            }
        }

        inserirEmNaoCheia(no->filhos[i], elemento);
    }
}

/*Função porta de entrada pra inserção na Btree
 *
 * Checa tambem se a raíz esta cheia, se ela estiver da o split e cria uma
 * nova raiz para a nossa Btree
 *
 * é a funcao inserirEmNaoCheia() que vai ser responsável por PERCORRER a árvore
 * e adicionar a nossa chave ao no certo
 *
 * */
void inserenaBtree(ArvoreB *arvore, Elemento elemento) {
    NoArvoreB *raiz = arvore->raiz;

    //Se a raiz tiver cheia, da split
    if (raiz->numChaves == chaveMax) {
        NoArvoreB *novoNo = criarNo();
        novoNo->folha = 0;
        novoNo->filhos[0] = raiz;
        arvore->raiz = novoNo;
        splitNoFilho(novoNo, 0, raiz);
        inserirEmNaoCheia(novoNo, elemento);
    } else {
        //Se a raiz nao tiver cheia, percorra a btree
        inserirEmNaoCheia(raiz, elemento);
    }
}

/*Função que procura um no na nossa Btree que contenha o indice que do
 * arquivo.txt que estamos procurando
 *
 * Essa função vai percorrendo a Btree até achar o no com aquele indice
 * ou até se deparar com um nó folha e perceber que nao existe um nó com
 * aquele índice.
 * */
NoArvoreB* procuraNo(NoArvoreB* no, int valor) {
    int i = 0;

    //Vai percorrendo as chaves pra ver qual filho pegar pra percorrer mais a Btree
    while (i < no->numChaves && valor > no->chaves[i].valor) {
        i++;
    }

    //Se o no tiver o indice, retorne o no
    if (i < no->numChaves && valor == no->chaves[i].valor) {
        return no;
    }

    /*Se estiver em um nó folha e nao tiver encontrado o indice, entao o indice
     * nao existe e temos que retornar NULL*/
    if (no->folha) {
       return NULL;
    }

    // Percorre mais ainda a Btree
    return procuraNo(no->filhos[i], valor);
}

/*Função porta de entrada para começar a pesquisa pelo índice na nossa Btree*/
void procuraElemento(ArvoreB arvore, int valor, FILE *f){

    /*Essa é a linha mais importante, pois chama a função principal por percorrer
     * a árvore e achar o no certo, que é a função procuraNo*/
  NoArvoreB *achou = procuraNo(arvore.raiz, valor);

  /*Se o procuraNo tiver dado certo, nos vamos percorrer as chaves do nosso no
   * ate achar a struct com o indice certo, ao achar o struct certo, nós mandamos
   * para a funcao imprimeLinhaBtree para imprimir na tela a linha do arquivo txt*/
  if (achou != NULL) {
    for (int i = 0; i < achou->numChaves; i++) {
      if(achou->chaves[i].valor == valor) {
        Elemento *elemento = &(achou->chaves[i]);
        imprimeLinhaBtree(f, elemento->linha);
        break;
      }
    }

    //Se nao tivermos achado o nosso indice na btree, retorna esta mensagem
  } else {
      printf("Indice nao encontrado na Btree. Tente inserir outro.\n");
    }

}


/*Função da Btree responsável por fazer o merge de nós quando
 * deletamos uma chave da Btree e não há chaves de irmãos que
 * possamos pegar
 *
 * Fazemos o merge com os filhos de um dado nó (NoArvoreB *no)
 *
 * O nó que ira sofrer o merge é o filho da esquerda e o que ficara vazio
 * será o filho da direita
 *
 * Nós primeiro pegamos a chave do pai e passamos para o filho da esquerda,
 * seguido das chaves do filho da direita que são passados para o filho da
 * esquerda
 *
 * Se não tivermos no nível das folhas, nós precisamos passar todas as crianças
 * do filho da direita para o filho da esquerda
 *
 * Ajustamos as chaves e filhos do pai e por fim liberamos da memória o filho
 * da direita
 * */
Elemento merge(NoArvoreB *no, int indiceFilho) {
    NoArvoreB *filhoEsquerdo = no->filhos[indiceFilho];
    NoArvoreB *filhoDireito = no->filhos[indiceFilho + 1];

    // Leva a chave separadora do pai para o filho da esquerda
    filhoEsquerdo->chaves[filhoEsquerdo->numChaves] = no->chaves[indiceFilho];
    filhoEsquerdo->numChaves++;

    // Pegando as chaves do filho da direita e movendo para o filho da esquerda
    for (int i = 0; i < filhoDireito->numChaves; i++) {
        filhoEsquerdo->chaves[filhoEsquerdo->numChaves + i] = filhoDireito->chaves[i];
    }

    /* Caso o merge esteja sendo feito em um no que não é folha, entao pegamos
     * as crianças do filho da direita e passamos todos pros filhos da esquerda*/
    if (!filhoEsquerdo->folha) {
        for (int i = 0; i <= filhoDireito->numChaves; i++) {
            filhoEsquerdo->filhos[filhoEsquerdo->numChaves + i] = filhoDireito->filhos[i];
        }
    }

    // Remaneja as chaves e os filhos do nó pai (empurrando eles pra esquerda)
    for (int i = indiceFilho; i < no->numChaves - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
        no->filhos[i + 1] = no->filhos[i + 2];
    }

    // Nó pai perde uma de suas chaves
    no->numChaves--;

    // Tiramos da memória o nó que foi usado no merge
    free(filhoDireito);

    // Retorna a chave que veio do pai e foi para o nó que sofreu merge
    return no->chaves[indiceFilho];
}


/*Função que faz um nó abaixo da capacidade de chaves mínimas pegar
 * uma chave de um de seus irmãos
 * */
void emprestaDoIrmao(NoArvoreB *no, int filhoindice) {
    NoArvoreB *filhoEsquerdo = no->filhos[filhoindice - 1];
    NoArvoreB *filhoDireito = no->filhos[filhoindice + 1];

    /*Checa se o irmao da esquerda está apto a emprestar uma chave, se sim, entao
     * pega uma chave emprestada*/
    if (filhoindice > 0 && filhoEsquerdo->numChaves > chaveMin) {

        NoArvoreB *filho = no->filhos[filhoindice];
        //Empurra todas as chaves do nó principal para a direita
        for (int i = filho->numChaves; i > 0; i--) {
            filho->chaves[i] = filho->chaves[i - 1];
        }
        //Pega chave emprestado do pai que pega emprestado do filho da esquerda
        filho->chaves[0] = no->chaves[filhoindice - 1];
        no->chaves[filhoindice - 1] = filhoEsquerdo->chaves[filhoEsquerdo->numChaves - 1];

        /*Caso não estejamos no nível das folhas, então temos que pegar o filho
         * da direita de tudo do nó da esquerda e passar para o primeiro filho
         * do nó principal (nó que recebeu emprestado uma chave)*/
        if (!filho->folha) {
            for (int i = filho->numChaves + 1; i > 0; i--) {
                filho->filhos[i] = filho->filhos[i - 1];
            }
            filho->filhos[0] = filhoEsquerdo->filhos[filhoEsquerdo->numChaves];
        }

        filhoEsquerdo->numChaves--;
        filho->numChaves++;

      /*Checa se o filho da direita pode doar uma chave, se sim, pega uma chave
       * emprestada dele*/
    } else if (filhoindice < no->numChaves && filhoDireito->numChaves > chaveMin) {

        NoArvoreB *filho = no->filhos[filhoindice];
        /*Passa uma chave do pai para o filho e uma chave do filho da direita
         * para o pai
         *
         * Nao precisamos empurrar todas as chaves pra esquerda pois o filho do
         * meio esta faltando chaves nele, e ja vai ser automaticamente adicionado
         * na primeiro posição de chave disponível
         * */
        filho->chaves[filho->numChaves] = no->chaves[filhoindice];
        no->chaves[filhoindice] = filhoDireito->chaves[0];

        /*Se não estivermos no nível das folhas, passamos o primeiro filho do filho
         * da direita para o ultimo filho do filho do meio*/
        if (!filho->folha) {
            filho->filhos[filho->numChaves + 1] = filhoDireito->filhos[0];
        }

        //Empurra as chaves do filho da direita para a esquerda
        for (int i = 0; i < filhoDireito->numChaves - 1; i++) {
            filhoDireito->chaves[i] = filhoDireito->chaves[i + 1];
        }

        //Empurra os filhos do filho da direita para a esquerda
        if (!filhoDireito->folha) {
            for (int i = 0; i < filhoDireito->numChaves; i++) {
                filhoDireito->filhos[i] = filhoDireito->filhos[i + 1];
            }
        }

        filhoDireito->numChaves--;
        filho->numChaves++;

        /*Se nao der pra pegar emprestado de nenhum dos irmaos, entao
         * faz o merge ou com o filho da esquerda o com o filho da direita*/
    } else {

        if (filhoindice > 0) {

            merge(no, filhoindice - 1);
        } else {

            merge(no, filhoindice);
        }
    }
}

/*Função de entrada para a remoção na Btree
 *
 * Caso a raiz esteja vazia, para a função
 *
 * Se a raiz ficar sem chaves, substitue ela pelo seu filho
 *
 * */
void removeChaveBTree(ArvoreB *arvore, int valor) {
    if (arvore->raiz == NULL) {
        printf("Btree esta vazia.\n");
        return;
    }

    //Função principal responsavel por percorrer a Btree e deletar o indice da Btree
    removeChaveNo(arvore->raiz, valor);

    // Se a raiz ficar vazia, substitua essa raiz pelo o seu filho restante
    if (arvore->raiz->numChaves == 0) {
        NoArvoreB *raizAntiga = arvore->raiz;
        arvore->raiz = arvore->raiz->filhos[0];
        free(raizAntiga);
    }
}


/*Função que procura o índice procurado na Btree para remover-lo
 *
 * Se for um índice em um nó folha, apaga ele normalmente
 *
 * Se for um índice em um nó intermediario, procura pelo predecessor
 * ou pelo sucessor para substituir-lo, caso não possa ser achado nenhum
 * dos dois, da um merge com o irmao da direita
 *
 * Se não achar o índice no nó atual, vai percorrendo a árvore até achar
 * o no com índice
 * */
void removeChaveNo(NoArvoreB *no, int valor) {
    int i = 0;

    //Percorremos o nó atual até acharmos um índice maior que o procurado
    while (i < no->numChaves && valor > no->chaves[i].valor) {
        i++;
    }

    /*Se acharmos o nosso índice(valor) no nó atual, nós vamos remover ele*/
    if (i < no->numChaves && valor == no->chaves[i].valor) {

        //Se acharmos o indice em um nó folha...
        if (no->folha) {

            /*Da overwrite na chave que achamos e empurra todas as chaves para
             * a esquerda*/
            for (int j = i; j < no->numChaves - 1; j++) {
                no->chaves[j] = no->chaves[j + 1];
            }
            no->numChaves--;

            //Se acharmos o indice em um nó interno...
        } else {

            NoArvoreB *filhoEsquerdo = no->filhos[i];
            NoArvoreB *filhoDireito = no->filhos[i + 1];

            /*Procuramos pelo índice predecessor pasa substituir o que sera
             * excluido*/
            if (filhoEsquerdo->numChaves >= chaveMin) {

                NoArvoreB *predecessor = filhoEsquerdo;
                while (!predecessor->folha) {
                    predecessor = predecessor->filhos[predecessor->numChaves];
                }
                //Da overwrite no indice que queremos excluir pelo predecessor
                no->chaves[i] = predecessor->chaves[predecessor->numChaves - 1];

                // Aplica a função no predecessor para remover ele da árvore
                removeChaveNo(filhoEsquerdo, predecessor->chaves[predecessor->numChaves - 1].valor);

                /*Se não der para achar o predecessor, procuramos pelo sucessor*/
            } else if (filhoDireito->numChaves >= chaveMin) {

                NoArvoreB *successor = filhoDireito;
                //Procura pelo sucessor
                while (!successor->folha) {
                    successor = successor->filhos[0];
                }
                //Da overwrite no indice que se quer excluir pelo sucessor
                no->chaves[i] = successor->chaves[0];

                // Aplica a função no sucessor para remover ele da arvore
                removeChaveNo(filhoDireito, successor->chaves[0].valor);

                //Se nao pudemos achar nem o predecessor e nem o sucessor...
            } else {

                /* Junta o índice com o seu irmão da direita em um nó só e substitui
                 * o indice que se quer remover pelo indice do pai*/
                no->chaves[i] = merge(no, i);

                // Aplica a função no indice que foi o substituidor
                removeChaveNo(no->filhos[i], valor);
            }
        }
    } else {
        // índice não achado no nó atual, logo, percorre-se a Btree
        NoArvoreB *filho = no->filhos[i];

        /*Vai preemptivamente pegando emprestado do irmao antes de passar a vez
         * para o nó filho*/
        if (filho->numChaves == chaveMin) {

            emprestaDoIrmao(no, i);

        }

        //Percorre a arvore até achar o nó com o índice procurado
        removeChaveNo(filho, valor);
    }
}

//Função para buscar no arquivo.txt direto, usado no código de comparação de métodos
int imprimeLinhaDireto(FILE *f, int id) {
    char buffer[50];
    int h = 0;

    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        sscanf(buffer, "%d", &h);
        if(h == id) {
            printf("%s\n", buffer);
            rewind(f);
            return 1;
        }
    }

    return 0;
}
