#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STRINGS 20

int main() {
    const char *string = "U mojkokot tvojkokot jejkokot pizedolsimikokot";
    char *strings[10];
    int num_words = 0;
    int num_spaces = 0;
    char cur_word[30];
    int cur_index = 0;
    for (size_t i = 0; i < strlen(string); i++) {
        if (string[i] == ' ') {
            if (num_spaces == 1) {
                fprintf(stderr, "zle\n");
                return 1;
            }
            else {
                //strings = realloc(strings, (num_words + 1) * sizeof(char *));
                strings[num_words] = malloc(cur_index);
                memcpy(strings[num_words], cur_word, cur_index);
                ++num_words;
                num_spaces = 1;
                cur_index = 0;
            }
        }
        else {
            cur_word[cur_index] = string[i];
            ++cur_index;
            num_spaces = 0;
        }
    }
    if (num_spaces == 0) {
        //strings = realloc(strings, (num_words + 1) * sizeof(char *));
        strings[num_words] = malloc(cur_index);
        memcpy(strings[num_words], cur_word, cur_index);
        ++num_words;
    }
    else {
        fprintf(stderr, "Zle brasko\n");
        return 1;
    }

    for (int i = 0; i < num_words; i++) {
        printf("%s\n", strings[i]);
        free(strings[i]);
    }

    return 0;
}
