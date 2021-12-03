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

int free_all(int return_value, universe_t *U, line_t lines[MAX_LINES], int num_lines);

// prototypes of functions for sets
set_t *set_ctor();
char* find_string(char **array, int array_len, const char* string);
int set_add_item(universe_t *U, set_t* S, const char* item);
void set_print(set_t *set);
int set_load(universe_t *U, set_t *S, const char *buffer);

// prototypes of functions for relations
rel_t *rel_ctor();
int rel_load(universe_t *U, rel_t *rel, char *buffer);
int rel_add_pair(universe_t *U, rel_t **rel, const char *first, const char *second);
void rel_print(rel_t *rel);

bool contains_eng_alphabet_chars(const char *string);
bool is_command(const char *string);
bool is_set_command(const char *string);
bool is_rel_command(const char *string);
bool is_true_false(const char *string);

int line_load(universe_t *U, line_t *line, char *buffer);
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

int run(FILE *fp) {
    (void) fp;
    const char *test_string = "Hello my name is Peter and I am no t gay\n";
    int num_words;
    char **words;
    split_string(&words, test_string, &num_words);
    return 1;
    for (int i = 0; i < num_words; i++) {
        printf("%s\n", words[i]);
    }
    return 1;
}
