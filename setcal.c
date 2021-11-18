#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


const int UNIVERSE_MAX_CHARS = 30; // maximum length of an item in Universe
const int NUM_ARGUMENTS = 2; // maximum number of arguments on command line
const int NUM_SET_COMMANDS = 9; // number of commands for Set
const int NUM_REL_COMMANDS = 10; // number of commands for Relation
const int BUFFER_LEN = 20000; // size of buffer for storing lines from text file


// commands for Set
const char* set_commands[] = {"empty", "card", "complement", "union",
                            "intersect", "minus", "subseteq", "subset",                    
                            "equals"};

// commands for Relation
const char *rel_commands[] = {"reflexive", "symmetric", "antisymmetric",              
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
int universe_add_item (universe_t *U, char *word);
bool universe_valid_item (const char *string);
void universe_print (universe_t *U);

bool contains_eng_alphabet_chars (const char *string);
bool is_command (const char *string);
bool is_true_false (const char *string);

int run(FILE *fp);

int main (int argc, char **argv) {
    if (!enough_arguments(argc)) { // if there aren't enough arguments, terminate program
        return 1;
    }

    const char *filename = argv[1]; // file we'll be working with 
    FILE *fp;
    fp = fopen (filename, "r");
    if(!fp){ // if there was an error opening file
        fprintf(stderr, "Could not open file.\n");
        return 1;
    }
    
    return run (fp); // function that runs the whole program, reading from file and creating instances of data structures
}

// returns true if enough arguments are given
bool enough_arguments (int argc) {
    if (argc != NUM_ARGUMENTS) { // number of arguments must be exactly 2
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
    char *token = strtok(line, " "); // split line by space, strtok returns pointer to first string from split
    if (strcmp(token, "U") != 0) { // definition of Universe must begin with 'U', otherwise it's invalid input
        fprintf(stderr, "[ERROR] Invalid Universum.\n"); 
        return 0;
    }
    //char current_word[UNIVERSE_MAX_CHARS]; // TODO
    // infinite cycle 
    while (true) {
        token = strtok(NULL, " ");
        // ak token nenasiel ziadny dalsi string, ukoncime cyklus
        if (token == NULL) {
            break;
        }
        // ak sa nepodarilo pridat nejaky prvok do univerza, vratime false
        if (!universe_add_item(U, token)) {
            return 0;
        }
    }
    return 1;
}


// function returns true if adding element to universe was succesfull
int universe_add_item (universe_t *U, char *token) {
    int token_len = strlen(token);
    // najprv overime, ci je dlzka nanajvys 30
    if (token_len > UNIVERSE_MAX_CHARS) {
        fprintf(stderr, "[ERROR] Invalid length of an item, maximum is 30.\n");
        return 0;
    }

    // we first have to check if the item we're adding is valid 
    if (!universe_valid_item(token)) {
        return 0;
    }

    // allocate memory for new pointer to string
    char **temp = (char **) realloc(U->items, ++U->num_items * sizeof(char *));
    if (temp == NULL) { // if realloc failed
        return 0;
    } 
    // if realloc worked
    U->items = temp;

    // allocate memory for new item/string
    U->items[U->num_items - 1] = (char *) malloc(sizeof(token_len));
    if (U->items[U->num_items - 1] == NULL) {
        return 0;
    }
    
    U->items[U->num_items - 1] = token;

    return 1;
}

// returns true if all condiotions for item in universe are met
bool universe_valid_item (const char *string) {
    if (!contains_eng_alphabet_chars(string)) {
        printf("%s\n", string);
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
        (void) element; // TODO
    }
    return 0;
}

