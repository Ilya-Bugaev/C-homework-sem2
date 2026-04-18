#include "dfa.h"
#include <string.h>

static bool isValidSymbol(const char* alphabet, char c)
{
    if (!alphabet) {
        return false;
    }
    return strchr(alphabet, c) != NULL;
}

bool dfaCheck(const DFAt* dfa, const char* input, DFAStatus* status)
{
    if (!status) {
        return false;
    }
    if (!dfa || !input) {
        *status = DFA_STATUS_ERR_NULL_PARAM;
        return false;
    }

    int current_state = dfa->startState;
    const char* ptr = input;

    while (*ptr != '\0') {
        char sym = *ptr;

        if (!isValidSymbol(dfa->alphabet, sym)) {
            *status = DFA_STATUS_ERR_INVALID_SYMBOL;
            return false;
        }

        bool transitionFound = false;
        for (int i = 0; i < dfa->n; ++i) {
            if (dfa->transitions[i].fromState == current_state && dfa->transitions[i].symbol == sym) {
                current_state = dfa->transitions[i].toState;
                transitionFound = true;
                break;
            }
        }

        if (!transitionFound) {
            *status = DFA_STATUS_ERR_NO_TRANSITION;
            return false;
        }

        ++ptr;
    }

    *status = DFA_STATUS_OK;
    for (int i = 0; i < dfa->m; ++i) {
        if (dfa->acceptingStates[i] == current_state) {
            return true;
        }
    }

    return false;
}