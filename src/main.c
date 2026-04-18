#include "dfa.h"
#include <stdio.h>
#include <string.h>

#define MAX_TRANSITIONS 100

static void addTrans(DFATransition* arr, int* count, int from, char sym, int to)
{
    if (*count < MAX_TRANSITIONS) {
        arr[*count].fromState = from;
        arr[*count].symbol = sym;
        arr[*count].toState = to;
        (*count)++;
    }
}

int main()
{
    DFATransition transitions[MAX_TRANSITIONS];
    int n = 0;

    // 0 -> 1 (опциональный знак)
    addTrans(transitions, &n, 0, '+', 1);
    addTrans(transitions, &n, 0, '-', 1);
    // 0 -> 2 (начальные цифры)
    for (char c = '0'; c <= '9'; c++)
        addTrans(transitions, &n, 0, c, 2);
    // 0 -> 5 (точка без целой части)
    addTrans(transitions, &n, 0, '.', 5);

    // 1 -> 2 (цифры после знака)
    for (char c = '0'; c <= '9'; c++)
        addTrans(transitions, &n, 1, c, 2);
    // 1 -> 5 (точка после знака, формат "-.5")
    addTrans(transitions, &n, 1, '.', 5);

    // 2 -> 2 (продолжение целой части)
    for (char c = '0'; c <= '9'; c++)
        addTrans(transitions, &n, 2, c, 2);
    // 2 -> 3 (точка)
    addTrans(transitions, &n, 2, '.', 3);
    // 2 -> 7 (экспонента)
    addTrans(transitions, &n, 2, 'E', 7);
    addTrans(transitions, &n, 2, 'e', 7);

    // 3 -> 4 (цифры после точки)
    for (char c = '0'; c <= '9'; c++)
        addTrans(transitions, &n, 3, c, 4);
    // 3 -> 7 (экспонента после точки)
    addTrans(transitions, &n, 3, 'E', 7);
    addTrans(transitions, &n, 3, 'e', 7);

    // 4 -> 4 (продолжение дробной части)
    for (char c = '0'; c <= '9'; c++)
        addTrans(transitions, &n, 4, c, 4);
    // 4 -> 7 (экспонента)
    addTrans(transitions, &n, 4, 'E', 7);
    addTrans(transitions, &n, 4, 'e', 7);

    // 5 -> 6 (цифры после начальной точки)
    for (char c = '0'; c <= '9'; c++)
        addTrans(transitions, &n, 5, c, 6);

    // 6 -> 6 (продолжение дробной части без целой)
    for (char c = '0'; c <= '9'; c++)
        addTrans(transitions, &n, 6, c, 6);
    // 6 -> 7 (экспонента)
    addTrans(transitions, &n, 6, 'E', 7);
    addTrans(transitions, &n, 6, 'e', 7);

    // 7 -> 8 (знак экспоненты)
    addTrans(transitions, &n, 7, '+', 8);
    addTrans(transitions, &n, 7, '-', 8);
    // 7 -> 9 (цифры экспоненты без знака)
    for (char c = '0'; c <= '9'; c++)
        addTrans(transitions, &n, 7, c, 9);

    // 8 -> 9 (цифры после знака экспоненты)
    for (char c = '0'; c <= '9'; c++)
        addTrans(transitions, &n, 8, c, 9);

    // 9 -> 9 (продолжение цифр экспоненты)
    for (char c = '0'; c <= '9'; c++)
        addTrans(transitions, &n, 9, c, 9);

    int accepting_states[] = { 2, 3, 4, 6, 9 };
    int m = sizeof(accepting_states) / sizeof(accepting_states[0]);

    const char* alphabet = "0123456789.Ee+-";

    DFAt numberDFA = {
        .startState = 0,
        .n = n,
        .transitions = transitions,
        .m = m,
        .acceptingStates = accepting_states,
        .alphabet = alphabet
    };

    char input[256];
    printf("Введите строку: ");
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = '\0';

        DFAStatus status;
        bool isNumber = dfaCheck(&numberDFA, input, &status);

        if (status == DFA_STATUS_OK && isNumber) {
            printf("Это число!\n");
        } else {
            printf("Это не число :(\n");
        }
    }

    return 0;
}