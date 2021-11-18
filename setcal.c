#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

const int UNIVERSE_MAX_CHARS = 30; // maximalna dlzka prvku v univerze
const int NUM_ARGUMENTS = 2;
const int NUM_SET_COMMANDS = 9;
const int NUM_REL_COMMANDS = 10; 
const int BUFFER_LEN = 20000;


const char* set_commands[] = {"empty", "card", "complement", "union",
                            "intersect", "minus", "subseteq", "subset",                     //autor: petec kovac(xpetko66)
                            "equals"};

const char *rel_commands[] = {"reflexive", "symmetric", "antisymmetric",               //napisal peter kovac(xkovac66)
                                   "transitive", "function", "domain", "codomain", 
                                   "injective", "surjective", "bijective"};


typedef struct Universe {
    char **items;
    size_t num_items;
} universe_t;

// prototypes of functions
bool enough_arguments (int argc);

// prototypes of functions for universe
int universe_load (universe_t *U, char *line);
int universe_add (universe_t *U, char *word);
bool universe_valid_item (const char *string);
void universe_print (universe_t *U);

bool contains_eng_alphabet_chars (const char *string);
bool is_command (const char *string);
bool is_true_false (const char *string);

int run(FILE *fp);

int main (int argc, char **argv) {

    if (!enough_arguments(argc)) {
        return 1;
    }


    const char *filename = argv[1];
    FILE *fp;
    fp = fopen(filename, "r");
    if(!fp){
        fprintf(stderr, "Could not open file.\n");
        return 1;
    }
    
    return run (fp);
}

// returns true if enough arguments are given
bool enough_arguments (int argc) {
    if (argc != NUM_ARGUMENTS) {
        fprintf(stderr, "Invalid number of arguments.\n");
        return false;
    }
    return true;
}

// load elements to universe from text file
int universe_load (universe_t *U, char *line) {
    // na zaciatku nemame ziadne itemy, a pocet itemov je nula
    U->items = NULL;
    U->num_items = 0;
    char *token = strtok(line, " ");
    if (strcmp(token, "U") != 0) {
        fprintf(stderr, "[ERROR] Invalid Universum.\n"); 
        return 0;
    }
    char current_word[UNIVERSE_MAX_CHARS];
    // nekonecny cyklus
    while (true) {
        token = strtok(NULL, " ");
        // ak token nenasiel ziadny dalsi string, ukoncime cyklus
        if (token == NULL) {
            break;
        }
        // ak sa nepodarilo pridat nejaky prvok do univerza, vratime false
        if (!universe_add(U, token)) {
            return 0;
        }
    }
    return 1;
}

// ak sa nam podarilo pridat prvok, returnujeme true, ak nie, tak false
int universe_add (universe_t *U, char *token) {
    int token_len = strlen(token);
    // najprv overime, ci je dlzka nanajvys 30
    if (token_len > UNIVERSE_MAX_CHARS) {
        fprintf(stderr, "[ERROR] Invalid length of an item, maximum is 30.\n");
        return 0;
    }

    // alokumeme miesto pre pointer na char, cize zaciatok nejakeho stringu
    char **temp = (char **) realloc(U->items, ++U->num_items * sizeof(char *));
    if (temp == NULL) { // if realloc failed
        return 0;
    } 
    // if realloc worked
    U->items = temp;

    // alokujeme miesto pre samostny string
    U->items[U->num_items - 1] = (char *) malloc(sizeof(token_len));
    if (U->items[U->num_items - 1] == NULL) {
        return 0;
    }
    
    if (!universe_valid_item(token)) {
        return 0;
    }

    U->items[U->num_items - 1] = token;

    return 1;
}

// returns true if all condiotions for item in universe are met
bool universe_valid_item (const char *string) {
    if (!contains_eng_alphabet_chars(string)) {
        fprintf(stderr, "[ERROR] Universe must only contain lowercase and uppercase characters.\n");
        return false;
    }
    if (is_command(string)) {
        fprintf(stderr, "[ERROR] Universe can't contain commands.\n");
        return false;
    } 
    if (is_true_false(string)) {
        fprintf(stderr, "[ERROR] Universe can't contain keywords 'true' or 'false'.\n");
        return false;
    }
    return true;
}

void universe_print(universe_t *U) {
    for (size_t i = 0; i < U->num_items; i++) {
        printf("%s ", U->items[i]);
    }
    printf("\n");
}

// returns true if all characters from the string are from english alphabet 
bool contains_eng_alphabet_chars (const char *string) {
    char c;
    for (size_t i = 0; i < strlen(string); i++) {
        c = string[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            return false;
        }
    }
    return true;
}

// returns true if given string matches with a command
bool is_command (const char *string) {
    int string_len = strlen(string);
    char lower[string_len];
    for (int i = 0; i < string_len; i++) {
        lower[i] = tolower(string[i]);
    }

    for (int i = 0; i < NUM_SET_COMMANDS; i++) {
        if (strcmp(lower, set_commands[i]) == 0) {
            return true;
        }
    }
    for (int i = 0; i < NUM_REL_COMMANDS; i++) {
        if (strcmp(lower, rel_commands[i]) == 0) {
            return true;
        }
    }
    return false;
}
bool is_true_false (const char *string) {
    int string_len = strlen(string);
    char lower[string_len];
    for (int i = 0; i < string_len; i++) {
        lower[i] = tolower(string[i]);
    }
    if (strcmp(lower, "true") == 0) {
        return true;
    }

    if (strcmp(lower, "false") == 0) {
        return true;
    }
    return false;
}

// main program, returns 1 when an error occurs
int run (FILE* fp) {
    int line_count = 0;
    char buffer[BUFFER_LEN];
    universe_t U;
    while (fgets(buffer, BUFFER_LEN, fp)) { 
        // ked sme na zaciatku, tak nacitame universe, riadok = 0
        if (line_count == 0) {
            if (!universe_load (&U, buffer)) {
                return 1;
            }
            universe_print(&U);
        }
        printf ("Line: %d - %s", ++line_count, buffer); 
        char *element = strtok(buffer, " ");
    }
    return 0;
}

