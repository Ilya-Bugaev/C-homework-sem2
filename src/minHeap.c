#include "minHeap.h"
#include <stdlib.h>

MinHeap* createMinHeap(int capacity)
{
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) {
        return NULL;
    }
    heap->data = malloc(sizeof(HeapNode) * capacity);
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swapHeapNodes(HeapNode* a, HeapNode* b)
{
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void shiftDown(MinHeap* heap, int index)
{
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->data[left].weight < heap->data[smallest].weight) {
        smallest = left;
    }
    if (right < heap->size && heap->data[right].weight < heap->data[smallest].weight) {
        smallest = right;
    }

    if (smallest != index) {
        swapHeapNodes(&heap->data[index], &heap->data[smallest]);
        shiftDown(heap, smallest);
    }
}

void shiftUp(MinHeap* heap, int index)
{
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[parent].weight > heap->data[index].weight) {
            swapHeapNodes(&heap->data[parent], &heap->data[index]);
            index = parent;
        } else {
            break;
        }
    }
}

void pushHeap(MinHeap* heap, int weight, int city, int stateID)
{
    if (heap->size == heap->capacity) {
        int newCapacity = heap->capacity * 2;
        HeapNode* temp = realloc(heap->data, sizeof(HeapNode) * newCapacity);
        if (!temp) {
            return;
        }
        heap->data = temp;
        heap->capacity = newCapacity;
    }
    heap->data[heap->size].weight = weight;
    heap->data[heap->size].city = city;
    heap->data[heap->size].stateID = stateID;
    shiftUp(heap, heap->size);
    heap->size++;
}

int popHeap(MinHeap* heap, HeapNode* outNode)
{
    if (heap->size == 0) {
        return 0;
    }
    *outNode = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    shiftDown(heap, 0);
    return 1;
}

void freeHeap(MinHeap* heap)
{
    if (heap) {
        free(heap->data);
        free(heap);
    }
}