#include <iostream>

// Dijkstra.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "queue.c"


#define Vertex long int

typedef struct node *link;

struct node {
	Vertex w;
	long int c;
	link next;
};

struct digraphLA {
	long int V;
	long int A;
	link *adj;
};

typedef struct digraphLA *DigraphLA;

clock_t time_i, time_f;
double timeExec;

long int maxC;

/* A função NEWnode recebe um vértice w e o endereço next de um nó e devolve o endereço a de um novo nó tal que a->w == w e a->next == next. */

link NEWnode(Vertex w, link next, long int c) {
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

void DIGRAPHinsertA_LA(DigraphLA G, Vertex v, Vertex w, long int c) {
	// link a;
	//  for (a = G->adj[v]; a != NULL; a = a->next)
	//     if (a->w == w) return;
	G->adj[v] = NEWnode(w, G->adj[v], c);
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

	maxC = 0;
	for (i = 0; i < m; i++) {
		//Para cada linha do arquivo (terceira linha até o final) ler os valores v-w e executar
		fscanf(fp, "%ld %ld %ld", &v, &w, &c);
		DIGRAPHinsertA_LA(G, v - 1, w - 1, c);

		if (maxC < c){
			maxC = c;
		}

	}

	fclose(fp);
	return G;
}


void imprimirListaAdjacencia(DigraphLA G)
{
	int i;
	for (i = 0; i < G->V; i++)
	{
		printf("%d->", i + 1);
		link a = G->adj[i];
		while (a != NULL){
			printf("%ld->", (a->w) + 1);
			a = a->next;
		}
		printf("NULL\n");
	}
}


void desalocarLista(long int V, link *adj){
	link aux, atual;
	long int i;
	for (i = 0; i<V; i++){
		if (adj[i] != NULL){
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

void desalocarFila(long int V, fila *adj){
	long int i;
	for (i = 0; i<V; i++){
		while(adj[i].count>0){
			desenfileira(&adj[i]);
		}
	}
    free(adj);
}

void DijkstraDial(DigraphLA G, Vertex s, Vertex *parent, long int *dist){
	Vertex *visit = (Vertex *) malloc(G->V* sizeof(Vertex)),k,v;
	long int i, nVisitados;
	link a;

	fila *filas = (fila*)malloc((maxC+1) * sizeof(fila));
	for (i = 0; i < maxC+1; i++){
		inicializa_fila(&filas[i]);
	}

	time_i = clock();
	for (i = 0; i < G->V; i++){
		dist[i] = LONG_MAX;
		parent[i] = -1;
		visit[i] = 0;
	}
	nVisitados = 0;

	dist[s] = 0;
	parent[s] = 0;

	enfileira(&filas[0],s);

	k = 0;

	while (nVisitados < G->V){
		if ((k == maxC) && (filas[k].count==0)){
			k = 0;
		}

        if (filas[k].count==0){
			k++;
		}
		else{
			v = desenfileira(&filas[k]);
			if (visit[v] == 0){
				visit[v] = 1;
				nVisitados++;

				for (a = G->adj[v]; a != NULL; a = a->next) {
					if ((dist[a->w] > dist[v] + a->c) && (visit[a->w] == 0)){
						dist[a->w] = dist[v] + a->c;
						parent[a->w] = v;
						enfileira(&filas[dist[a->w] % (maxC + 1)],(a->w));
					}
				}
			}
		}
	}

	time_f = clock();

	desalocarFila((maxC+1),filas);
	free(visit);
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

void escreveArquivo(char *nome, DigraphLA G, long int *dist){

	char arquivo[100] = "";
	strcat(arquivo, nome);
	strcat(arquivo, "_Dial_exec.txt");

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

	printf("---------- Dijkstra  Dial ----------\n");
	int i;
	for (i = 0; i<8; i++){
		printf("\nGrafo %s     |", nomes[i]);
		DigraphLA G = lerArquivo(nomes[i]);
		printf("     maxC: %ld\n", maxC);

		printf("Vertices: %ld\n", G->V);
		printf("Arcos: %ld\n", G->A);

		Vertex *parent = (Vertex *)malloc(G->V * sizeof(Vertex));
		long int *dist = (long int*)malloc(G->V * sizeof(long int));


		DijkstraDial(G, 0, parent, dist);

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
