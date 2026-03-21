#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int testsPassed = 0;
static int testsFailed = 0;

static void testCreateAndFree(void)
{
    printf("Тест testCreateAndFree:   ");

    AVLTree* tree = avlTreeCreate();
    if (tree == NULL) {
        printf("FAILED\n");
        testsFailed++;
        return;
    }

    if (avlTreeSize(tree) != 0) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    avlTreeFree(tree);
    printf("PASSED\n");
    testsPassed++;
}

static void testInsertAndFind(void)
{
    printf("Тест testInsertAndFind:   ");

    AVLTree* tree = avlTreeCreate();
    avlTreeInsert(tree, "SVO", "Sheremetyevo");
    avlTreeInsert(tree, "JFK", "John F Kennedy");

    if (avlTreeSize(tree) != 2) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    const char* name = avlTreeFind(tree, "SVO");
    if (name == NULL || strcmp(name, "Sheremetyevo") != 0) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    avlTreeFree(tree);
    printf("PASSED\n");
    testsPassed++;
}

static void testUpdateExisting(void)
{
    printf("Тест testUpdateExisting:   ");

    AVLTree* tree = avlTreeCreate();
    avlTreeInsert(tree, "SVO", "Sheremetyevo");
    avlTreeInsert(tree, "SVO", "Sheremetyevo Updated");

    if (avlTreeSize(tree) != 1) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    const char* name = avlTreeFind(tree, "SVO");
    if (strcmp(name, "Sheremetyevo Updated") != 0) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    avlTreeFree(tree);
    printf("PASSED\n");
    testsPassed++;
}

static void testDelete(void)
{
    printf("Тест testDelete:   ");

    AVLTree* tree = avlTreeCreate();
    avlTreeInsert(tree, "SVO", "Sheremetyevo");
    avlTreeInsert(tree, "JFK", "John F Kennedy");

    if (avlTreeDelete(tree, "JFK") != 1) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    if (avlTreeSize(tree) != 1) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    avlTreeFree(tree);
    printf("PASSED\n");
    testsPassed++;
}

static void testSave(void)
{
    printf("Тест testSave:   ");

    AVLTree* tree = avlTreeCreate();
    avlTreeInsert(tree, "AAA", "Airport A");
    avlTreeInsert(tree, "BBB", "Airport B");

    avlTreeSave(tree, "test_output.txt");

    FILE* f = fopen("test_output.txt", "r");
    if (f == NULL) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), f))
        count++;
    fclose(f);

    if (count != 2) {
        printf("FAILED\n");
        testsFailed++;
        remove("test_output.txt");
        avlTreeFree(tree);
        return;
    }

    remove("test_output.txt");
    avlTreeFree(tree);
    printf("PASSED\n");
    testsPassed++;
}

static void testNullHandling(void)
{
    printf("Тест testNullHandling:   ");

    AVLTree* tree = avlTreeCreate();

    avlTreeInsert(NULL, "SVO", "Test");
    avlTreeInsert(tree, NULL, "Test");
    avlTreeInsert(tree, "SVO", NULL);

    if (avlTreeFind(NULL, "SVO") != NULL) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    if (avlTreeFind(tree, NULL) != NULL) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    if (avlTreeDelete(NULL, "SVO") != 0) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    avlTreeSave(NULL, "test.txt");
    avlTreeSave(tree, NULL);
    avlTreeFree(NULL);
    avlTreeFree(tree);

    printf("PASSED\n");
    testsPassed++;
}

static void testLargeInsert(void)
{
    printf("Тест testLargeInsert:   ");

    AVLTree* tree = avlTreeCreate();

    for (int i = 0; i < 100; i++) {
        char code[8];
        char name[32];
        snprintf(code, sizeof(code), "%03d", i);
        snprintf(name, sizeof(name), "Airport %d", i);
        avlTreeInsert(tree, code, name);
    }

    if (avlTreeSize(tree) != 100) {
        printf("FAILED\n");
        testsFailed++;
        avlTreeFree(tree);
        return;
    }

    for (int i = 0; i < 100; i++) {
        char code[8];
        snprintf(code, sizeof(code), "%03d", i);
        if (avlTreeFind(tree, code) == NULL) {
            printf("FAILED\n");
            testsFailed++;
            avlTreeFree(tree);
            return;
        }
    }

    avlTreeFree(tree);
    printf("PASSED\n");
    testsPassed++;
}

int main()
{
    testCreateAndFree();
    testInsertAndFind();
    testUpdateExisting();
    testDelete();
    testSave();
    testNullHandling();
    testLargeInsert();

    printf("\nResults:\n");
    printf("Passed: %d\n", testsPassed);
    printf("Failed: %d\n", testsFailed);
    printf("Total: %d\n", testsPassed + testsFailed);

    return testsFailed > 0 ? 1 : 0;
}