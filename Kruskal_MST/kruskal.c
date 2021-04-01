#include<stdio.h>
#include <malloc.h>

int vertices, edges;

/*
 * represents an edge
 */
struct edge {
    int weight;
    struct vertex *first;
    struct vertex *second;
};

/*
 * represents a vertex
 */
struct vertex {
    int id;
    struct vertex *parent;
    int rank;
};

/*
 * assigns a set to each vertex initially
 */
void do_make_set(struct vertex *node) {
    node->parent = node;
    node->rank = 0;
}

/*
 * find the set id for corresponding vertex
 */
struct vertex *do_find(struct vertex *node) {
    if (node != node->parent)
        node->parent = do_find(node->parent);
    return node->parent;
}

/*
 * Assigns same set id to given two vertices
 */
void do_union(struct vertex *u, struct vertex *v) {
    struct vertex *node1 = do_find(u);
    struct vertex *node2 = do_find(v);
    if (node1->rank > node2->rank)
        node2->parent = node1;
    else {
        node1->parent = node2;
        if (node1->parent == node2->parent)
            node2->rank = node1->rank + 1;
    }
}

/*
* Merge sort to sort the edges in ascending order wrt to edge weights
 */
void merge(struct edge arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    struct edge L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merging the temp arrays back*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].weight <= R[j].weight) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(struct edge arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        //merge both the halves
        merge(arr, l, m, r);
    }
}

int main() {
    printf("Enter the number of vertices: ");
    scanf("%d", &vertices);

    int u, v, weight;

    printf("Enter the number of edges: ");
    scanf("%d", &edges);

    if (edges < vertices - 1) {
        printf("The Graph is disconnected!");
        return 0;
    }

    struct edge edge_weights[edges]; // to store all the edge weights

    for (int i = 0; i < edges; ++i) {
        printf("Enter the edge weight(like u v w(u, v)): ");
        scanf("%d %d %d", &u, &v, &weight);
        struct vertex *first = (struct vertex *) malloc(sizeof(struct vertex));
        first->id = u;
        do_make_set(first);
        struct vertex *second = (struct vertex *) malloc(sizeof(struct vertex));
        second->id = v;
        do_make_set(second);
        struct edge *newone = (struct edge *) malloc(sizeof(struct edge));
        newone->first = first;
        newone->second = second;
        newone->weight = weight;
        edge_weights[i] = *newone;
    }

    mergeSort(edge_weights, 0, edges - 1); // sort the edges

    struct edge spanning_tree[v - 1]; // to store the spanning tree

    int j = 0;
    for (int i = 0; i < vertices - 1;) {
        struct edge e1 = edge_weights[j++];
        if (do_find(e1.first)->id == do_find(e1.second)->id)
            continue;
        else {
            do_union(e1.first, e1.second);
            spanning_tree[i++] = e1;
        }
    }

    printf("Printing the spanning tree \n");
    for (int i = 0; i < vertices - 1; i++) {
        printf("(%d, %d) weight=%d\n", spanning_tree[i].first->id, spanning_tree[i].second->id, spanning_tree[i].weight);
    }

    return 0;
}