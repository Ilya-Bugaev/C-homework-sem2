#include "csv.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int passed = 0, failed = 0;

void check(int condition, const char* test_name, const char* message)
{
    if (condition) {
        passed++;
    } else {
        printf("FAIL: %s - %s\n", test_name, message);
        failed++;
    }
}

void test_isNumber()
{
    printf("Testing isNumber()...\n");
    check(isNumber("123") == true, "isNumber", "positive int");
    check(isNumber("3.14") == true, "isNumber", "float");
    check(isNumber("-50") == true, "isNumber", "negative");
    check(isNumber("abc") == false, "isNumber", "text");
    check(isNumber("") == false, "isNumber", "empty");
    check(isNumber(NULL) == false, "isNumber", "NULL");
}

void test_splitLine()
{
    printf("Testing splitLine()...\n");

    char line1[] = "a,b,c";
    int cols;
    char** f = splitLine(line1, &cols);
    check(f != NULL, "splitLine", "not NULL");
    check(cols == 3, "splitLine", "3 columns");
    check(strcmp(f[0], "a") == 0, "splitLine", "field 0");
    check(strcmp(f[1], "b") == 0, "splitLine", "field 1");
    check(strcmp(f[2], "c") == 0, "splitLine", "field 2");
    free(f);

    char line2[] = "x,,z";
    f = splitLine(line2, &cols);
    check(cols == 3, "splitLine", "empty field count");
    check(strcmp(f[1], "") == 0, "splitLine", "empty field");
    free(f);

    char line3[] = "f1,f2\n";
    f = splitLine(line3, &cols);
    check(strcmp(f[1], "f2") == 0, "splitLine", "newline removed");
    free(f);
}

void test_readLine()
{
    printf("Testing readLine()...\n");

    FILE* tmp = tmpfile();
    check(tmp != NULL, "readLine", "tmpfile created");

    fprintf(tmp, "hello");
    rewind(tmp);
    char* line = readLine(tmp, NULL);
    check(strcmp(line, "hello") == 0, "readLine", "simple line");
    free(line);

    rewind(tmp);
    fprintf(tmp, "test\n");
    rewind(tmp);
    line = readLine(tmp, NULL);
    check(strcmp(line, "test") == 0, "readLine", "with newline");
    free(line);

    fclose(tmp);

    // Длинная строка
    tmp = tmpfile();
    for (int i = 0; i < 300; i++)
        fputc('A', tmp);
    fputc('\n', tmp);
    rewind(tmp);
    line = readLine(tmp, NULL);
    check(strlen(line) == 300, "readLine", "long line");
    free(line);
    fclose(tmp);
}

void test_integration()
{
    printf("Testing integration...\n");

    FILE* tmp = tmpfile();
    fprintf(tmp, "Name,Age\n");
    fprintf(tmp, "John,25\n");
    fprintf(tmp, "Ann,30\n");
    rewind(tmp);

    char* line = readLine(tmp, NULL);
    int cols;
    char** h = splitLine(line, &cols);
    check(cols == 2, "integration", "header columns");
    free(line);
    free(h);

    int rows = 0;
    while ((line = readLine(tmp, NULL)) != NULL) {
        char** f = splitLine(line, &cols);
        free(line);
        free(f);
        rows++;
    }
    check(rows == 2, "integration", "data rows");
    fclose(tmp);
}

void test_memory()
{
    printf("Testing memory...\n");

    for (int i = 0; i < 50; i++) {
        char line[] = "1,2,3";
        int cols;
        char** f = splitLine(line, &cols);
        free(f);
    }

    FILE* tmp = tmpfile();
    for (int i = 0; i < 50; i++)
        fprintf(tmp, "line\n");
    rewind(tmp);
    char* line;
    while ((line = readLine(tmp, NULL)) != NULL)
        free(line);
    fclose(tmp);

    check(1, "memory", "no crash in loop");
}

int main(void)
{
    printf("=== CSV Parser Tests ===\n\n");

    test_isNumber();
    printf("\n");

    test_splitLine();
    printf("\n");

    test_readLine();
    printf("\n");

    test_integration();
    printf("\n");

    test_memory();
    printf("\n");

    printf("=== Results: %d passed, %d failed ===\n", passed, failed);
    return failed > 0 ? 1 : 0;
}