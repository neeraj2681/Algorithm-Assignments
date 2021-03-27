#include<stdio.h>
#include<stdlib.h>

int  main() {

	int nodes, edges;
	//int distances[];
	printf("Enter the number of vertices in the graph(vertices will be numbers from 0 to |V|):");
	scanf("%d", &nodes);

	printf("Enter the number of edges in the graph:");
	scanf("%d", &edges);

	int graph[nodes][nodes];
	int edge_weights[edges];
	for (int i = 0; i < edges; ++i)
	{
		int u, v, weight;
		printf("first vertex number: ");
		scanf("%d", &u);

		printf("second vertex number: ");
		scanf("%d", &v);

		printf("Enter the edge weight: ");
		scanf("%d", &weight);

		graph[u-1][v-1] = weight; 
	}

	printf("checking the graph:\n");
	for (int i = 0; i < nodes; ++i)
	{
		for (int j = 0; j < nodes; ++j)
		{
			printf("%d ", graph[i][j]);
		}
		printf("\n");
	}

	int distances[nodes + 1];
	for (int i = 0; i < nodes + 1; ++i)
	{
		distances[i] = 1000000000+7;
	}
	distances[0] = 0;

	//Bellman Ford
	for (int i = 0; i < nodes; ++i)
	{
		for ()
	}
	return 0;
}