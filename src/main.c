#include "graph.h"
#include "minHeap.h"
#include <stdio.h>
#include <stdlib.h>

// Чтение входных данных и построение неориентированного графа
static Graph* readAndBuildGraph(int* outTotalCities, int* outTotalRoads, int* outTotalStates)
{
    if (scanf("%d %d", outTotalCities, outTotalRoads) != 2) {
        return NULL;
    }

    Graph* graph = createGraph(*outTotalCities);
    if (!graph) {
        return NULL;
    }

    for (int roadIndex = 0; roadIndex < *outTotalRoads; roadIndex++) {
        int cityFrom = 0, cityTo = 0, roadLength = 0;
        if (scanf("%d %d %d", &cityFrom, &cityTo, &roadLength) != 3)
            break;
        graphAddEdge(graph, cityFrom, cityTo, roadLength);
        graphAddEdge(graph, cityTo, cityFrom, roadLength);
    }

    if (scanf("%d", outTotalStates) != 1) {
        freeGraph(graph);
        return NULL;
    }

    return graph;
}

// Инициализация столиц и заполнение кучи начальными рёбрами
static MinHeap* initializeCapitals(Graph* graph, int totalRoads, int totalStates)
{
    // Ёмкость кучи: каждое неориентированное ребро даёт 2 записи + k столиц
    MinHeap* heap = createMinHeap(totalRoads * 2 + totalStates);
    if (!heap) {
        return NULL;
    }

    for (int stateIndex = 0; stateIndex < totalStates; stateIndex++) {
        int capitalCity = 0;
        if (scanf("%d", &capitalCity) != 1)
            break;

        int stateId = stateIndex + 1;
        if (graph->cityOwner[capitalCity] == 0) {
            graph->cityOwner[capitalCity] = stateId;

            EdgeNode* edge = graph->adjacencyList[capitalCity];
            while (edge) {
                if (graph->cityOwner[edge->to] == 0) {
                    pushHeap(heap, edge->weight, edge->to, stateId);
                }
                edge = edge->next;
            }
        }
    }

    return heap;
}

// Многоисточниковый Дейкстра: распределение территорий
static void distributeTerritories(Graph* graph, MinHeap* heap)
{
    HeapNode extractedNode;
    while (popHeap(heap, &extractedNode)) {
        int currentCity = extractedNode.city;

        // если город захвачен, пропускаем
        if (graph->cityOwner[currentCity] != 0) {
            continue;
        }

        graph->cityOwner[currentCity] = extractedNode.stateID;

        // добавлtybt свободных соседей с накопительным расстоянием
        EdgeNode* edge = graph->adjacencyList[currentCity];
        while (edge) {
            if (graph->cityOwner[edge->to] == 0) {
                pushHeap(heap, extractedNode.weight + edge->weight, edge->to, extractedNode.stateID);
            }
            edge = edge->next;
        }
    }
}

// Сбор результатов, форматированный вывод и очистка локальных массивов
static void collectAndPrintResults(Graph* graph, int totalCities, int totalStates)
{
    int* citiesPerStateCount = calloc(totalStates + 1, sizeof(int));
    if (!citiesPerStateCount)
        return;

    for (int cityIndex = 1; cityIndex <= totalCities; cityIndex++) {
        int assignedState = graph->cityOwner[cityIndex];
        if (assignedState > 0 && assignedState <= totalStates) {
            citiesPerStateCount[assignedState]++;
        }
    }

    int** citiesByState = malloc(sizeof(int*) * (totalStates + 1));
    if (!citiesByState) {
        free(citiesPerStateCount);
        return;
    }

    // Выделяем точные массивы под каждое государство
    for (int stateIndex = 1; stateIndex <= totalStates; stateIndex++) {
        if (citiesPerStateCount[stateIndex] > 0) {
            citiesByState[stateIndex] = malloc(sizeof(int) * citiesPerStateCount[stateIndex]);
            if (!citiesByState[stateIndex]) {
                for (int j = 1; j < stateIndex; j++)
                    free(citiesByState[j]);
                free(citiesByState);
                free(citiesPerStateCount);
                return;
            }
            citiesPerStateCount[stateIndex] = 0; // Сбрасываем для использования как индекс
        } else {
            citiesByState[stateIndex] = NULL;
        }
    }

    // Заполняем массивы городами в порядке возрастания номеров
    for (int cityIndex = 1; cityIndex <= totalCities; cityIndex++) {
        int assignedState = graph->cityOwner[cityIndex];
        if (assignedState > 0 && assignedState <= totalStates) {
            citiesByState[assignedState][citiesPerStateCount[assignedState]++] = cityIndex;
        }
    }

    // Вывод
    for (int stateIndex = 1; stateIndex <= totalStates; stateIndex++) {
        printf("%d:", stateIndex);
        if (citiesByState[stateIndex]) {
            for (int cityIndex = 0; cityIndex < citiesPerStateCount[stateIndex]; cityIndex++) {
                printf(" %d", citiesByState[stateIndex][cityIndex]);
            }
        }
        printf("\n");
    }

    // Очистка локальных структур вывода
    for (int stateIndex = 1; stateIndex <= totalStates; stateIndex++) {
        free(citiesByState[stateIndex]);
    }
    free(citiesByState);
    free(citiesPerStateCount);
}

int main()
{
    int totalCities = 0, totalRoads = 0, totalStates = 0;

    // Ввод и построение графа
    Graph* graph = readAndBuildGraph(&totalCities, &totalRoads, &totalStates);
    if (!graph) {
        fprintf(stderr, "Ошибка: не удалось создать граф\n");
        return 1;
    }

    // Инициализация столиц и кучи
    MinHeap* heap = initializeCapitals(graph, totalRoads, totalStates);
    if (!heap) {
        fprintf(stderr, "Ошибка: не удалось создать кучу\n");
        freeGraph(graph);
        return 1;
    }

    distributeTerritories(graph, heap);

    collectAndPrintResults(graph, totalCities, totalStates);

    freeHeap(heap);
    freeGraph(graph);

    return 0;
}