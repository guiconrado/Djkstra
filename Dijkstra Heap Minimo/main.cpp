#include <iostream>

#include <iostream>
#include <stdlib.h>
#include "DigraphLA.h"
#include <time.h>
#include <string.h>
#include <limits.h>


/* A função NEWnode recebe um vértice w e o endereço next de um nó e devolve o endereço a de um novo nó tal que a->w == w e a->next == next. */

link NEWnode( Vertex w, link next, long int c) {
  link a = (link)malloc(sizeof(struct node));
	a->w = w;
	a->c = c;
	a->next = next;
	return a;
}

DigraphLA DIGRAPHinit_LA(long int V) {
   DigraphLA G = (DigraphLA)malloc(sizeof *G);
	G->V = V;
	G->A = 0;
	G->adj = (link*)malloc(V * sizeof(link));

	long int v;
	for (v = 0; v < V; v++)
		G->adj[v] = NULL;

	return G;
}


/* REPRESENTAÇÃO POR VETOR DE ARCOS: A função DIGRAPHinsertA insere no digrafo G um (novo) arco com ponta inicial v e ponta final w. Se o arco já existe, a função não faz nada. Supõe-se que v e w são distintos e estão entre 0 e G->V-1. */

void DIGRAPHinsertA_LA( DigraphLA G, Vertex v, Vertex w, long int c) {
  // link a;
 //  for (a = G->adj[v]; a != NULL; a = a->next)
 //     if (a->w == w) return;
   G->adj[v] = NEWnode( w, G->adj[v], c);
   G->A++;
}


DigraphLA lerArquivo(char *nome){
long int i, c, n, m;
	Vertex v, w;

	char arquivo[100] = "";
	strcat(arquivo, nome);
	strcat(arquivo, ".txt");

	FILE *fp;
	fp = fopen(arquivo, "r");
	// n = Ler a primeira linha do arquivo
	fscanf(fp, "%ld", &n);
	// m = Ler a segunda linha do arquivo
	fscanf(fp, "%ld", &m);

	DigraphLA G = DIGRAPHinit_LA(n);


	for (i = 0; i < m; i++) {
		//Para cada linha do arquivo (terceira linha até o final) ler os valores v-w e executar
		fscanf(fp, "%ld %ld %ld", &v, &w, &c);
		DIGRAPHinsertA_LA(G, v - 1, w - 1, c);
	}

	fclose(fp);
	return G;
}


void imprimirListaAdjacencia(DigraphLA G)
{
	int i;
	for(i = 0; i < G->V; i++)
	{
		printf("%d->", i+1);
		link a = G->adj[i];
			while (a != NULL){
				printf("%ld->", (a->w)+1);
				a = a->next;
			}
		printf("NULL\n");
	}
}


void desalocarLista(int V,link *adj){
	link aux, atual;
	int i;
	for( i=0; i<V; i++){
		if(adj[i] != NULL){
			atual = adj[i]->next;
			aux = adj[i];
			free(aux);
			while (atual != NULL){
				aux = atual;
				atual = atual->next;
				free(aux);
			}
		}
	}
}


void initialize(DigraphLA G, Vertex s, Vertex *parent, long int *dist, Vertex *frj)
{
	Vertex w;
	link a;
	long int c;
	for (w = 0; w < G->V; w++) {
		parent[w] = -1;
		dist[w] = LONG_MAX;
	}
	parent[s] = s;
	dist[s] = 0;
	for (a = G->adj[s]; a != NULL; a = a->next) {
		w = a->w, c = a->c;
		dist[w] = c;
		frj[w] = s;
	}
}


MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->maxSize = capacity;
    minHeap->nodes = (HeapNode*) malloc(capacity * sizeof(HeapNode));
    return minHeap;
}


void minHeapInsert(MinHeap* minHeap, int v, long int dist) {
    // Aumenta o tamanho da Heap
    int size = minHeap->size++;
    minHeap->nodes[size].v = v;
    minHeap->nodes[size].dist = dist;

    // Restaura as propriedades da Heap
    while (size && minHeap->nodes[size].dist < minHeap->nodes[(size - 1) / 2].dist) {
        // Troca com o nó pai
        minHeap->nodes[size] = minHeap->nodes[(size - 1) / 2];
        size = (size - 1) / 2;
    }
}


HeapNode minHeapRemoveMin(MinHeap* minHeap) {
    HeapNode root = minHeap->nodes[0];
    minHeap->nodes[0] = minHeap->nodes[--minHeap->size];

    // Restaura as propriedades da Heap
    minHeapify(minHeap, 0);
    return root;
}


void minHeapDecreaseKey(MinHeap* minHeap, int v, long int dist) {
    int i = minHeap->pos[v];
    minHeap->nodes[i].dist = dist;

    // Restaura as propriedades da Heap
    while (i && minHeap->nodes[i].dist < minHeap->nodes[(i - 1) / 2].dist) {
        // Troca com o nó pai
        minHeap->nodes[i] = minHeap->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
}


int isMinHeapEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}



void minHeapify(MinHeap* minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->nodes[left].dist < minHeap->nodes[smallest].dist)
        smallest = left;

    if (right < minHeap->size && minHeap->nodes[right].dist < minHeap->nodes[smallest].dist)
        smallest = right;

    if (smallest != idx) {
        // Troca e continua heapifying
        HeapNode temp = minHeap->nodes[idx];
        minHeap->nodes[idx] = minHeap->nodes[smallest];
        minHeap->nodes[smallest] = temp;
        minHeapify(minHeap, smallest);
    }
}

void freeMinHeap(MinHeap* minHeap) {
    if (minHeap != NULL) {
        // Libera o array de nós
        if (minHeap->nodes != NULL) {
            free(minHeap->nodes);
        }

        // Libera o array de posições
        if (minHeap->pos != NULL) {
            free(minHeap->pos);
        }

        // Libera a própria estrutura da heap
        free(minHeap);
    }
}

void DIGRAPHsptD1(DigraphLA G, Vertex s, Vertex *parent, long int *dist) {
    MinHeap* minHeap = createMinHeap(G->V);

    time_i = clock();

    // Inicializa distâncias e Heap
    for (int v = 0; v < G->V; ++v) {
        dist[v] = LONG_MAX;
        minHeapInsert(minHeap, v, dist[v]);
    }
    dist[s] = 0;
    minHeapDecreaseKey(minHeap, s, dist[s]);

    // Laço principal do algoritmo de Dijkstra
    while (!isMinHeapEmpty(minHeap)) {
        HeapNode minNode = minHeapRemoveMin(minHeap);
        int u = minNode.v;

        // Atualiza a distância dos vértices adjacentes a u
        for (link a = G->adj[u]; a != NULL; a = a->next) {
            int v = a->w;
            long int weight = a->c;

            // Se a distância para v não estiver finalizada e a distância para u + peso for menor
            if (dist[u] != LONG_MAX && dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                minHeapDecreaseKey(minHeap, v, dist[v]);
            }
        }
    }
    time_f = clock();
    freeMinHeap(minHeap);
}



/*void DIGRAPHsptD1(DigraphLA G, Vertex s, Vertex *parent, long int *dist){

	Vertex v0, w, *frj = (Vertex *)malloc(G->V* sizeof(Vertex));
	link a;
	long int c;

	time_i = clock();
	initialize(G, s, parent, dist, frj);

	while (1) {
		long int mindist = LONG_MAX;
		for (w = 0; w < G->V; w++)
			if (parent[w] == -1 && mindist > dist[w])
				mindist = dist[v0 = w];
		if (mindist == LONG_MAX) break;
		parent[v0] = frj[v0];
		for (a = G->adj[v0]; a != NULL; a = a->next) {
			w = a->w, c = a->c;
			if (parent[w] == -1 && dist[w] > dist[v0] + c) {
				dist[w] = dist[v0] + c;
				frj[w] = v0;
			}
		}
	}
	time_f = clock();
	free(frj);
}*/

void escreveArquivo(char *nome, DigraphLA G, long int *dist){

	char arquivo[100] = "";
	strcat(arquivo, nome);
	strcat(arquivo, "_Simples_exec.txt");

	FILE *fp;
	fp = fopen(arquivo, "w");
	fprintf(fp, "Tempo de execucao: %f", timeExec);
	fprintf(fp, "\nDistancia ate ultimo no: %ld\n", dist[G->V - 1]);

	fclose(fp);
}

int main() {

	char nomes[8][50];
	strcpy(nomes[0], "Tempo/USA-road-t-BAY");
	strcpy(nomes[1], "Tempo/USA-road-t-COL");
	strcpy(nomes[2], "Tempo/USA-road-t-FLA");
	strcpy(nomes[3], "Tempo/USA-road-t-NY");
	strcpy(nomes[4], "Distancia/USA-road-d-BAY");
	strcpy(nomes[5], "Distancia/USA-road-d-COL");
	strcpy(nomes[6], "Distancia/USA-road-d-FLA");
	strcpy(nomes[7], "Distancia/USA-road-d-NY");

	printf("---------- Dijkstra Heap Minimo ----------\n");
	int i;
	for (i = 0; i<8; i++){
		printf("\nGrafo: %s", nomes[i]);
		DigraphLA G = lerArquivo(nomes[i]);

		printf("\nVertices: %ld\n", G->V);
		printf("Arcos: %ld\n", G->A);

		Vertex *parent = (Vertex *)malloc(G->V * sizeof(Vertex));
		long int *dist = (long int*)malloc(G->V * sizeof(long int));

		DIGRAPHsptD1(G, 0, parent, dist);
		timeExec = ((double)time_f - (double)time_i) / CLOCKS_PER_SEC;
		printf("Tempo de execucao: %f", timeExec);
		printf("\nDistancia ate ultimo no: %ld\n", dist[G->V - 1]);

		escreveArquivo(nomes[i], G, dist);

		free(parent);
		free(dist);
		desalocarLista(G->V, G->adj);
		free(G);
	}


	system("pause");
	return (EXIT_SUCCESS);
}
