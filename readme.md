# Trabalho 2 - Algoritmos e Estrutura de Dados II

## 📋 Descrição

Implementação de uma **B-Tree (Árvore B)** em C para o curso de Algoritmos e Estrutura de Dados II. O projeto permite criar índices, buscar e remover registros de um arquivo de dados contendo avaliações de filmes.

## 🎯 Funcionalidades

- **Criação de índice**: Carrega 10.000 registros do arquivo `tabelaDados.txt` e constrói a B-Tree
- **Busca de elementos**: Pesquisa registros pelo ID na árvore
- **Remoção de registros**: Remove registros da árvore mantendo suas propriedades
- **Menu interativo**: Interface de linha de comando para operações

## 🗂️ Estrutura do Projeto

```
.
├── btree.h              # Cabeçalho com definições da B-Tree
├── btree.c              # Implementação das funções da B-Tree
├── main.c               # Programa principal com menu interativo
├── tabelaDados.txt      # Arquivo de dados (10.000 registros)
├── TesteMetodos.c       # Testes dos métodos
├── CMakeLists.txt       # Configuração do CMake
└── readme.md            # Este arquivo
```

## 📊 Formato dos Dados

Cada linha do arquivo `tabelaDados.txt` contém uma avaliação de filme no formato:

```
ID Nome Plataforma Filme Avaliação
```

**Exemplo:**
```
07591 Diogo Piratebay Room 731 Gostei muito
09542 Danilo Myfamilie It follows Gostei
```

## ⚙️ Configuração da B-Tree

A ordem da árvore é configurável através dos defines no arquivo `btree.h`:

```c
#define chaveMax 12    // Número máximo de chaves por nó
#define chaveMin 6     // Número mínimo de chaves por nó
```

## 🔧 Compilação

### Usando CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Compilação direta

```bash
gcc main.c btree.c -o btree_program
```

## ▶️ Execução

```bash
./trabAEDIIpratico2BTree
```

## 📱 Como Usar

Ao executar o programa, você verá um menu interativo:

```
==============================
Menu:
1. Criar indice (arvore)
2. Procurar elementos
3. Remover registro
4. Sair
Opcao:
```

### Passo a passo:

1. **Escolha a opção 1** para criar os índices (primeira execução obrigatória)
2. **Escolha a opção 2** para buscar um registro pelo ID
3. **Escolha a opção 3** para remover um registro da árvore
4. **Escolha a opção 4** para sair do programa

## 🏗️ Estrutura da B-Tree

### Elemento
```c
typedef struct {
    int valor;    // ID do registro
    int linha;    // Linha no arquivo onde o registro está
} Elemento;
```

### Nó da Árvore
```c
typedef struct NoArvoreB {
    int numChaves;                      // Número atual de chaves
    Elemento chaves[chaveMax];          // Array de chaves
    struct NoArvoreB *filhos[chaveMax + 1];  // Ponteiros para filhos
    int folha;                          // Flag indicando se é folha
} NoArvoreB;
```

## 🔍 Funções Principais

### Operações de Entrada/Saída
- `processaCarga()`: Carrega dados do arquivo para memória
- `imprimeLinhaBtree()`: Imprime uma linha específica do arquivo
- `imprimeLinhaDireto()`: Imprime linha por ID

### Operações da Árvore
- `criarNo()`: Cria um novo nó vazio
- `inserenaBtree()`: Insere elemento na B-Tree
- `inserirEmNaoCheia()`: Insere em nó não cheio
- `splitNoFilho()`: Divide nó cheio
- `procuraElemento()`: Busca elemento na árvore
- `procuraNo()`: Busca nó específico
- `removeChaveBTree()`: Remove chave da árvore
- `removeChaveNo()`: Remove chave de um nó
- `emprestaDoIrmao()`: Empresta chave de nó irmão
- `merge()`: Funde nós adjacentes

## 📝 Requisitos

- **Compilador C**: GCC ou Clang com suporte a C11
- **CMake**: Versão 3.27 ou superior (opcional)
- **Sistema Operacional**: Windows, Linux ou macOS