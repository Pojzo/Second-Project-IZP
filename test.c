#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STRINGS 20

void empty_string(char *string) {
    for (size_t i = 0; i < strlen(string); i++) {
    }
}

int main() {
    const char *string = "U mojkokot tvojkokot jejkokot pizedolsimikokot picifdasfsd";
    char **strings = (char **) malloc(MAX_STRINGS * sizeof(char*));
    int n_strings = 0;
    int cur_index = 0;
    char cur_string[30];
    int num_spaces = 0;
    for (size_t i = 0; i < strlen(string); i++) {
        if (string[i] == ' ') {
            if (num_spaces == 1) {
                fprintf(stderr, "Dve medzery su tam.\n");
                return 1;
            }
            ++num_spaces;
            continue;
        }
        else {
            if (num_spaces == 1)  {
                strings[n_strings] = (char *)malloc(cur_index + 1);
                memcpy(strings[n_stri
                num_spaces = 0;
                cur_index = 0;
            }
            else {
                cur_string[cur_index] = string[i];
                ++cur_index;
            }
        }
    }
    for (int i = 0; i < n_strings; i++) {
        printf("%d. %s\n", (i + 1), strings[i]);
    }

    return 0;
}