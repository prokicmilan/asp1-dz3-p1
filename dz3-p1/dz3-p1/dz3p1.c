#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INFTY 999999

//testing purposes
void printGraph(int** graph, int nodes) {
	int i;
	int j;

	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			printf("%d ", graph[i][j]);
		}
		printf("\n");
	}
}

void printMenu() {
	printf("1. Ucitavanje grafa - brise prethodno uneti graf\n");
	printf("2. Dodavanje novog cvora\n");
	printf("3. Dodavanje nove grane\n");
	printf("4. Uklanjanje postojeceg cvora\n");
	printf("5. Uklanjanje postojece grane\n");
	printf("6. Brisanje grafa\n");
	printf("7. Geodezijska distanca cvorova\n");
	printf("8. Dijametar grafa\n");
	printf("9. Relaciona centralnost cvora\n");
	printf("0. Kraj rada\n");
}

/*
Ucitavanje grafa sa standardnog ulaza, vraca matricnu reprezentaciju,
n - broj cvorova, v - broj grana, unose se sa standardnog ulaza
*/
int** loadGraph(int* n, int* e) {
	int nodes;
	int edges; //zasto je ovo bitno?
	int **graph;
	int neighbour;
	int weight;
	int i;
	int j;

	printf("Broj cvorova? ");
	scanf("%d", &nodes);
	printf("Broj grana? ");
	scanf("%d", &edges);
	graph = malloc(sizeof(int*)* nodes);
	if (graph == NULL) {
		exit(1);
	}
	for (i = 0; i < nodes; i++) {
		graph[i] = malloc(sizeof(int)* nodes);
		if (graph[i] == NULL) {
			exit(1);
		}
	}
	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			if (i != j) {
				graph[i][j] = INFTY;
			}
			else {
				graph[i][j] = 0;
			}
		}
	}
	for (i = 0; i < nodes; i++) {
		printf("Susedi cvora %d i tezine grana koje ih spajaju, tezina = -1 za kraj\n", i);
		while (true) {
			scanf("%d%d", &neighbour, &weight);
			if (weight != -1 && neighbour < nodes) {
				graph[i][neighbour] = weight;
				graph[neighbour][i] = weight;
			}
			else {
				break;
			}
		}
	}
	*n = nodes;
	*e = edges;

	return graph;
}

/*
Dodavanje novog cvora u graf, vraca matricnu reprezentaciju novog grafa,
graph - matricna reprezentacija postojeceg, nodes - broj cvorova, vertices - broj grana
*/
int** addNode(int** graph, int* nodes, int* edges) {
	int** newGraph;
	int weight;
	int neighbour;
	int i;

	if (graph == NULL) {
		printf("Graf ne postoji\n");
		return NULL;
	}
	newGraph = NULL;
	newGraph = realloc(graph, sizeof(int*)* (++(*nodes)));
	if (newGraph == NULL) {
		exit(1);
	}
	graph = newGraph;
	newGraph[*nodes - 1] = NULL;
	for (i = 0; i < *nodes; i++) {
		newGraph[i] = realloc(graph[i], sizeof(int)* (*nodes));
		if (newGraph[i] == NULL) {
			exit(1);
		}
		graph[i] = newGraph[i];
	}
	for (i = 0; i < *nodes; i++) {
		graph[i][*nodes - 1] = INFTY;
		graph[*nodes - 1][i] = INFTY;
	}
	printf("Susedi novog cvora i tezine grana koje ih spajaju, tezina = -1 za kraj\n");
	while (true) {
		scanf("%d%d", &neighbour, &weight);
		if (weight != -1 && neighbour < *nodes) {
			graph[*nodes - 1][neighbour] = weight;
			graph[neighbour][*nodes - 1] = weight;
			(*edges)++;
		}
		else {
			break;
		}
	}

	return graph;
}

/*
Dodavanje nove grane u graf
*/
void addEdge(int*** graph, int nodes, int* edges) {
	int nodeA;
	int nodeB;
	int weight;

	if (*graph == NULL) {
		printf("Graf ne postoji\n");
		return;
	}
	printf("Unesite redne brojeve cvorova izmedju kojih se dodaje nova grana: ");
	scanf("%d%d", &nodeA, &nodeB);
	if (nodeA >= nodes || nodeB >= nodes || nodeA < 0 || nodeB < 0) {
		printf("Nepostojeci cvorovi.\n");
		return;
	}
	if ((*graph)[nodeA][nodeB] != INFTY) {
		printf("Grana izmedju zadatih cvorova vec postoji.\n");
		return;
	}
	printf("Unesite tezinu grane izmedju cvorova: ");
	scanf("%d", &weight);
	if (weight != -1) {
		(*graph)[nodeA][nodeB] = (*graph)[nodeB][nodeA] = weight;
		(*edges)++;
	}
}

/*
Uklanjanje cvora iz grafa, njegovo mesto po redosledu zauzima trenutno posledni cvor
*/
int** removeNode(int** graph, int* nodes, int* edges) {
	int node;
	int *tmpRow;
	int tmp;
	int **newGraph;
	int i;

	if (graph == NULL) {
		printf("Graf ne postoji\n");
		return NULL;
	}
	printf("Unesite redni broj cvora koji se brise: ");
	scanf("%d", &node);
	if (node < 0 || node >= (*nodes)) {
		printf("Nepostojeci cvor.\n");
		return graph;
	}
	tmpRow = graph[node];
	graph[node] = graph[(*nodes) - 1];
	graph[(*nodes) - 1] = tmpRow;
	for (i = 0; i < (*nodes); i++) {
		tmp = graph[i][node];
		graph[i][node] = graph[i][(*nodes) - 1];
		graph[i][(*nodes) - 1] = tmp;
	}
	for (i = 0; i < (*nodes); i++) {
		if (graph[i][(*nodes) - 1] > 0) {
			(*edges)--;
		}
	}
	newGraph = realloc(graph, sizeof(int*)* (--(*nodes)));
	if (newGraph == NULL) {
		exit(1);
	}
	graph = newGraph;
	for (i = 0; i < (*nodes); i++) {
		newGraph[i] = realloc(graph[i], sizeof(int)* (*nodes));
		if (newGraph[i] == NULL) {
			exit(1);
		}
		graph[i] = newGraph[i];
	}

	return graph;
}
/*
Uklanjanje grane iz grafa
*/
void removeEdge(int*** graph, int nodes, int* edges) {
	int nodeA;
	int nodeB;

	if (*graph == NULL) {
		printf("Graf ne postoji\n");
		return;
	}
	printf("Unesite redne brojeve cvorova izmedju kojih se brise grana: ");
	scanf("%d%d", &nodeA, &nodeB);
	if (nodeA >= nodes || nodeB >= nodes || nodeA < 0 || nodeB < 0) {
		printf("Nepostojeci cvorovi.\n");
		return;
	}
	if ((*graph)[nodeA][nodeB] == INFTY) {
		printf("Ne postoji grana izmedju zadatih cvorova.\n");
		return;
	}
	(*graph)[nodeA][nodeB] = (*graph)[nodeB][nodeA] = INFTY;
	(*edges)--;
}

/*
Brisanje grafa
*/
int** deleteGraph(int** graph, int* nodes, int* edges) {
	int i;

	if (graph == NULL) {
		printf("Graf ne postoji\n");
		return NULL;
	}
	for (i = 0; i < *nodes; i++) {
		free(graph[i]);
	}
	free(graph);
	graph = NULL;
	(*nodes) = 0;
	(*edges) = 0;

	return graph;
}

/*
Dijkstrin algoritam za odredjivanje najkraceg rastojanja izmedju zadata dva cvora,
koristi se pri odredjivanju najkracih rastojanja za geodezijsku distancu dva cvora
*/

int dijkstra(int** graph, int nodes, int start, int end, int** prev) {
	int* distance;
	bool* visited;
	int minDistance;
	int nextNode;
	int i;
	int count;
	int result;

	distance = malloc(sizeof(int)* nodes);
	if (distance == NULL) {
		exit(1);
	}
	visited = malloc(sizeof(bool)* nodes);
	if (visited == NULL) {
		exit(1);
	}
	for (i = 0; i < nodes; i++) {
		distance[i] = graph[start][i];
		visited[i] = false;
	}
	if (prev != NULL) {
		for (i = 0; i < nodes; i++) {
			(*prev)[i] = graph[start][i] != INFTY ? start : -1;
		}
	}
	distance[start] = 0;
	visited[start] = true;
	count = 1;
	while (count < nodes - 1) {
		minDistance = INFTY;
		for (i = 0; i < nodes; i++) {
			if (distance[i] < minDistance && !visited[i]) {
				minDistance = distance[i];
				nextNode = i;
			}
		}
		visited[nextNode] = true;
		if (nextNode == end) {
			break;
		}
		for (i = 0; i < nodes; i++) {
			if (!visited[i]) {
				if (minDistance + graph[nextNode][i] < distance[i]) {
					distance[i] = minDistance + graph[nextNode][i];
					if (prev != NULL) {
						(*prev)[i] = nextNode;
					}
				}
			}
		}
		count++;
	}
	result = distance[end];
	free(distance);
	free(visited);

	return result;
}

/*
Floyd - Warshallov algoritam za odredjivanje najkraceg rastojanja izmedju svih parova
cvorova u grafu, koristi se pri odredjivanju dijametra grafa i relacione centralnosti grafa
*/

int** floydWarshall(int** graph, int nodes, int*** prev) {
	int** distances;
	int i, j, k;

	distances = malloc(sizeof(int*)* nodes);
	if (distances == NULL) {
		exit(1);
	}
	for (i = 0; i < nodes; i++) {
		distances[i] = malloc(sizeof(int)* nodes);
		if (distances[i] == NULL) {
			exit(1);
		}
	}
	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			distances[i][j] = graph[i][j];
			if (prev != NULL) {
				(*prev)[i][j] = graph[i][j] != INFTY ? i : -1;
			}
		}
	}
	for (k = 0; k < nodes; k++) {
		for (i = 0; i < nodes; i++) {
			for (j = 0; j < nodes; j++) {
				if (distances[i][j] > distances[i][k] + distances[k][j]) {
					if (prev != NULL) {
						(*prev)[i][j] = (*prev)[k][j];
					}
					distances[i][j] = distances[i][k] + distances[k][j];
				}
			}
		}
	}

	return distances;
}

/*
Geodezijska distanca - najkrace rastojanje izmedju dva cvora u grafu
Ispisuje duzinu puta i sve cvorove koji se nalaze na tom putu
*/
void geodesicDistance(int **graph, int nodes) {
	int start, end;
	int* prev;
	int cost;
	int i;

	if (graph == NULL) {
		printf("Graf ne postoji\n");
		return;
	}
	printf("Unesite pocetni i krajnji cvor: ");
	scanf("%d%d", &start, &end);
	if (start < 0 || start >= nodes || end < 0 || end >= nodes) {
		printf("Nepostojeci cvorovi\n");
		return;
	}
	prev = malloc(sizeof(int)* nodes);
	if (prev == NULL) {
		exit(1);
	}
	cost = dijkstra(graph, nodes, start, end, &prev);
	printf("Duzina puta izmedju %d i %d je %d\n", start, end, cost);
	printf("Putanja je %d", end);
	i = end;
	while (i != start) {
		printf("<-%d", prev[i]);
		i = prev[i];
	}
	printf("\n");
}
/*
Dijametar grafa - najveca geodezijska distanca u grafu
*/
int diameter(int** graph, int nodes) {
	int** distances;
	int diam = -1;
	int i, j;

	if (graph == NULL) {
		printf("Graf ne postoji\n");
		return -1;
	}
	distances = floydWarshall(graph, nodes, NULL);
	for (i = 0; i < nodes; i++) {
		for (j = 0; j < nodes; j++) {
			if (distances[i][j] > diam) {
				diam = distances[i][j];
			}
		}
	}

	return diam;
}

int relCenNode(int** graph, int nodes) {
	int** prev;
	int node;
	int i, j, k;
	int count;

	if (graph == NULL) {
		printf("Graf ne postoji\n");
		return -1;
	}
	printf("Unesite zeljeni cvor: ");
	scanf("%d", &node);
	if (node < 0 || node >= nodes) {
		printf("Nepostojeci cvor\n");
		return -1;
	}
	prev = malloc(sizeof(int*)* nodes);
	if (prev == NULL) {
		exit(1);
	}
	for (i = 0; i < nodes; i++) {
		prev[i] = malloc(sizeof(int)* nodes);
		if (prev[i] == NULL) {
			exit(1);
		}
	}
	floydWarshall(graph, nodes, &prev);
	count = 0;
	//printf("Putevi: \n");
	for (i = 0; i < nodes; i++) {
		for (j = i + 1; j < nodes; j++) { //ako se racuna centralnost u oba smera j = 0
			k = j;
			//printf("%d - %d: %d", i, j, j);
			while (k != i) {
				//printf("<-%d", prev[i][k]);
				if (node != i && prev[i][k] == node) {
					count++;
				}
				k = prev[i][k];
			}
			//printf("\n");
		}
	}

	return count;
}

int main(void) {
	int choice;
	bool radi;
	int** graph;
	int nodes;
	int edges;

	graph = NULL;
	nodes = 0;
	edges = 0;
	radi = true;
	while (radi) {
		printMenu();
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			if (graph != NULL) {
				graph = deleteGraph(graph, &nodes, &edges);
			}
			graph = loadGraph(&nodes, &edges);
			printf("Ucitan graf: \n");
			printGraph(graph, nodes);
			break;
		case 2:
			graph = addNode(graph, &nodes, &edges);
			break;
		case 3:
			addEdge(&graph, nodes, &edges);
			break;
		case 4:
			graph = removeNode(graph, &nodes, &edges);
			break;
		case 5:
			removeEdge(&graph, nodes, &edges);
			break;
		case 6:
			graph = deleteGraph(graph, &nodes, &edges);
			break;
		case 7:
			geodesicDistance(graph, nodes);
			break;
		case 8:
			printf("Dijametar grafa je %d\n", diameter(graph, nodes));
			break;
		case 9:
			
			printf("%d\n", relCenNode(graph, nodes));
			break;
		case 0:
			radi = false;
			deleteGraph(graph, &nodes, &edges);
			break;
		default:
			printf("Nepostojeca opcija.\n");
			break;
		}
	}
	return 0;
}