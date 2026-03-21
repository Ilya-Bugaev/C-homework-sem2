#pragma once

typedef struct AVLTree AVLTree;

// Создание дерева
AVLTree* avlTreeCreate(void);

// Удаление дерева и освобождение памяти
void avlTreeFree(AVLTree* tree);

// Вставка или обновление записи
void avlTreeInsert(AVLTree* tree, const char* code, const char* name);

// Поиск названия по коду, возвращает NULL если не найдено
const char* avlTreeFind(AVLTree* tree, const char* code);

// Удаление записи, возвращает 1 если удалено, 0 если не найдено
int avlTreeDelete(AVLTree* tree, const char* code);

// Сохранить все записи в файл в формате "код:название" (отсортировано по коду)
void avlTreeSave(AVLTree* tree, const char* filename);

// Количество записей в дереве
int avlTreeSize(AVLTree* tree);