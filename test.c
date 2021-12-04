#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


void split_string(char ***words, const char *string, int *num_words);
void free_2d_array(char **array, int len);

int main() {
    const char *test_string = "Hello my name is Peter\n";
    char **words = NULL;
    int num_words;
    split_string(&words, test_string, &num_words);
    free_2d_array(words, num_words);

    return 0;
}

void split_string(char ***words, const char *string, int *num_words)  {
    *num_words = 0;
    int num_spaces = 0;
    char cur_word[30];
    int cur_index = 0;
    for (size_t i = 0; i < strlen(string); i++) {
        if (string[i] == ' ') {
            if (num_spaces == 1) {
                *num_words = -1;
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
        (*words)[*num_words] = calloc(' ', cur_index-1);
        memcpy((*words)[*num_words], cur_word, cur_index-1);
        ++(*num_words);
    }
    else {
        *num_words = -1;
    }
}


void free_2d_array(char **array, int len) {
    for (int i = 0; i < len; i++) {
        free(array[i]);
    }
    free(array);
}

