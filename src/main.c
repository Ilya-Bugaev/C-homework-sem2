#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    if (scanf("%d %d", &n, &m) != 2) {
        return 1;
    }

    adj = (EdgeNode**)calloc(n + 1, sizeof(EdgeNode*));
    owner = (int*)calloc(n + 1, sizeof(int));

    for (int i = 0; i < m; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        addEdge(u, v, w);
        addEdge(v, u, w);
    }

    if (scanf("%d", &k) != 1) {
        freeGraph();
        return 1;
    }

    int* capitals = (int*)malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++)
        scanf("%d", &capitals[i]);

    MinHeap* pq = createHeap(m * 2 + k + 10);

    for (int i = 0; i < k; i++) {
        int cap = capitals[i];
        int stateID = i + 1;
        if (owner[cap] == 0) {
            owner[cap] = stateID;
            EdgeNode* curr = adj[cap];
            while (curr) {
                if (owner[curr->to] == 0) {
                    pushHeap(pq, curr->weight, curr->to, stateID);
                }
                curr = curr->next;
            }
        }
    }
    free(capitals);

    HeapNode node;
    while (popHeap(pq, &node)) {
        int u = node.city;
        int stateID = node.stateID;
        if (owner[u] != 0) {
            continue;
        }
        owner[u] = stateID;
        EdgeNode* curr = adj[u];
        while (curr) {
            if (owner[curr->to] == 0) {
                pushHeap(pq, curr->weight, curr->to, stateID);
            }
            curr = curr->next;
        }
    }
    freeHeap(pq);

    int* counts = (int*)calloc(k + 1, sizeof(int));
    for (int i = 1; i <= n; i++) {
        if (owner[i] > 0 && owner[i] <= k) {
            counts[owner[i]]++;
        }
    }

    int** states = (int**)malloc(sizeof(int*) * (k + 1));
    for (int i = 1; i <= k; i++) {
        if (counts[i] > 0) {
            states[i] = (int*)malloc(sizeof(int) * counts[i]);
            counts[i] = 0;
        } else {
            states[i] = NULL;
        }
    }

    for (int i = 1; i <= n; i++) {
        int sid = owner[i];
        if (sid > 0 && sid <= k) {
            states[sid][counts[sid]++] = i;
        }
    }

    for (int i = 1; i <= k; i++) {
        printf("%d:", i);
        if (states[i]) {
            for (int j = 0; j < counts[i]; j++) {
                printf(" %d", states[i][j]);
            }
            free(states[i]);
        }
        printf("\n");
    }

    free(states);
    free(counts);
    freeGraph();
    return 0;
}