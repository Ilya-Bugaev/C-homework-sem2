#include "avl.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AVLNode {
    char* code;
    char* name;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

struct AVLTree {
    AVLNode* root;
    int count;
};

// Обрезка пробелов и копирование строки
static char* trimAndDup(const char* s)
{
    if (!s) {
        return NULL;
    }
    while (*s && isspace((unsigned char)*s)) {
        s++;
    }
    const char* end = s + strlen(s);
    while (end > s && isspace((unsigned char)*(end - 1))) {
        end--;
    }
    size_t len = end - s;
    char* result = malloc(len + 1);
    if (result) {
        memcpy(result, s, len);
        result[len] = '\0';
    }
    return result;
}

// Вспомогательные функции AVL
static int nodeHeight(AVLNode* n) { return n ? n->height : 0; }
static int imax(int a, int b) { return a > b ? a : b; }

static void fixHeight(AVLNode* n)
{
    if (n) {
        n->height = 1 + imax(nodeHeight(n->left), nodeHeight(n->right));
    }
}

static int balanceFactor(AVLNode* n)
{
    return nodeHeight(n->right) - nodeHeight(n->left);
}

static AVLNode* rotateRight(AVLNode* p)
{
    AVLNode* q = p->left;
    p->left = q->right;
    q->right = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}

static AVLNode* rotateLeft(AVLNode* p)
{
    AVLNode* q = p->right;
    p->right = q->left;
    q->left = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}

static AVLNode* balance(AVLNode* p)
{
    fixHeight(p);
    if (balanceFactor(p) == 2) {
        if (balanceFactor(p->right) < 0) {
            p->right = rotateRight(p->right);
        }
        return rotateLeft(p);
    }
    if (balanceFactor(p) == -2) {
        if (balanceFactor(p->left) > 0) {
            p->left = rotateLeft(p->left);
        }
        return rotateRight(p);
    }
    return p;
}

static AVLNode* createNode(const char* code, const char* name)
{
    AVLNode* n = malloc(sizeof(AVLNode));
    if (!n) {
        return NULL;
    }
    n->code = trimAndDup(code);
    n->name = trimAndDup(name);

    if (!n->code || !n->name) {
        free(n->code);
        free(n->name);
        free(n);
        return NULL;
    }

    n->height = 1;
    n->left = n->right = NULL;
    return n;
}

static void freeNode(AVLNode* n)
{
    if (!n) {
        return;
    }
    free(n->code);
    free(n->name);
    free(n);
}

static AVLNode* insertNode(AVLNode* p, const char* code, const char* name, int* inserted)
{
    if (!p) {
        *inserted = 1;
        return createNode(code, name);
    }

    int cmp = strcmp(code, p->code);
    if (cmp < 0) {
        p->left = insertNode(p->left, code, name, inserted);
    } else if (cmp > 0) {
        p->right = insertNode(p->right, code, name, inserted);
    } else {
        // Обновление существующей записи
        char* newName = trimAndDup(name);
        if (newName) {
            free(p->name);
            p->name = newName;
        }
        *inserted = 0;
        return p;
    }

    return balance(p);
}

static AVLNode* findMin(AVLNode* p)
{
    return p->left ? findMin(p->left) : p;
}

static AVLNode* removeMin(AVLNode* p, int* deleted)
{
    if (!p->left) {
        AVLNode* right = p->right;
        freeNode(p);
        *deleted = 1;
        return right;
    }
    p->left = removeMin(p->left, deleted);
    return balance(p);
}

static AVLNode* deleteNode(AVLNode* p, const char* code, int* deleted)
{
    if (!p) {
        return NULL;
    }
    int cmp = strcmp(code, p->code);
    if (cmp < 0) {
        p->left = deleteNode(p->left, code, deleted);
    } else if (cmp > 0) {
        p->right = deleteNode(p->right, code, deleted);
    } else {
        AVLNode* left = p->left;
        AVLNode* right = p->right;

        if (!right) {
            freeNode(p);
            *deleted = 1;
            return left;
        }
        if (!left) {
            freeNode(p);
            *deleted = 1;
            return right;
        }

        // Два потомка: заменяем минимальным из правого поддерева
        AVLNode* min = findMin(right);
        char* newCode = trimAndDup(min->code);
        char* newName = trimAndDup(min->name);

        if (newCode && newName) {
            free(p->code);
            free(p->name);
            p->code = newCode;
            p->name = newName;
        } else {
            free(newCode);
            free(newName);
        }

        p->right = removeMin(right, deleted);
        return balance(p);
    }

    return balance(p);
}

static void saveInorder(AVLNode* p, FILE* f)
{
    if (!p) {
        return;
    }
    saveInorder(p->left, f);
    fprintf(f, "%s:%s\n", p->code, p->name);
    saveInorder(p->right, f);
}

static void freeTree(AVLNode* p)
{
    if (!p) {
        return;
    }
    freeTree(p->left);
    freeTree(p->right);
    freeNode(p);
}

// Публичные функции
AVLTree* avlTreeCreate(void)
{
    AVLTree* tree = malloc(sizeof(AVLTree));
    if (!tree) {
        return NULL;
    }
    tree->root = NULL;
    tree->count = 0;
    return tree;
}

void avlTreeFree(AVLTree* tree)
{
    if (!tree) {
        return;
    }
    freeTree(tree->root);
    free(tree);
}

void avlTreeInsert(AVLTree* tree, const char* code, const char* name)
{
    if (!tree || !code || !name) {
        return;
    }
    int inserted = 0;
    tree->root = insertNode(tree->root, code, name, &inserted);
    if (inserted) {
        tree->count++;
    }
}

const char* avlTreeFind(AVLTree* tree, const char* code)
{
    if (!tree || !code) {
        return NULL;
    }
    AVLNode* p = tree->root;
    while (p) {
        int cmp = strcmp(code, p->code);
        if (cmp == 0) {
            return p->name;
        }
        p = (cmp < 0) ? p->left : p->right;
    }
    return NULL;
}

int avlTreeDelete(AVLTree* tree, const char* code)
{
    if (!tree || !code) {
        return 0;
    }
    int deleted = 0;
    tree->root = deleteNode(tree->root, code, &deleted);
    if (deleted) {
        tree->count--;
    }
    return deleted;
}

void avlTreeSave(AVLTree* tree, const char* filename)
{
    if (!tree || !filename) {
        return;
    }
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("fopen");
        return;
    }
    saveInorder(tree->root, f);
    fclose(f);
}

int avlTreeSize(AVLTree* tree)
{
    return tree ? tree->count : 0;
}