#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Проверка является ли строка числом
bool isNumber(char *str);

// Разбиение строки на поля
char** splitLine(char line[], int *actualCols);

// Печать разделителей таблицы
void printSeparation(FILE *out, char ch, int *widths, int cols);

// Печать данных в таблицу
void printRow (FILE *out, char **row, int *widths, int cols, int isHeader);

// Освобождение данных
void freeData(char ***data, int count, int columns);

// Овобождение заголовка
void freeHeader(char **header, int columns);

// Чтение строк произвольной длины
char* readLine(FILE *file, size_t *len);