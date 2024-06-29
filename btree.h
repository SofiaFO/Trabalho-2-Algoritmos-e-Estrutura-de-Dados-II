#include <stdio.h>

// arquivo de dados usado para criar Btree
#define FILE_NAME "tabelaDados.txt"

// quantidade maximas e minimas de chaves da arvore
#define chaveMax 12
#define chaveMin 6

// numero de linhas no arquivo
#define linhas 10000

// tamanho que era pra ser de cada linha
// DEIXE SEMPRE 2 UNIDADES A MAIS PARA FUNCIONAR
#define tamLinha 92

/* Structs
 * Vou começar a explicação pelo segundo struct porq eh melhor
 *
 * Segundo struct é o nó da arvore mesmo, contém chaves de ordem da arvore
 * e ponteiros para apontar para os seus respectivos filhos, possui tbm
 * a variavel folha para ver se é um nó folha da árvore
 *
 * As chaves são guardadas em um vetor de structs, cada elemento possui um
 * índice(variavel valor) que aponta para algum registro no arquivo txt, já
 * a variavel "line" é a linha onde se encontra o registro no arquivo txt,
 * servindo como se fosse a coordenada do registro no arquivo
 *
 * */
typedef struct {
    int valor;
    int linha;
} Elemento;

typedef struct NoArvoreB {
    int numChaves;
    Elemento chaves[chaveMax];
    struct NoArvoreB *filhos[chaveMax + 1];
    int folha;
} NoArvoreB;

typedef struct {
    NoArvoreB *raiz;
} ArvoreB;

/*Explicação de cada função está presente no arquivo btree.c*/

// Entrada e saida de dados
Elemento* processaCarga();

void imprimeLinhaBtree(FILE *f, int linha);

int imprimeLinhaDireto(FILE *f, int id);

// Relacionado a árvore e nós

//Relacionado a inserção
NoArvoreB* criarNo();

void inserenaBtree(ArvoreB *arvore, Elemento elemento);

void inserirEmNaoCheia(NoArvoreB *arvore, Elemento elemento);

void splitNoFilho(NoArvoreB *pai, int Indice, NoArvoreB *filho);

//Relacionado a procura

void procuraElemento(ArvoreB arvore, int valor, FILE *f);

NoArvoreB* procuraNo(NoArvoreB *no, int valor);

//Relacionado a remoção

void emprestaDoIrmao(NoArvoreB *no, int filhoIndice);

Elemento merge(NoArvoreB *no, int mergefilhoIndice);

void removeChaveBTree(ArvoreB *arvore, int valor);

void removeChaveNo(NoArvoreB *no, int valor);



