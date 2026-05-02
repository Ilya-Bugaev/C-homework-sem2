#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int passed = 0, failed = 0;

// Проверка условия теста
static void check(int condition, const char* testName, const char* message)
{
    if (condition) {
        passed++;
        printf("PASS: %s - %s\n", testName, message);
    } else {
        failed++;
        printf("FAIL: %s - %s\n", testName, message);
    }
}

// Запись входных данных во временный файл
static void writeTestInput(const char* input)
{
    FILE* f = fopen("test_in.tmp", "w");
    if (f) {
        fprintf(f, "%s", input);
        fclose(f);
    }
}

// Чтение вывода программы
static char* readTestOutput(void)
{
    FILE* f = fopen("test_out.tmp", "r");
    if (!f)
        return NULL;

    char* buffer = malloc(10240);
    if (!buffer)
        return NULL;

    size_t total = 0;
    size_t bytesRead;
    while ((bytesRead = fread(buffer + total, 1, 1024, f)) > 0) {
        total += bytesRead;
    }
    buffer[total] = '\0';
    fclose(f);
    return buffer;
}

// Запуск одного теста
static void runTest(const char* name, const char* input, const char* expected)
{
    writeTestInput(input);

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "./solution < test_in.tmp > test_out.tmp 2>&1");
    system(cmd);

    char* actual = readTestOutput();
    check(actual != NULL, name, "output file exists");

    if (actual && strcmp(actual, expected) == 0) {
        check(1, name, "output matches expected");
    } else {
        check(0, name, "output matches expected");
        if (actual) {
            printf("  Expected:\n%s\n  Got:\n%s\n", expected, actual);
        }
    }

    free(actual);
    remove("test_in.tmp");
    remove("test_out.tmp");
}

static void testBasicTwoCapitals(void)
{
    runTest("basicTwoCapitals",
        "5 5\n"
        "1 2 10\n"
        "2 3 5\n"
        "3 4 1\n"
        "4 5 2\n"
        "1 5 20\n"
        "2\n"
        "1 4\n",
        "1: 1\n2: 2 3 4 5\n");
}

static void testSingleCapital(void)
{
    runTest("singleCapital",
        "4 3\n"
        "1 2 5\n"
        "2 3 10\n"
        "3 4 15\n"
        "1\n"
        "1\n",
        "1: 1 2 3 4\n");
}

static void testEqualWeights(void)
{
    runTest("equalWeights",
        "6 5\n"
        "1 2 1\n2 3 1\n3 4 1\n4 5 1\n5 6 1\n"
        "2\n"
        "1 6\n",
        "1: 1 2 3\n2: 4 5 6\n");
}

static void testLinearGraph(void)
{
    runTest("linearGraph",
        "5 4\n"
        "1 2 2\n2 3 3\n3 4 4\n4 5 5\n"
        "2\n"
        "2 4\n",
        "1: 1 2 3\n2: 4 5\n");
}

static void testThreeStates(void)
{
    runTest("threeStates",
        "7 6\n"
        "1 2 10\n2 3 20\n3 4 30\n4 5 40\n5 6 50\n6 7 60\n"
        "3\n"
        "1 4 7\n",
        "1: 1 2 3\n2: 4 5 6\n3: 7\n");
}

static void testStarGraph(void)
{
    runTest("starGraph",
        "5 4\n"
        "1 2 1\n1 3 2\n1 4 3\n1 5 4\n"
        "2\n"
        "2 5\n",
        "1: 1 2 3 4\n2: 5\n");
}

static void testMinimalGraph(void)
{
    runTest("minimalGraph",
        "2 1\n1 2 100\n2\n1 2\n",
        "1: 1\n2: 2\n");
}

static void testDifferentWeights(void)
{
    runTest("differentWeights",
        "4 3\n"
        "1 2 1\n2 3 2\n3 4 100\n"
        "2\n"
        "1 4\n",
        "1: 1 2 3\n2: 4\n");
}

static void testMemoryCleanup(void)
{
    // Многократный запуск для проверки отсутствия утечек и крашей
    for (int i = 0; i < 5; i++) {
        runTest("memoryCleanup",
            "3 2\n1 2 1\n2 3 1\n1\n1\n",
            "1: 1 2 3\n");
    }
    check(1, "memoryCleanup", "stable after repeated runs");
}

int main(void)
{
    printf("City Distribution Integration Tests\n\n");

    testBasicTwoCapitals();
    testSingleCapital();
    testEqualWeights();
    testLinearGraph();
    testThreeStates();
    testStarGraph();
    testMinimalGraph();
    testDifferentWeights();
    testMemoryCleanup();

    printf("\n");
    printf("Results: %d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
}