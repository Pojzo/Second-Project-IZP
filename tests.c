#include <stdio.h>
#include "setcal.h"

int main() {
    int tests = 0;
    for (int i = 0; i < NUM_SET_COMMANDS; i++) {
        if (is_command(set_commands[i])) {
            printf("Test %d: [OK]\n", tests++);
        }
        else {
            printf("Test %d: [FAIL]\n", tests++);
        }
    }

    for (int i = 0; i < NUM_REL_COMMANDS; i++) {
        if (is_command(rel_commands[i])) {
            printf("Test %d: [OK]\n", tests++);
        }
        else {
            printf("Test %d: [FAIL]\n", tests++);
        }
    }
}