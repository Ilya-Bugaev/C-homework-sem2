#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// АТД перехода
typedef struct DFATransition {
    int fromState;
    char symbol;
    int toState;
} DFATransition;

// ДКА
typedef struct DFAt {
    int startState; // Номер стартового состояния
    int n; // Число возможных переходов
    DFATransition* transitions; // Массив длины n с переходами
    int m; // Число допускающих состояний
    int* acceptingStates; // Массив длины m допускающих состояний
    const char* alphabet; // Строка допустимых символов алфавита
} DFAt;

// Статусы работы
typedef enum {
    DFA_STATUS_OK, // Корректная работа
    DFA_STATUS_ERR_INVALID_SYMBOL, // Символ не входит в алфавит
    DFA_STATUS_ERR_NO_TRANSITION, // Отсутствует переход из текущего состояния
    DFA_STATUS_ERR_NULL_PARAM // Передан нулевой указатель
} DFAStatus;

bool dfaCheck(const DFAt* dfa, const char* input, DFAStatus* status);