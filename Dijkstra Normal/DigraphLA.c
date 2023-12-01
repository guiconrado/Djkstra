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
				printf("%d->", (a->w)+1);
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

void DIGRAPHsptD1(DigraphLA G, Vertex s, Vertex *parent, long int *dist){

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
}

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

int main(int argc, char** argv) {

	char nomes[8][50];
	strcpy(nomes[0], "tempo/USA-road-t-BAY");
	strcpy(nomes[1], "tempo/USA-road-t-COL");
	strcpy(nomes[2], "tempo/USA-road-t-FLA");
	strcpy(nomes[3], "tempo/USA-road-t-NY");
	strcpy(nomes[4], "distancia/USA-road-d-BAY");
	strcpy(nomes[5], "distancia/USA-road-d-COL");
	strcpy(nomes[6], "distancia/USA-road-d-FLA");
	strcpy(nomes[7], "distancia/USA-road-d-NY");

	printf("---------- Dijkstra ----------\n");
	int i;
	for (i = 0; i<8; i++){
		printf("----- Grafo %s\n", nomes[i]);
		DigraphLA G = lerArquivo(nomes[i]);

		printf("Vertices: %ld\n", G->V);
		printf("Arcos: %ld\n", G->A);

		Vertex *parent = (Vertex *)malloc(G->V * sizeof(Vertex));
		long int *dist = (long int*)malloc(G->V * sizeof(long int));

		DIGRAPHsptD1(G, 0, parent, dist);
		timeExec = ((double)time_f - (double)time_i) / CLOCKS_PER_SEC;
		printf("\nTempo de execucao: %f", timeExec);
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
