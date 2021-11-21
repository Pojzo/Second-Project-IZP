#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STRINGS 20

void split_string(char ***words, const char *string, int *num_words)  {
    *num_words = 0;
    int num_spaces = 0;
    char cur_word[30];
    int cur_index = 0;
    for (size_t i = 0; i < strlen(string); i++) {
        if (string[i] == ' ') {
            if (num_spaces == 1) {
                fprintf(stderr, "zle\n");
                return;
            }
            else {
                *words = realloc(*words, (*num_words + 1) * sizeof(char *));
                (*words)[*num_words] = calloc(' ', cur_index);
                cur_word[cur_index] = '\0';
                memcpy((*words)[*num_words], cur_word, cur_index);
                ++(*num_words);
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
        *words = realloc(*words, (*num_words + 1) * sizeof(char *));
        (*words)[*num_words] = calloc(' ', cur_index);
        memcpy((*words)[*num_words], cur_word, cur_index);
        ++(*num_words);
    }
    else {
        fprintf(stderr, "Zle brasko\n");
        *num_words = -1;
        return;
    }
}


int main() {
    const char *string = "U mojkokot tvojkokot jejkokot pizedolsimikokot";
    char **words = malloc(sizeof(char *));
    int num_words = 0;
    split_string(&words, string, &num_words);
    printf("%d\n", num_words);

    for (int i = 0; i < 5; i++) {
        printf("%s\n", words[i]);
    }
    return 0;

}
