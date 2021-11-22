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
const int MAX_LINES = 1000; // maximum allowed number of lines on input


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


typedef struct Set {
    char **items;
    size_t num_items;
} set_t; 

typedef struct Pair {
    const char *first;
    const char *second;
} pair_t;

typedef struct Relation {
    pair_t *pairs;
    size_t num_items;
} rel_t; 


typedef struct Line {
    set_t *set;
    rel_t *rel;
} line_t;


// prototypes of functions
bool enough_arguments(int argc);
void split_string(char ***words, const char *string, int *num_words);

// prototypes of functions for universe
int universe_load(universe_t *U, char *line);
int universe_add_item(universe_t *U, char *word);
bool universe_valid_item(const char *string);
void universe_print(universe_t *U);
void universe_free(universe_t *U);
void free_all(universe_t *U);

// prototypes of functions for sets
set_t *set_ctor();
char* find_string(char **array, size_t array_len, const char* string);
int set_add_item(universe_t *U, set_t* S, const char* item);
void set_print(set_t *set);
int set_load(universe_t *U, set_t *S, const char *buffer);

// protoypes of functions for relations
int rel_load(universe_t *U, rel_t *rel, char *buffer);

bool contains_eng_alphabet_chars(const char *string);
bool is_command(const char *string);
bool is_true_false(const char *string);

int line_load(universe_t *U, line_t *line, char *buffer);
int process_command(line_t *lines, int num_lines, const char *buffer);

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

// function splits 'strings' by spaces, stores them into words and stores
// length of words in variable 'num_words'
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


// load elements into universe from line
int universe_load (universe_t *U, char *line) {
    // na zaciatku nemame ziadne itemy, a pocet itemov je nula
    U->items = NULL;
    U->num_items = 0;
    char **words = NULL;
    int num_words;

    split_string(&words, line, &num_words);
    if (num_words == -1) {
        fprintf(stderr,"[ERROR] invalid definition of universum\n");
        return 0;
    }
    const char *identifier = words[0];
    if (strcmp(identifier, "U") != 0) {
        fprintf(stderr, "[ERROR] Invalid definition for Universum.\n"); 
        return 0;
    }
    // load every word to universe
    char *cur_word;
    for (int i = 1; i < num_words; i++) {
        // remove trailing newline, if there is one
        cur_word = words[i];
        if (!universe_add_item(U, cur_word)) {
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
    printf("U: ");
    for (size_t i = 0; i < U->num_items; i++) {
        printf("%s ", U->items[i]);
    }
    printf("\n");
}



void universe_free (universe_t *U) {
    for (size_t i = 0; i < U->num_items; i++) {
        free(U->items[i]);
    }
    free(U->items);
}

void free_all(universe_t *U) {
    universe_free(U);
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

// returns pointer to newly created set
set_t *set_ctor () {
    set_t *S = (set_t*) malloc( sizeof(set_t));
    S->items = NULL;
    S->num_items = 0;

    return S;
}

// returns true if array contains given string
char *find_string (char **array, size_t array_len, const char* string) {
    for (size_t i = 0; i < array_len; i++) {
        // if strings matches array[i]
        if (strcmp(array[i], string) == 0) {
            return array[i];
        }
    }
    return NULL;
}


// add new item to set
int set_add_item (universe_t *U, set_t *S, const char* item) {
    // every item in set must be contained in universe
    char* new_item = find_string(U->items, U->num_items, item);
    if (new_item == NULL) {
        fprintf (stderr, "[ERROR] Set must only contain elments from universe.\n");
        return 0;
    }
    // allocate space for new pointer to char
    char **temp = (char **) realloc(S->items, (S->num_items + 1) * sizeof (char*));

    // if realloc failed
    if (temp == NULL) {
        fprintf(stderr, "[ERROR] Couldn't allocate space for.\n");
        return 0;
    }

    // assign newly allocated memory to set
    S->items = temp;
    S->items[S->num_items] = new_item;
    S->num_items++; 

    return 1;
}

void set_print(set_t *S) {
    printf("S: ");
    for (size_t i = 0; i < S->num_items; i++) {
        printf("%s ", S->items[i]);
    }
    printf("\n");
}

int set_load (universe_t *U, set_t *S, const char *buffer) {
    char **words = NULL;
    int num_words = 0;
    split_string(&words, buffer, &num_words);

    if (num_words == -1) {
        fprintf(stderr, "[ERROR] Invalid definition of set\n");
        return 0;
    }

    // load every word to set
    for (int i = 1; i < num_words; i++) {
        if (!set_add_item (U, S, words[i])) {
            return 1;
        }
    }
    return 0;
}

int rel_load (universe_t *U, rel_t *rel, char *buffer) {
    char **words = NULL;
    int num_words = 0;
    split_string(&words, buffer, &num_words);
    if (num_words == -1) {
        fprintf(stderr, "[ERROR] Invalid definition of a relation\n");
        return 0;
    }
    
    for (int i = 0; i < num_words; i++) {
        printf("%s ", words[i]);   
    }
    printf("\n");
    (void)U;
    (void)rel;
    (void)buffer;

    return 1;
}

int line_load (universe_t *U, line_t *line, char *buffer) {
    set_t* set = set_ctor();
    // if line starts with R
    if (buffer[0] == 'R') {
        rel_t rel;
        // if loading realtion was successful
        if(!rel_load(U,&rel, buffer)) { 
            fprintf(stderr, "[ERROR] Failed to load relation.\n");
            return 0;
        }
        line->set = NULL;

        return 1;
    }

    // if loading set was successful
    if (set_load(U, set, buffer)) {
        fprintf(stderr, "[ERROR] Failed to load set.\n");
        return 0;
    }
    line->set = set;
    line->rel = NULL;
    set_print(set);

    // if everything went well, return 0
    return 1;
}

// returns 1 if processing command was successful
int process_command(line_t *lines, int num_lines, const char* buffer) {
    (void) lines;
    (void) num_lines;
    (void) buffer;

    // TODO
    return 0;
}

// main program, returns 1 when an error occurs
int run(FILE *fp) {
    line_t *lines = malloc(5 * sizeof(line_t));

    int line_count = 0;
    char buffer[BUFFER_LEN];
    universe_t U;
    bool ready_for_command = false;
    while (fgets(buffer, BUFFER_LEN, fp)) { // ked sme na zaciatku, tak nacitame universe, riadok = 0
        // allocate space for another line
        if (line_count == 0) {
            if (!universe_load(&U, buffer)) {
                universe_free(&U);
                return 1;
            }
            universe_print(&U);
        }
        if ((buffer[0] == 'R' || buffer[0] == 'S')) {
            //lines = (line_t *) realloc(lines, line_count * sizeof(line_t));
            // relations and sets must be defined before reading commands
            if (ready_for_command) {
                return 1;
            }

            // if buffer on index 1 exists and it is not space
            if (buffer[1] && buffer[1] != ' ') {
                fprintf(stderr, "[ERROR] Invalid input.\n");
                return 1;
            }
            if (!line_load(&U, &lines[line_count], buffer)) {
                return 1;
            }
        }
        // if line starts with 'C', we're ready for reading commands
        if (buffer[0] == 'C') {
            // if buffer on index 1 exists and it is not space 
            if (buffer[1] && buffer[1] != ' ') {
                ready_for_command = true;
                process_command(lines, line_count, buffer);
            }
        }
        ++line_count;
    }
    return 0;
}
