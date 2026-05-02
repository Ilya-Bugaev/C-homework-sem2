#include "graph.h"
#include <stdlib.h>

Graph* createGraph(int vertices)
{
    Graph* graph = malloc(sizeof(Graph));
    if (!graph) {
        return NULL;
    }
    graph->vertexCount = vertices;
    graph->adjacencyList = calloc(vertices + 1, sizeof(EdgeNode*));
    graph->cityOwner = calloc(vertices + 1, sizeof(int));
    if (!graph->adjacencyList || !graph->cityOwner) {
        free(graph->adjacencyList);
        free(graph->cityOwner);
        free(graph);
        return NULL;
    }
    return graph;
}

void graphAddEdge(Graph* graph, int from, int to, int weight)
{
    if (!graph) {
        return;
    }
    EdgeNode* node = malloc(sizeof(EdgeNode));
    if (!node) {
        return;
    }
    node->to = to;
    node->weight = weight;
    node->next = graph->adjacencyList[from];
    graph->adjacencyList[from] = node;
}

void freeGraph(Graph* graph)
{
    if (!graph) {
        return;
    }
    for (int i = 0; i <= graph->vertexCount; i++) {
        EdgeNode* current = graph->adjacencyList[i];
        while (current) {
            EdgeNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adjacencyList);
    free(graph->cityOwner);
    free(graph);
}