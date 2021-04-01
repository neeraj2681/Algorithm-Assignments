/*
 * @author CS2011(Niraj Kumar)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*
 * an instance of a graph
 */
struct Graph *graph;
/*
 * structure to represent the vertex of a graph
 */
struct Vertex {
    int id;
    struct Edge *next;
};
/*
 * structure to represent an edge of a graph
 */
struct Edge {
    int source, destination, weight;
    struct Edge *next;
};

/*
 * A structure to represent a connected graph
 */
struct Graph {
    int V, E;
    struct Vertex *vertex_array;
};

/*
 * This procedure creates a graph
 */
struct Graph *createGraph(int V, int E) {
    graph = (struct Graph *) malloc(sizeof(struct Graph));

    graph->V = V;
    graph->E = E;
    graph->vertex_array = (struct Vertex *) malloc((V + 1) * sizeof(struct Vertex));

    return graph;
}

/*
 * procedure to add an edge in the graph
 */
void add_edge(struct Edge *edge) {
    edge->next = NULL;
    int vertex_id = edge->source;

    if (graph->vertex_array[vertex_id].next == NULL) {
        graph->vertex_array[vertex_id].next = edge;
        return;
    }

    struct Edge *temp = graph->vertex_array[vertex_id].next;

    while (temp->next != NULL)
        temp = temp->next;

    temp->next = edge;
}
/*
 * procedure to apply Bellman Ford algorithm
 */
int BellmanFord(struct Graph *graph, int source) {
    int V = graph->V;

    int StoreDistance[V];

    int i, j;

    // This is initial step that we know , we initialize all distance to infinity except source.
    // We assign source distance as 0(zero)

    for (i = 0; i < V; i++)
        StoreDistance[i] = INT_MAX;

    StoreDistance[source] = 0;

    //The shortest path of graph that contain V vertices, never contain "V-1" edges. So we do here "V-1" relaxations
    for (i = 0; i < V - 1; i++) {
        for (j = 0; j < V; j++) {
            struct Edge *temp = graph->vertex_array[i].next;
            while (temp != NULL) {
                if (StoreDistance[temp->source] + temp->weight < StoreDistance[temp->destination])
                    StoreDistance[temp->destination] = StoreDistance[temp->source] + temp->weight;
                temp = temp->next;
            }
        }
    }

    // If we get a shorter path, then there is a negative edge cycle.

    for (j = 0; j < V; j++) {
        struct Edge *temp = graph->vertex_array[i].next;
        while (temp != NULL) {
            if (StoreDistance[temp->source] + temp->weight < StoreDistance[temp->destination])
                return 0;
            temp = temp->next;
        }
    }

    for (int i = 1; i < V; i++) {
        struct Vertex v = graph->vertex_array[i];
        struct Edge *temp = v.next;
        while (temp != NULL) {
            temp->weight = temp->weight + StoreDistance[temp->source] - StoreDistance[temp->destination];
            temp = temp->next;
        }
        printf("\n");
    }

    return 1;
}

/*
 * structure to represent a node in Min Heap
 */
struct MinHeapNode {
    int v;
    int dist;
};

/*
 * structure to represent a Min Heap
 */
struct MinHeap {

    int size; //total number of nodes in the heap
    int capacity; //total capacity of the min heap
    int *pos;
    struct MinHeapNode **array;
};

/*
 * procedure to create a min heap node
 */
struct MinHeapNode *newMinHeapNode(int v, int dist) {
    struct MinHeapNode *minHeapNode = (struct MinHeapNode *) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

/*
 * procedure to create min heap
 */
struct MinHeap *createMinHeap(int capacity) {
    struct MinHeap *minHeap =
            (struct MinHeap *)
                    malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *) malloc(
            capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
            (struct MinHeapNode **)
                    malloc(capacity *
                           sizeof(struct MinHeapNode *));
    return minHeap;
}

/*
 * procedure to swap two nodes in the min heap
 */
void swapMinHeapNode(struct MinHeapNode **a,
                     struct MinHeapNode **b) {
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

/*
 * procedure to heapify the heap when required
 */
void minHeapify(struct MinHeap *minHeap,
                int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist <
        minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist <
        minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx) {
        /*The nodes to be swapped in min heap*/
        struct MinHeapNode *smallestNode =
                minHeap->array[smallest];
        struct MinHeapNode *idxNode =
                minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

/*
 * checks if the min heap is empty or not
 */
int isEmpty(struct MinHeap *minHeap) {
    return minHeap->size == 0;
}

/*
 * extracts minimum key element from the min heap
 */
struct MinHeapNode *extractMin(struct MinHeap *
minHeap) {
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode *root =
            minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode *lastNode =
            minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

/*
 * decreases the key of a particular element in the min heap
 */
void decreaseKey(struct MinHeap *minHeap,
                 int v, int dist) {
    // Get the index of v in heap array
    int i = minHeap->pos[v];

    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist <
                minHeap->array[(i - 1) / 2]->dist) {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] =
                (i - 1) / 2;
        minHeap->pos[minHeap->array[
                (i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],
                        &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

/*
 * checks if a particular key is present in the heap or not
 */
int isInMinHeap(struct MinHeap *minHeap, int v) {
    if (minHeap->pos[v] < minHeap->size)
        return 1;
    return 0;
}

/*
 * calculates the distances of all the nodes from a given node
 */
int *dijkstra(int dist[], int src) {
    src--;
    // Get the number of vertices in graph
    int V = graph->V - 1;

    struct MinHeap *minHeap = createMinHeap(V);

    /*
     * creating nodes for min heap as per requirements
     */
    for (int v = 0; v < V; ++v) {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v,
                                           dist[v]);
        minHeap->pos[v] = v;
    }

    /*
     * making the distance of source vertex as 0
     */
    minHeap->array[src] =
            newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V
    minHeap->size = V;

    /*
     * calculating shortest distance to all the nodes from the source vertex
     */
    while (!isEmpty(minHeap)) {
        // Extract the vertex with
        // minimum distance value
        struct MinHeapNode *minHeapNode =
                extractMin(minHeap);

        // Store the extracted vertex number
        int u = minHeapNode->v;

        struct Edge *pCrawl =
                graph->vertex_array[u + 1].next;
        while (pCrawl != NULL) {
            int v = pCrawl->destination - 1;

        /*
         * updates the shortest distance if the new one is smaller than the one already present
         */
            if (isInMinHeap(minHeap, v) &&
                dist[u] != INT_MAX &&
                pCrawl->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->weight;

                /*
                 * updating distances in the min heap as well
                 */
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    // print the calculated shortest distances
    return dist;
}

int main() {
    int V, E, S;  //V = no.of Vertices, E = no.of Edges, S is source vertex

    printf("Enter number of vertices in graph\n");
    scanf("%d", &V);
    V++;

    printf("Enter number of edges in graph\n");
    scanf("%d", &E);

    struct Graph *graph = createGraph(V, E);
    int i;


    for (int j = 1; j < V; ++j) {
        struct Edge *temp = (struct Edge *) malloc(sizeof(struct Edge));
        temp->weight = 0;
        temp->source = 0;
        temp->destination = j;
        add_edge(temp);
    }

    printf("Note: Vertex number of vertices start from 1\n");
    for (i = 0; i < E; i++) {
        printf("\nEnter edge %d properties Source, destination, weight respectively\n", i + 1);
        struct Edge *edge = (struct Edge *) malloc(sizeof(struct Edge));
        scanf("%d %d %d", &edge->source, &edge->destination, &edge->weight);
        edge->next = NULL;
        add_edge(edge);
    }

    int val = BellmanFord(graph, 0);

    /*
     * check for the negative weighted cycle in the graph
     */
    if (val == 0) {
        printf("The graph contains negative weight cycle!\n");
        return 0;
    }

    struct Vertex arr[V - 1];

    for (int i = 0; i < V - 1; i++) {
        arr[i] = graph->vertex_array[i + 1];
    }

    int dist[V - 1];
    int distances[V - 1][V - 1];

    /*
     * calculating shortest distance from all nodes to all nodes using Dijkstra algorithm
     */
    for (int i = 1; i < V; i++) {
        dijkstra(dist, i);
        for (int k = 0; k < V - 1; k++) {
            distances[i - 1][k] = dist[k];
        }
    }

    for (int i = 0; i < V - 1; i++) {
        printf("Shortest distance from vertex %d to all vertices\n", i+1);
        for (int j = 0; j < V - 1; j++) {
            if (distances[i][j] == INT_MAX)
                printf("Unreachable ");
            else
                printf("%d ", distances[i][j]);
        }
        printf("\n");
        printf("\n");

    }
    return 0;
}