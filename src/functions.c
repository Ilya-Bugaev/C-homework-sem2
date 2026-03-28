#include "functions.h"
#include <stdlib.h>

// Глобальные переменные
int n, m, k;
EdgeNode** adj;
int* owner;

// Функции кучи:
MinHeap* createHeap(int capacity)
{
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->data = malloc(sizeof(HeapNode) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(HeapNode* a, HeapNode* b)
{
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyDown(MinHeap* heap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->data[left].weight < heap->data[smallest].weight) {
        smallest = left;
    }
    if (right < heap->size && heap->data[right].weight < heap->data[smallest].weight) {
        smallest = right;
    }

    if (smallest != idx) {
        swap(&heap->data[idx], &heap->data[smallest]);
        heapifyDown(heap, smallest);
    }
}

void heapifyUp(MinHeap* heap, int idx)
{
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[parent].weight > heap->data[idx].weight) {
            swap(&heap->data[parent], &heap->data[idx]);
            idx = parent;
        } else {
            break;
        }
    }
}

void pushHeap(MinHeap* heap, int weight, int city, int stateID)
{
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->data = realloc(heap->data, sizeof(HeapNode) * heap->capacity);
    }
    heap->data[heap->size].weight = weight;
    heap->data[heap->size].city = city;
    heap->data[heap->size].stateID = stateID;
    heapifyUp(heap, heap->size);
    heap->size++;
}

int popHeap(MinHeap* heap, HeapNode* out)
{
    if (heap->size == 0) {
        return 0;
    }
    *out = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return 1;
}

void freeHeap(MinHeap* heap)
{
    free(heap->data);
    free(heap);
}

// Функции графа:
void addEdge(int u, int v, int w)
{
    EdgeNode* node = malloc(sizeof(EdgeNode));
    node->to = v;
    node->weight = w;
    node->next = adj[u];
    adj[u] = node;
}

void freeGraph()
{
    for (int i = 0; i <= n; i++) {
        EdgeNode* curr = adj[i];
        while (curr) {
            EdgeNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(adj);
    free(owner);
}