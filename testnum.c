#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int num_unique_words(char **words, size_t num_words);

int main() {
    char *words[] = {"test", "test", "hello", "test", "hello", "hovno"};
    int num_words = 6;
    printf("Unique words: %d\n", num_unique_words(words, num_words));
    return 0;
}


int num_unique_words(char **words, size_t num_words) {
    int count = 0;
    bool found;
    for (int i = 0; i < num_words; i++) {
        found = false;
        for (int j = i + 1; j < num_words; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                found = true;
            }
        }
        if (!found) {
            count += 1;
        }
    }
    return count;
}



