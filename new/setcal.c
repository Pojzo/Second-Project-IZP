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

/*
// commands that require two inputs
const char *len_two_commands[] = {"empty", "card", "complement", "reflexive", "symmetric", "antisymmetric", "function", "domain", "codomain"};

// commands that require three inputs
const char* len_three_commands[] = {"union", "intersect", "minus", "subseteq", "subset", "equals"};
*/


typedef struct Universe {
    char **items;
    int num_items;
} universe_t;


typedef struct Set {
    char **items;
    int num_items;
} set_t; 

typedef struct Pair {
    char *first;
    char *second;
} pair_t;

typedef struct Relation {
    pair_t **pairs;
    int num_items;
} rel_t; 


typedef struct Line {
    set_t *set;
    rel_t *rel;
    bool is_set;
} line_t;


// prototypes of functions
bool enough_arguments(int argc);
void split_string(char ***words, const char *string, int *num_words);

// prototypes of functions for universe
int universe_load(universe_t *U, char *line);
int universe_add_item(universe_t *U, char *word);
char **universe_resize(char ***item, size_t new_len);
bool universe_valid_item(const char *string);
void universe_print(universe_t *U);
void universe_free(universe_t *U);
void lines_free(line_t lines[MAX_LINES], int num_lines);
void set_free(set_t *set);
void rel_free(rel_t *rel);
void file_free(FILE *fp);

int free_all(int return_value, line_t lines[MAX_LINES], int num_lines, FILE *fp);

// prototypes of functions for sets
set_t *set_ctor();
char* find_string(char **array, int array_len, const char* string);
int set_add_item(set_t *U, set_t* S, char* item, bool loading_universe);
void set_print(set_t *set);
int set_load(set_t *U, set_t *S, const char *buffer, bool loading_universe);

// prototypes of functions for relations
rel_t *rel_ctor();
int rel_load(set_t *U, rel_t *rel, const char *buffer);
int rel_add_pair(universe_t *U, rel_t **rel, const char *first, const char *second);
void rel_print(rel_t *rel);

bool contains_eng_alphabet_chars(const char *string);
bool is_command(const char *string);
bool is_set_command(const char *string);
bool is_rel_command(const char *string);
bool is_true_false(const char *string);

int line_load(set_t *U, line_t *line, const char *buffer, bool loading_universe);
int process_command(universe_t *U, line_t lines[MAX_LINES], int num_lines, const char *buffer);
int process_set_command(universe_t *U, line_t lines[MAX_LINES], int num_words, char **words, int num_lines);
int process_rel_command(universe_t *U, line_t lines[MAX_LINES], int num_words, char **words, int num_lines);

void print_lines(line_t lines[MAX_LINES], int num_lines);
int count_unique_words(char **words, int num_words);
// prototypes for functions over sets and relations
// sets
void empty(set_t *set);
void card(set_t *set);
int complement(universe_t *U, set_t *set);
int union_function(set_t *first, set_t *second);
int intersect(set_t * first, set_t *second);
int minus(set_t * first, set_t *second);
int subseteq(set_t * first, set_t *second);
int subset(set_t * first, set_t *second);
int equals(set_t * first, set_t *second);

// relations
int reflexive(rel_t *rel);
int symmetric(rel_t *rel);
int antisymmetric(rel_t *rel);
int transitive(rel_t *rel);
void function(rel_t *rel);
void domain(rel_t *rel);
void codomain(rel_t *rel);
int injective(rel_t* rel, set_t* first, set_t *second);
int surjective(rel_t * rel, set_t *first, set_t *second);
int bijective(rel_t * rel, set_t *first, set_t *second);


int run(FILE *fp);

int main (int argc, char **argv) {
    /*
    for (int i = 0; i < NUM_SET_COMMANDS; i++) {
        printf("%s\n", is_set_command(set_commands[i]) ? "True" : "False");
    }
    
    for (int i = 0; i < NUM_REL_COMMANDS; i++) {
        printf("%s\n", is_rel_command(rel_commands[i]) ? "True" : "False");
    }
    */

    if (!enough_arguments(argc)) { // if there aren't enough arguments, terminate program
        return 1;
    }

    const char *filename = argv[1]; // file we'll be working with 
    FILE *fp;
    fp = fopen(filename, "r");
    if(!fp){ // if there was an error opening file
        fprintf(stderr, "Could not open file.\n");
        return 1;
    }
    return run(fp);
    //return run(fp); // function that runs the whole program, reading from file and creating instances of data structures
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

set_t *set_ctor () {
    set_t *S = (set_t*) malloc( sizeof(set_t));
    S->items = NULL;
    S->num_items = 0;

    return S;
}

// free 2d array from memory
void free_2d_array(char ***array, int len) {
    for (int i = 0; i < len; i++) {
        free((*array)[i]);
    }
    free(*array);
}

// free set from memory
void set_free(set_t *set) {
    if (set == NULL) {
        return;
    }

    for(int i = 0; i < set->num_items; i++) {
        free(set->items[i]);
    }

    free(set->items);
}

void file_free(FILE *fp) {
    fclose(fp);
}

int free_all(int return_value, line_t lines[MAX_LINES], int num_lines, FILE *fp) {
    for(int i = 0; i < num_lines; i++) {
        set_free(lines[i].set);
        rel_free(lines[i].rel);
        free(lines[i].set);
        free(lines[i].rel);
    }
    file_free(fp);
    return return_value;
}

int line_load(set_t *U, line_t *line, const char *buffer, bool loading_universe) {
    // if line starts with R
    
    if (loading_universe) {
        line->is_set = true;
        line->rel = NULL;
        line->set = set_ctor();
    }
    if (buffer[0] == 'R') {
        rel_t *rel = rel_ctor();
        line->is_set = false;
        line->set = NULL;

        if(!rel_load(U, rel, buffer)) { 
            fprintf(stderr, "[ERROR] Failed to load relation.\n");
            line = NULL;
            return 0;
        }
        else {
            // rel_print(rel);
            line->set = set_ctor();
            line->rel = rel;

            return 1;
        }
        return 1;
    }
    
    // if loading set was successful
    set_t *set = set_ctor();
    if (!set_load(U, set, buffer, loading_universe)) {
        line = NULL;
        fprintf(stderr, "[ERROR] Failed to load set.\n");
        return 0;
    }

    line->is_set = true;
    line->set = set;
    line->rel = NULL;
    //set_print(line->set);

    // if everything went well, return 1
    set_print(line->set);
    return 1;
}

void set_print(set_t *S) {
    printf("S");
    for (int i = 0; i < S->num_items; i++) {
        printf(" %s", S->items[i]);
    }
    printf("\n");
}

// free relation from memory
void rel_free(rel_t *rel) {
    if (rel == NULL) {
        return;
    }
    for (int i = 0; i < rel->num_items; i++) {
        free(rel->pairs[i]->first);
        free(rel->pairs[i]->second);
        rel->pairs[i]->first = NULL;
        rel->pairs[i]->second = NULL;
    }
    free(rel->pairs);
    rel->pairs = NULL;
    free(rel);
}



int set_load(set_t *U, set_t *set, const char *buffer, bool loading_universe) {
    char **words = NULL;
    int num_words = 0;
    split_string(&words, buffer, &num_words);

    if (num_words == -1) {
        fprintf(stderr, "[ERROR] Invalid definition of set\n");
        free(words);
        return 0;
    }

    // load every word to set
    for (int i = 1; i < num_words; i++) {
        // if we're loading universe
        if (U == NULL) {
            if (!set_add_item(set, set, words[i], loading_universe)) {
                free(words);
                return 0;
            }
        }
        else {
            if (!set_add_item(U, set, words[i], loading_universe)) {
                free(words);
                return 0;
            }
        }
    }
    // free words from memor
    free(words);
    return 1;
}



int set_add_item(set_t *U, set_t *set, char* item, bool loading_universe) {
    // first check if there isnt already item in the set
    for (int i = 0; i < set->num_items; i++) {
        // if we found an item that is already int the set
        if (strcmp(item, set->items[i]) == 0) {
            printf("%s %s\n", item, set->items[i]);
            fprintf(stderr, "[ERROR] Found a duplicate element in set\n");
            return 0;
        }
    }
    if (loading_universe) goto hello; // if we're loading universe, we want to jump over this part

    // now we check if there are only elements from the universe
    bool found;
    found = false;
    for (int i = 0; i < U->num_items; i++) {
        if (strcmp(item, U->items[i]) == 0) {
            found = true;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "[ERROR] Set must only contain elements from the universe\n");
        return 0;

    }
hello:
    // allocate space for new pointer to char

    char **temp = (char **) realloc(set->items, (set->num_items + 1) * sizeof (char*));   
    // if realloc failed
    if (temp == NULL) {
        fprintf(stderr, "[ERROR] Couldn't allocate space for.\n");
        return 0;
    }

    // assign newly allocated memory to set
    set->items = temp;
    set->items[set->num_items] = item;
    set->num_items++; 

    return 1;
}

int run(FILE *fp) {
    printf("dostal som sa sem\n");
    line_t lines[MAX_LINES];
    char buffer[10000];
    int line_count = 0;
    bool command_only = false;
    //bool command_only = false;
    while (fgets(buffer, BUFFER_LEN, fp)) { // ked sme na zaciatku, tak nacitame universe, riadok = 0
        line_t new_line;
        // allocate space for another line
        if (line_count == 0) {
            if (buffer[0] == 'U' && buffer[1] == ' ') {
                // we wanna specify that we're loading universe
                line_load(NULL, &new_line, buffer, true);
                lines[line_count] = new_line;
            }
            else {
                return free_all(1, lines, line_count, fp);
            }
            printf("%s", buffer);
            // universe_print(&U);
        }
        // load set
        if (buffer[0] == 'S') {
            if (command_only) {
                return free_all(0, lines, line_count, fp);
            }
            // if loading line didnt fail
            if (line_load(lines[0].set, &new_line, buffer, false)) {
                lines[line_count] = new_line;
            }
            // if loading line failed
            else {
                return free_all(1, lines, line_count, fp);
            }
        }
        // load relation
        if (buffer[0] == 'R') {
            if (command_only) {
                return free_all(0, lines, line_count, fp);
            }
            if (line_load(lines[0].set, &new_line, buffer, false)) {

            }
        }
        if (buffer[0] == 'C') {

        }
        ++line_count;
    }

    return free_all(0, lines, line_count, fp);
}
