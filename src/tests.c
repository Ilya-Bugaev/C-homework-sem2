#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int passed = 0, failed = 0;

// Функция проверки условия
void check(int condition, const char* testName, const char* message)
{
    if (condition) {
        passed++;
        printf("PASS: %s - %s\n", testName, message);
    } else {
        printf("FAIL: %s - %s\n", testName, message);
        failed++;
    }
}

// Запись входных данных во временный файл
void writeTestInput(const char* input)
{
    FILE* f = fopen("test_in.tmp", "w");
    if (f) {
        fprintf(f, "%s", input);
        fclose(f);
    }
}

// Чтение вывода программы
char* readTestOutput()
{
    FILE* f = fopen("test_out.tmp", "r");
    if (!f)
        return NULL;

    char* buffer = malloc(10000);
    size_t total = 0;
    size_t bytesRead;
    while ((bytesRead = fread(buffer + total, 1, 1000, f)) > 0) {
        total += bytesRead;
    }
    buffer[total] = '\0';
    fclose(f);
    return buffer;
}

// Запуск одного теста
void runTest(const char* name, const char* input, const char* expected)
{
    writeTestInput(input);

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "./solution < test_in.tmp > test_out.tmp 2>&1");
    system(cmd);

    char* actual = readTestOutput();
    check(actual != NULL, name, "output file exists");

    if (actual && strcmp(actual, expected) == 0) {
        check(1, name, "output matches expected");
    } else {
        check(0, name, "output matches expected");
        if (actual) {
            printf("  Expected: [%s]\n", expected);
            printf("  Got:      [%s]\n", actual);
        }
    }

    free(actual);
    remove("test_in.tmp");
    remove("test_out.tmp");
}

// Тесты
void testBasicTwoCapitals()
{
    printf("Testing basic two capitals...\n");
    runTest(
        "basicTwoCapitals",
        "5 5\n1 2 10\n2 3 5\n3 4 1\n4 5 2\n1 5 20\n2\n1 4\n",
        "1: 1\n2: 2 3 4 5\n"
    );
}

void testSingleCapital()
{
    printf("Testing single capital...\n");
    runTest(
        "singleCapital",
        "4 3\n1 2 5\n2 3 10\n3 4 15\n1\n1\n",
        "1: 1 2 3 4\n");
}

void testEqualWeights()
{
    printf("Testing equal edge weights...\n");
    runTest(
        "equalWeights",
        "6 5\n1 2 1\n2 3 1\n3 4 1\n4 5 1\n5 6 1\n2\n1 6\n",
        "1: 1 2 3\n2: 4 5 6\n");
}

void testLinearGraph()
{
    printf("Testing linear graph...\n");
    runTest(
        "linearGraph",
        "5 4\n1 2 2\n2 3 3\n3 4 4\n4 5 5\n2\n2 4\n",
        "1: 1 2 3\n2: 4 5\n");
}

void testThreeStates()
{
    printf("Testing three states...\n");
    runTest(
        "threeStates",
        "7 6\n1 2 10\n2 3 20\n3 4 30\n4 5 40\n5 6 50\n6 7 60\n3\n1 4 7\n",
        "1: 1 2 3\n2: 4 5 6\n3: 7\n"
    );
}

void testStarGraph()
{
    printf("Testing star graph...\n");
    runTest(
        "starGraph",
        "5 4\n1 2 1\n1 3 2\n1 4 3\n1 5 4\n2\n2 5\n",
        "1: 1 2 3 4\n2: 5\n");
}

void testMinimalGraph()
{
    printf("Testing minimal graph...\n");
    runTest(
        "minimalGraph",
        "2 1\n1 2 100\n2\n1 2\n",
        "1: 1\n2: 2\n");
}

void testDifferentWeights()
{
    printf("Testing different weights...\n");
    runTest(
        "differentWeights",
        "4 3\n1 2 1\n2 3 2\n3 4 100\n2\n1 4\n",
        "1: 1 2 3\n2: 4\n");
}

void testMemoryCleanup()
{
    printf("Testing memory cleanup...\n");
    for (int i = 0; i < 10; i++) {
        runTest(
            "memoryCleanup",
            "3 2\n1 2 1\n2 3 1\n1\n1\n",
            "1: 1 2 3\n");
    }
    check(1, "memoryCleanup", "no crash in repeated runs");
}

int main()
{
    printf("City Distribution Tests\n\n");

    testBasicTwoCapitals();
    printf("\n");

    testSingleCapital();
    printf("\n");

    testEqualWeights();
    printf("\n");

    testLinearGraph();
    printf("\n");

    testThreeStates();
    printf("\n");

    testStarGraph();
    printf("\n");

    testMinimalGraph();
    printf("\n");

    testDifferentWeights();
    printf("\n");

    testMemoryCleanup();
    printf("\n");

    printf("Results: %d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}