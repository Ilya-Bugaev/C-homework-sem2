
#pragma once

// Элемент min-кучи
typedef struct HeapNode {
    int weight;
    int city;
    int stateID;
} HeapNode;

// Структура min-кучи
typedef struct MinHeap {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

// Создание min-кучи
MinHeap* createMinHeap(int capacity);

// Меняет местами два элемента кучи
void swapHeapNodes(HeapNode* a, HeapNode* b);

// Просеивание вниз
void shiftDown(MinHeap* heap, int index);

// Просеивание вверх
void shiftUp(MinHeap* heap, int index);

// Добавление элемента в кучу
void pushHeap(MinHeap* heap, int weight, int city, int stateID);

// Извлечение элемента из кучи
int popHeap(MinHeap* heap, HeapNode* outNode);

// Освобождение памяти, выделенной под кучу
void freeHeap(MinHeap* heap);