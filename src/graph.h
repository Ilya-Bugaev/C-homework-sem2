#pragma once

// Узел списка смежности
typedef struct EdgeNode {
    int to;
    int weight;
    struct EdgeNode* next;
} EdgeNode;

// Структура графа
typedef struct Graph {
    int vertexCount;
    EdgeNode** adjacencyList;
    int* cityOwner;
} Graph;

Graph* createGraph(int vertices);

// Добавление ребра в список смежности
void graphAddEdge(Graph* graph, int from, int to, int weight);

// Освобождение памяти, выделенной под граф
void freeGraph(Graph* graph);