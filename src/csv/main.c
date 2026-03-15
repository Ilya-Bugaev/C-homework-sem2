#include "csv.h"
#include <stdlib.h>

#define MAXLINE 4096

int main()
{
    FILE* in = fopen("input.csv", "r");
    if (!in) {
        fprintf(stderr, "Cannot open input file input.csv\n");
        return 1;
    }

    FILE* out = fopen("output.txt", "w");
    if (!out) {
        fprintf(stderr, "Cannot open output file output.txt");
        fclose(in);
        return 1;
    }

    char* line = NULL;
    size_t len = 0;

    line = readLine(in, &len);
    if (!line) {
        fprintf(stderr, "Input file is empty\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    int columns;
    char** headerFields = splitLine(line, &columns); // Массив указателей на первый элемент поля таблицы

    if (!headerFields) {
        fprintf(stderr, "Failed to parse header\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    char** header = malloc(columns * sizeof(char*));
    if (!header) {
        fprintf(stderr, "Memory allocation error\n");
        free(headerFields);
        fclose(in);
        fclose(out);
        return 1;
    }

    for (int i = 0; i < columns; i++) {
        header[i] = strdup(headerFields[i]);
        if (!header[i]) {
            fprintf(stderr, "Memory allocation error\n");
            // Очищаем то, что успели выделить
            for (int j = 0; j < i; j++)
                free(header[j]);
            free(header);
            free(headerFields);
            fclose(in);
            fclose(out);
            return 1;
        }
    }
    free(line);
    free(headerFields);

    int capacity = 10;
    char*** data = malloc(capacity * sizeof(char**)); // Указатель на строки таблицы
    if (!data) {
        fprintf(stderr, "Memory allocation error\n");
        freeHeader(header, columns);
        fclose(in);
        fclose(out);
        return 1;
    }

    int count = 0;
    int actualCols;

    while ((line = readLine(in, &len)) != NULL) {
        char** fields = splitLine(line, &actualCols);
        if (!fields) {
            free(line);
            continue;
        }

        char** row = malloc(columns * sizeof(char*));
        if (!row) {
            fprintf(stderr, "Memory allocation error\n");
            free(fields);
            // Ошибка при выделении строки: очищаем всё и выходим
            freeData(data, count, columns);
            freeHeader(header, columns);
            fclose(in);
            fclose(out);
            return 1;
        }

        // Заполнение полей строки
        for (int i = 0; i < columns; i++) {
            if (i < actualCols) {
                row[i] = strdup(fields[i]);
            } else {
                row[i] = strdup("");
            }

            if (!row[i]) {
                fprintf(stderr, "Memory allocation error\n");
                free(fields);
                // Очищаем текущую строку
                for (int j = 0; j < i; j++)
                    free(row[j]);
                free(row);
                // Очищаем всё остальное
                freeData(data, count, columns);
                freeHeader(header, columns);
                fclose(in);
                fclose(out);
                return 1;
            }
        }
        free(line);
        free(fields);

        // Увеличение массива данных при необходимости
        if (count == capacity) {
            capacity *= 2;
            char*** newData = realloc(data, capacity * sizeof(char**));
            if (!newData) {
                fprintf(stderr, "Memory allocation error\n");
                // Очищаем текущую строку, так как она еще не добавлена в data
                for (int i = 0; i < columns; i++)
                    free(row[i]);
                free(row);
                freeData(data, count, columns);
                freeHeader(header, columns);
                fclose(in);
                fclose(out);
                return 1;
            }
            data = newData;
        }
        data[count++] = row;
    }

    // Выделение памяти под ширины колонок
    int* widths = malloc(columns * sizeof(int));
    if (!widths) {
        fprintf(stderr, "Memory allocation error\n");
        freeData(data, count, columns);
        freeHeader(header, columns);
        fclose(in);
        fclose(out);
        return 1;
    }

    // Вычисление максимальной ширины для каждой колонки
    for (int i = 0; i < columns; i++) {
        widths[i] = strlen(header[i]);
    }

    for (int r = 0; r < count; r++) {
        for (int c = 0; c < columns; c++) {
            int len = strlen(data[r][c]);
            if (len > widths[c])
                widths[c] = len;
        }
    }

    // Верхняя граница
    printSeparation(out, '=', widths, columns);
    // Заголовок
    printRow(out, header, widths, columns, 1);
    // Разделитель после заголовка
    printSeparation(out, '=', widths, columns);
    // Строки данных
    for (int r = 0; r < count; r++) {
        printRow(out, data[r], widths, columns, 0);
        if (r < count - 1) {
            printSeparation(out, '-', widths, columns);
        }
    }
    // Закрытие таблицы
    printSeparation(out, '-', widths, columns);

    free(widths);
    freeData(data, count, columns);
    freeHeader(header, columns);
    fclose(in);
    fclose(out);

    return 0;
}