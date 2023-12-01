#ifndef __DIGRAPHLA_h
#define __DIGRAPHLA_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define Vertex int

/* A lista de adjacência de um vértice v é composta por nós do tipo node. Cada nó da lista corresponde a um arco e contém um vizinho w de v e o endereço do nó seguinte da lista. Um link é um ponteiro para um node. */

typedef struct {
    int v;      // Vértice
    long int dist; // Distância do vértice à origem
} HeapNode;

typedef struct {
    HeapNode *nodes;    // Array de nós da heap
    int maxSize;        // Tamanho máximo da heap
    int size;           // Tamanho atual da heap
    int *pos;           // Posição dos vértices na heap
} MinHeap;


typedef struct node *link;
struct node {
   Vertex w;
   link next;
   long int c; // Este é o campo que deve existir para armazenar o custo ou peso
};

struct digraphLA {
   int V;
   int A;
   int **mDist;
   link *adj;
};

typedef struct digraphLA *DigraphLA;

void minHeapify(MinHeap* minHeap, int idx);

clock_t time_i, time_f;
double timeExec;


#endif
