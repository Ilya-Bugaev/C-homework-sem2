#include "avl.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Обрезка пробелов и копирование в буфер фиксированного размера
static void trimCopy(char* dest, const char* src, size_t dest_size)
{
    if (dest_size == 0) {
        return;
    }
    while (*src && isspace((unsigned char)*src)) {
        src++;
    }
    const char* end = src + strlen(src);
    while (end > src && isspace((unsigned char)*(end - 1))) {
        end--;
    }
    size_t len = end - src;
    if (len >= dest_size) {
        len = dest_size - 1;
    }
    memcpy(dest, src, len);
    dest[len] = '\0';
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <airports.txt>\n", argv[0]);
        return 1;
    }

    FILE* fin = fopen(argv[1], "r");
    if (!fin) {
        perror("fopen");
        return 1;
    }

    AVLTree* tree = avlTreeCreate();
    if (!tree) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(fin);
        return 1;
    }

    // Загрузка данных из файла
    char line[1024];
    int loaded = 0;
    while (fgets(line, sizeof(line), fin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        char* colon = strchr(line, ':');
        if (!colon) {
            continue; // Пропускаем неверные строки
        }

        *colon = '\0';
        char* code = line;
        char* name = colon + 1;

        // Обрезаем пробелы
        while (*code && isspace((unsigned char)*code)) {
            code++;
        }
        while (*name && isspace((unsigned char)*name)) {
            name++;
        }

        if (strlen(code) == 0) {
            continue;
        }

        avlTreeInsert(tree, code, name);
        loaded++;
    }
    fclose(fin);

    printf("Загружено %d аэропортов. Система готова к работе.\n", avlTreeSize(tree));

    // Интерактивный цикл
    char buffer[1024];
    while (1) {
        printf("> ");
        fflush(stdout);

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            break;
        }

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Пропускаем пустые строки
        char* p = buffer;
        while (*p && isspace((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0') {
            continue;
        }

        char* cmd = strtok(buffer, " \t");
        if (!cmd) {
            continue;
        }

        if (strcmp(cmd, "quit") == 0) {
            break;
        } else if (strcmp(cmd, "save") == 0) {
            avlTreeSave(tree, argv[1]);
            printf("База сохранена: %d аэропортов.\n", avlTreeSize(tree));
        } else if (strcmp(cmd, "find") == 0) {
            char* arg = strtok(NULL, " \t\n");
            if (!arg) {
                printf("Использование: find <код>\n");
                continue;
            }
            char code[16];
            trimCopy(code, arg, sizeof(code));
            if (strlen(code) == 0) {
                printf("Пустой код.\n");
                continue;
            }
            const char* name = avlTreeFind(tree, code);
            if (name)
                printf("%s → %s\n", code, name);
            else
                printf("Аэропорт с кодом '%s' не найден в базе.\n", code);
        } else if (strcmp(cmd, "delete") == 0) {
            char* arg = strtok(NULL, " \t\n");
            if (!arg) {
                printf("Использование: delete <код>\n");
                continue;
            }
            char code[16];
            trimCopy(code, arg, sizeof(code));
            if (strlen(code) == 0) {
                printf("Пустой код.\n");
                continue;
            }
            if (avlTreeDelete(tree, code))
                printf("Аэропорт '%s' удалён из базы.\n", code);
            else
                printf("Аэропорт с кодом '%s' не найден в базе.\n", code);
        } else if (strcmp(cmd, "add") == 0) {
            char* arg = strtok(NULL, " \t\n");
            if (!arg) {
                printf("Использование: add <код>:<название>\n");
                continue;
            }
            char* colon = strchr(arg, ':');
            if (!colon) {
                printf("Неверный формат. Ожидается: <код>:<название>\n");
                continue;
            }
            *colon = '\0';
            char* codePart = arg;
            char* namePart = colon + 1;

            char code[16];
            trimCopy(code, codePart, sizeof(code));
            if (strlen(code) == 0) {
                printf("Код не может быть пустым.\n");
                continue;
            }
            // Обрезаем пробелы в названии
            while (*namePart && isspace((unsigned char)*namePart)) {
                namePart++;
            }

            avlTreeInsert(tree, code, namePart);
            printf("Аэропорт '%s' добавлен в базу.\n", code);
        } else {
            printf("Неизвестная команда '%s'. Доступны: find, add, delete, save, quit\n", cmd);
        }
    }

    avlTreeFree(tree);
    return 0;
}