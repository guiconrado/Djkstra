#ifndef __DIGRAPHLA_h
#define __DIGRAPHLA_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define Vertex int

/* A lista de adjac�ncia de um v�rtice v � composta por n�s do tipo node. Cada n� da lista corresponde a um arco e cont�m um vizinho w de v e o endere�o do n� seguinte da lista. Um link � um ponteiro para um node. */

typedef struct {
    int v;      // V�rtice
    long int dist; // Dist�ncia do v�rtice � origem
} HeapNode;

typedef struct {
    HeapNode *nodes;    // Array de n�s da heap
    int maxSize;        // Tamanho m�ximo da heap
    int size;           // Tamanho atual da heap
    int *pos;           // Posi��o dos v�rtices na heap
} MinHeap;


typedef struct node *link;
struct node {
   Vertex w;
   link next;
   long int c; // Este � o campo que deve existir para armazenar o custo ou peso
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
