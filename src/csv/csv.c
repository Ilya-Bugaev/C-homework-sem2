#include "csv.h"

bool isNumber(char *str){
    if (str == NULL) {
        return false;
    }

    // Провека на пустую строку и строку из пробелов
    char *start = str;
    while (*start && isspace((unsigned  char)*start)) {
        start++;
    }
    if (*start == '\0') {
        return false;
    }

    char *endptr;
    strtod(start, &endptr);
    if (start == endptr) {
        return false;
    }
    while (*endptr != '\0' && isspace((unsigned char)*endptr)) {
        endptr++;
    }
    return *endptr == '\0';
}


char** splitLine(char line[], int *actualCols) //Строка, которую делим и указатель на число полей. Возвращаем 
{
    if (line == NULL || actualCols == NULL) {
        return NULL;
    }

    int count = 1;
    for (char *p = line; *p; p++) {
        if (*p == ',') {
            count++;
        }
    }

    *actualCols = count;
    char **fields = malloc(sizeof(char*) * count);
    if (fields == NULL) {
        return NULL;
    }

    char *p = line;
    char *start = line;
    int idx = 0;
    while(*p && *p != '\n') {
        if (*p == ',') {
            *p = '\0';
            fields[idx] = start;
            start = p + 1;
            idx++;
        }
        p++;
    }
    fields[idx] = start;

    // Если встретили '\n', заменяем его на '\0', чтобы завершить строку
    if (*p == '\n') {
        *p = '\0';
    }

    return fields;
}

void printSeparation(FILE *out, char ch, int *widths, int cols)
{
    fputc('+', out);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < widths[i] + 2; j++) {
            fputc(ch, out);
        }
        if (i < cols - 1) {
            fputc('+', out);
        }
    }
    fputc('+', out);
    fputc('\n', out);
}

void printRow (FILE *out, char **row, int *widths, int cols, int isHeader)
{
    fputc('|', out);
    for (int i = 0; i < cols; i++) {
        if (isHeader || !isNumber(row[i])) {
            fprintf(out, " %-*s ", widths[i], row[i]); // выравнивание влево
        } else {
            fprintf(out, " %*s ", widths[i], row[i]);  // выравнивание вправо (числа)
        }
        if (i < cols - 1) {
            fputc('|', out);
        }
    }
    fputc('|', out);
    fputc('\n', out);
}

void freeData(char ***data, int count, int columns)
{
    if (!data) return;
    for (int i = 0; i < count; i++) {
        if (data[i]) {
            for (int j = 0; j < columns; j++) {
                if (data[i][j]) free(data[i][j]);
            }
            free(data[i]);
        }
    }
    free(data);
}

void freeHeader(char **header, int columns)
{
    if (!header) return;
    for (int i = 0; i < columns; i++) {
        if (header[i]) free(header[i]);
    }
    free(header);
}

char* readLine(FILE *file, size_t *len)
{
    size_t capacity = 256;
    size_t size = 0;
    char *buffer = malloc(capacity);
    if (!buffer) return NULL;
    
    int c;
    while ((c = fgetc(file)) != EOF && c != '\n') {
        if (size + 1 >= capacity) {
            capacity *= 2;
            char *newBuffer = realloc(buffer, capacity);
            if (!newBuffer) {
                free(buffer);
                return NULL;
            }
            buffer = newBuffer;
        }
        buffer[size++] = c;
    }
    
    if (size == 0 && c == EOF) {
        free(buffer);
        return NULL;
    }
    
    buffer[size] = '\0';
    if (len) *len = size;
    return buffer;
}