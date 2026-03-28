#pragma once

// Узел списка смежности
typedef struct EdgeNode {
    int to;
    int weight;
    struct EdgeNode* next;
} EdgeNode;

// Элемент кучи
typedef struct HeapNode {
    int weight;
    int city;
    int stateID;
} HeapNode;

// Структура min-Кучи
typedef struct MinHeap {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

// Глобальные переменные
extern int n, m, k;
extern EdgeNode** adj;
extern int* owner;

// --- функции кучи ---
// Создание min-кучи
MinHeap* createHeap(int capacity);

// Меняет местами два элемента кучи
void swap(HeapNode* a, HeapNode* b);

// Просеивание элемента вниз
void heapifyDown(MinHeap* heap, int idx);

// Просеивание элемента вверх
void heapifyUp(MinHeap* heap, int idx);

// Добавление элемента в кучу
void pushHeap(MinHeap* heap, int weight, int city, int stateID);

// Извлечение элемента с минимальным приоритетом из кучи
int popHeap(MinHeap* heap, HeapNode* out);

// Освобождение памяти, выделенной под кучу
void freeHeap(MinHeap* heap);

// --- функции графа ---
// Добавление ребра в список смежности
void addEdge(int u, int v, int w);

// Освобождение памяти, выделенной под граф
void freeGraph();