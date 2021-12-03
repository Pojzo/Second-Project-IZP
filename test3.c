#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct U {
    char **items;
    int num_items;
};

int main() {
    char **test;
    test = NULL;
    const char *string = "Hello";
    test = realloc(test, sizeof(char *));
    test[0] = malloc(strlen(string) + 1);
    strcpy(test[0], string);
    printf("%s\n", test[0]);
    return 0;
}
