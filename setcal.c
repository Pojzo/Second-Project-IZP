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

// prototipes of functions to free memory 
void set_free(set_t *set);
void rel_free(rel_t *rel);
void file_free(FILE *fp);
void free_2d_array(char ***array, int len);        
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
int rel_add_pair(set_t  *U, rel_t **rel, const char *first, const char *second);
void rel_print(rel_t *rel);

bool contains_eng_alphabet_chars(const char *string);
bool is_command(const char *string);
bool is_set_command(const char *string);
bool is_rel_command(const char *string);
bool is_true_false(const char *string);

int line_load(set_t *U, line_t *line, const char *buffer, bool loading_universe);
int process_command(set_t *U, line_t lines[MAX_LINES], int num_lines, const char *buffer);
int process_set_command(set_t *U, line_t lines[MAX_LINES], int num_words, char **words, int num_lines);
int process_rel_command(set_t *U, line_t lines[MAX_LINES], int num_words, char **words, int num_lines);

void print_lines(line_t lines[MAX_LINES], int num_lines);
int count_unique_words(char **words, int num_words);
// prototypes for functions over sets and relations
// sets
void empty(set_t *set);
void card(set_t *set);
int complement(set_t *U, set_t *set);
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
 
// returns true if all condiotions for item in universe are met
bool universe_valid_item(const char *string) {
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

// return pointer to newly created set
set_t *set_ctor () {
    set_t *S = (set_t*) malloc(sizeof(set_t));
    S->items = NULL;
    S->num_items = 0;

    return S;
}

// return pointer to newly created relation
rel_t *rel_ctor () {
    rel_t *rel = (rel_t*) malloc(sizeof(rel_t));
    rel->pairs = NULL;
    rel->num_items = 0;

    return rel;
}

char *find_string (char **array, int array_len, const char* string) {
    for (int i = 0; i < array_len; i++) {
        // if strings matches array[i]
        if (array[i] == NULL || string == NULL) {
            return NULL;
        }
        if (strcmp(array[i], string) == 0) {
            return array[i];
        }
    }
    return NULL;
}

int rel_load (set_t *U, rel_t *rel, const char *buffer) {
    char **words = NULL;
    int num_words = 0;
    split_string(&words, buffer, &num_words);
    // if input to split_string() is invalid or there is only one word, input is invalid
    if (num_words == -1 || num_words == 1) {
        fprintf(stderr, "[ERROR] Invalid definition of a relation\n");
        free_2d_array(&words, num_words);
        return 0;
    }
    // there must be an odd number of words 
    if (num_words % 2 == 0) {
        fprintf(stderr, "[ERROR] Invalid definition of a relation\n");
        free_2d_array(&words, num_words);
        return 0;
    }

    // start after 'R', always increment by two
    // (dad mom) (boy girl)
    // take '(dad' and 'mom')
    for (int i = 1; i < num_words; i += 2) {
        if(!rel_add_pair(U, &rel, words[i], words[i + 1])) {
            free_2d_array(&words, num_words);
            return 0;
        }
    }
    free_2d_array(&words, num_words);
    return 1;
}

// add pair to relation
int rel_add_pair(set_t *U, rel_t **rel, const char *first, const char *second) {
    // left element must begin with (
    if (first[0] != '(') {
        return 0;
    }

    // right element must end with )
    if (second[strlen(second) - 1] != ')') {
        return 0;
    }
    // first string without (
    int len_first = strlen(first);
    int len_second = strlen(second);
    char *modified_first = (char *) malloc(len_first);
    first = first + 1;
    strcpy(modified_first, first);
    modified_first[len_first - 1] = '\0';

    // second string without )
    char *modified_second = (char *) malloc(len_second);
    memcpy(modified_second, second, len_second);
    modified_second[len_second - 1] = '\0';

    // find first string in universe
    char *new_item_first = find_string(U->items, U->num_items, modified_first);

    // find second string in universe
    char *new_item_second = find_string(U->items, U->num_items, modified_second);

    free(modified_second);

    free(modified_first);


    // if either of the words is not contained in universe, return 0;
    if(new_item_first == NULL || new_item_second == NULL) { 
        printf("%s %s\n", new_item_first, new_item_second);
        fprintf (stderr, "[ERROR] Relation must only contain items from universe\n");
        return 0;
    }
    // increase number of items by one
    (*rel)->num_items++;
    // allocate memory for pointer to new pair
    (*rel)->pairs = (pair_t **) realloc((*rel)->pairs, (*rel)->num_items * sizeof(pair_t*));

    // index to add new pair to 
    int cur_index = (*rel)->num_items - 1;
    // allocate memory for new pair
    (*rel)->pairs[cur_index] = (pair_t *) malloc(sizeof(pair_t));

    // allocate memory for pointer to first word of the pair
    (*rel)->pairs[cur_index]->first = (char *) malloc(strlen(new_item_first) + 1);

    // allocate memoryf for pointer to second word of the pair
    (*rel)->pairs[cur_index]->second = (char *) malloc(strlen(new_item_second) + 1);

    // copy first word to pair 
    strcpy((*rel)->pairs[cur_index]->first, new_item_first);

    // copy second word to pair
    strcpy((*rel)->pairs[cur_index]->second, new_item_second);

    return 1;
}

void rel_print(rel_t *rel) {
    printf("R: ");
    for (int i = 0; i < rel->num_items; i++) {
        printf("(%s, %s) ", rel->pairs[i]->first, rel->pairs[i]->second);
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
    return is_rel_command(string) || is_set_command(string);
}

// returns true if given string matches with a set command
bool is_set_command (const char *string) {
/*
    int string_len = strlen(string);

    char *lower = (char *) malloc(string_len);
    for (int i = 0; i < string_len; i++) {

        lower[i] = tolower(string[i]);
    }
*/

    for (int i = 0; i < NUM_SET_COMMANDS; i++) {
        if (strcmp(string, set_commands[i]) == 0) {
            // free(lower);
            return true;
        }
    }
    //free(lower);
    return false;
}

// returns true if given string matches with a relation command
bool is_rel_command (const char *string) {
    /*
    int string_len = strlen(string);

    char *lower = (char *) malloc(string_len);

    for (int i = 0; i < string_len; i++) {
        lower[i] = tolower(string[i]);
    }

    */
    for (int i = 0; i < NUM_REL_COMMANDS; i++) {
        if (strcmp(string, rel_commands[i]) == 0) {
            //free(lower);
            return true;
        }
    }
    //free(lower);
    return false;
}

// returns true if given string is "true" of "false"
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
    set->items = NULL;
    free(set);
}

void file_free(FILE *fp) {
    fclose(fp);
}

int free_all(int return_value, line_t lines[MAX_LINES], int num_lines, FILE *fp) {
    for(int i = 0; i < num_lines; i++) {
        set_free(lines[i].set);
        rel_free(lines[i].rel);
        free(lines[i].rel);
    }
    file_free(fp);
    return return_value;
}

int line_load(set_t *U, line_t *line, const char *buffer, bool loading_universe) {
    
    if (loading_universe) {
        set_t *set = set_ctor();
        if (!set_load(U, set, buffer, loading_universe)) {
            line = NULL;
            fprintf(stderr, "[ERROR] Failed to load set.\n");
            return 0;
        }

        line->is_set = true;
        line->set = set;
        line->rel = NULL;
        return 1;
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
            //rel_print(rel);
            line->set = NULL;
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

    // if everything went well, return 1
    //set_print(line->set);
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
        free(rel->pairs[i]);
    }

    free(rel->pairs);
    rel->pairs = NULL;
}



int set_load(set_t *U, set_t *set, const char *buffer, bool loading_universe) {
    char **words = NULL;
    int num_words = 0;
    split_string(&words, buffer, &num_words);

    if (num_words == -1) {
        fprintf(stderr, "[ERROR] Invalid definition of set\n");
        free_2d_array(&words, num_words);
        return 0;
    }

    // load every word to set
    for (int i = 1; i < num_words; i++) {
        // if we're loading universe
        if (U == NULL) {
            if (!set_add_item(set, set, words[i], loading_universe)) {
                free_2d_array(&words, num_words);
                return 0;
            }
        }
        else {
            if (!set_add_item(U, set, words[i], loading_universe)) {
                free_2d_array(&words, num_words);
                return 0;
            }
        }
    }
    // free words from memor
    free_2d_array(&words, num_words);
    // free(words);
    return 1;
}



int set_add_item(set_t *U, set_t *set, char *item, bool loading_universe) {
    // first check if there isnt already item in the set
    for (int i = 0; i < set->num_items; i++) {
        // if we found an item that is already int the set
        if (strcmp(item, set->items[i]) == 0) {
            //printf("%s %s\n", item, set->items[i]);
            fprintf(stderr, "[ERROR] Found a duplicate element in set\n");
            return 0;
        }
    }
    if (loading_universe) { // if we're loading universe

        char **temp = (char **) realloc(set->items, (set->num_items + 1) * sizeof (char*));   
        // if realloc failed
        if (temp == NULL) {
            fprintf(stderr, "[ERROR] Couldn't allocate space for.\n");
            return 0;
        }

        // assign newly allocated memory to set
        set->items = temp;

        if(!universe_valid_item(item)) {
            return 0;
        }

        set->items[set->num_items] = (char*) malloc(sizeof(char) * (1+strlen(item)));
        strcpy(set->items[set->num_items], item); 
        // set->items[set->num_items] = item;
        set->num_items++; 

        return 1;
    }

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
    // allocate space for new pointer to char

    char **temp = (char **) realloc(set->items, (set->num_items + 1) * sizeof (char*));   
    // if realloc failed
    if (temp == NULL) {
        fprintf(stderr, "[ERROR] Couldn't allocate space for.\n");
        return 0;
    }

    // assign newly allocated memory to set
    set->items = temp;
    set->items[set->num_items] = (char*) malloc(sizeof(char)*(1+strlen(item)));

    strcpy(set->items[set->num_items], item);
    //set->items[set->num_items] = item;
    set->num_items++; 
    return 1;
}

int process_command(set_t *U, line_t lines[MAX_LINES], int num_lines, const char *buffer) {

    char **words = NULL;
    int num_words;

    if (buffer[1] && buffer[1] != ' ') {
        fprintf(stderr, "[ERROR] Invalid definition of command.\n");
        return 0;
    }

    split_string(&words, buffer, &num_words);

    if (num_words == -1 || num_words == 1) {
        fprintf(stderr,"[ERROR] Invalid definition of command\n");
        free_2d_array(&words, num_words);
        return 0;
    }

    if (is_set_command(words[1])) {
        if (!process_set_command(U, lines, num_words, words, num_lines)) {
            fprintf(stderr, "[ERROR] Invalid definition of command\n");
            free_2d_array(&words, num_words);
            return 0;
        }
    }
    else if (is_rel_command(words[1])) {
        if (!process_rel_command(U, lines, num_words, words, num_lines)) {
            fprintf(stderr, "[ERROR] Invalid definition of command\n");
            free_2d_array(&words, num_words);
            return 0;
        }
    }
    else {
        fprintf(stderr, "[ERROR] Invalid definition of command\n");
        free_2d_array(&words, num_words);
        return 0;
    }

    free_2d_array(&words, num_words);
    return 1;
}

int process_set_command(set_t *U, line_t lines[MAX_LINES], int num_words, char **words, int num_lines) {
    // if number of words is 3, there are only three possible funcdtions
    if (num_words == 3) {
        // current command 
        const char* command = words[1];
        int line = atoi(words[2]);
        set_t *set = NULL;
        if (line == 1) {
            set = U;
        }
        else {
            line -= 1;
            // if num lines is negative or greater than number of lines stored
            //
            // if its first line - universum, its never empty
            if (line < 0 || line > num_lines - 1) {
                return 0;
            }
            line_t cur_line = lines[line];
            set = cur_line.set;
        }
        // if no set was defined on the line, return 0
        if (set == NULL) {
            //printf("%d\n", line);
            return 0; 
        }

        if (strcmp(command, "empty") == 0) {
            empty(set); // TODO
        }
        else if (strcmp(command, "card") == 0) {
            card(set);
        }
        else if (strcmp(command, "complement") == 0) {       
        //    complement(U, set); // TODO
        }
    }
    // if number of words is 4, there are 6 possible functions
    else if (num_words == 4) {
        const char* command = words[1];
        if (strcmp(command, "union") == 0) {
            // union_function();
        }
        if (strcmp(command, "intersect") == 0) {
            // intersect(); // TODO
        }
        if (strcmp(command, "minus") == 0) {
            // minus(); // TODO
        }
        if (strcmp(command, "subseteq") == 0) {
            // subseteq(); // TODO
        }
        if (strcmp(command, "subset") == 0) {
            // subset(); // TODO
        }
        if (strcmp(command, "equals") == 0) {
            // equals(); // TODO
        }
    }
    return 1;
}

int process_rel_command(set_t *U, line_t lines[MAX_LINES], int num_words, char **words, int num_lines) {
    (void) lines;
    (void) U;
    (void) num_lines;
    if (num_words == 3) {
        const char* command = words[1];
        int line = atoi(words[2]);
        line -= 1;
        // if num lines is negative or greater than number of lines stored
        if (line < 0 || line > num_lines - 1) {
            return 0;
        }
        line_t cur_line = lines[line];
        rel_t *rel = cur_line.rel;
        // if no set was defined on the line, return 0
        if (rel == NULL) {
            //printf("%d\n", line);
            return 0; 
        }
        if (strcmp(command, "reflexive") == 0) {
            reflexive(rel);
        }
        else if (strcmp(command, "symmetric") == 0) {
            symmetric(rel);
        }
        else if (strcmp(command, "antisymmetric") == 0) {
            antisymmetric(rel);
        }
        else if (strcmp(command, "transitive") == 0) {
            // return transitive();
        }
        else if (strcmp(command, "function") == 0) {
            function(rel);
        }
        else if (strcmp(command, "domain") == 0) {
            domain(rel);
        }
        else if (strcmp(command, "codomain") == 0) {
            codomain(rel);
        }
    }
    else if (num_words == 5) {
        const char *command = words[1];
        if (strcmp(command, "injective")) {
            // return injective();
        }
        if (strcmp(command, "surjective")) {
            // return surjective();
        }
        if (strcmp(command, "bijective")) {
            // return bijective();
        }
    }

    return 1;
}

void penis(line_t lines[MAX_LINES])
{
    for (int i = 0 ; i < MAX_LINES; i++)
    {
        lines[i].set = NULL;
        lines[i].rel = NULL;
        lines[i].is_set = false;
    }
}

int run(FILE *fp) {
    line_t lines[MAX_LINES];
    penis(lines);
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
                return free_all(1, lines, line_count, fp);
            }
            // if loading line didnt fail
            if (line_load(lines[0].set, &new_line, buffer, false)) {
                lines[line_count] = new_line;
            }
            // if loading line failed
            else {
                return free_all(1, lines, line_count, fp);
            }
            printf("%s", buffer);
        }
        // load relation
        if (buffer[0] == 'R') {
            if (command_only) {
                return free_all(1, lines, line_count, fp);
            }
            else if (line_load(lines[0].set, &new_line, buffer, false)) {
                lines[line_count] = new_line;
            }
            printf("%s", buffer);
        }
        if (buffer[0] == 'C') {
            // if buffer on index 1 exists and it is not space 
            command_only = true;
            if (!process_command(lines[0].set, lines, line_count, buffer)) {
                return free_all(1, lines, line_count, fp);
            }

        }
        ++line_count;
    }
    return free_all(0, lines, line_count, fp);
}

int count_unique_words(char **words, int num_words) {
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

void empty(set_t *set){
    if(set->num_items == 0) {
        printf("true\n");
        return;
    }
    printf("false\n");
}

void card(set_t *set) {
    printf("%d\n", set->num_items);
}

/*
int complement(set_t *U, set_t *set){
    set_t *new_set = set_ctor();
    bool found;
    for (int i = 0; i < U->num_items; i++) {
        found = false;
        for (int j = 0; j < set->num_items; j++) {
            if (strcmp(U->items[i], set->items[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            set_add_item(set, new_set, item, U->items[i]);
        }
    }
    set_print(new_set);
    set_free(new_set);
    return 1;
}

*/

int union_function(set_t *first, set_t *second) {
    (void) first;
    (void) second;

    bool found;
    for (int i = 0; i < first->num_items; i++)
    {
        printf("%s ", first->items[i]);
    }

    for (int i = 0; i < second->num_items; i++) {
        found = false;
        for (int j = 0; j < first->num_items; j++) {
            if (strcmp(first->items[i], second->items[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            printf("%s ", second->items[i]);
        }
    } 
    return 1;
}


// ---------------------------------------- // 

void function(rel_t *rel) {
    for(int i = 0; i < rel->num_items; i++) {
        for(int j = 0; j < rel->num_items; j++) {
            if (i == j) {
                continue;
            }
            if (strcmp(rel->pairs[i]->first, rel->pairs[j]->first) == 0) {
                if (strcmp(rel->pairs[i]->second, rel->pairs[j]->second) != 0) {
                    printf("false\n");
                    return;
                }
            }
        }
    }
    printf("true\n");
}

void domain(rel_t *rel) {
    printf("S ");
    for(int i = 0; i < rel->num_items; i++) {
        // if its the last pair, print new line, otherwise print space
        printf("%s%c", rel->pairs[i]->first, i == rel->num_items - 1 ? '\n' : ' ');
    }
}

void codomain(rel_t *rel){
    printf("S ");
    for(int i = 0; i < rel->num_items; i++) {
        // if its the last pair, print new line, otherwise print space
        printf("%s%c", rel->pairs[i]->second, i == rel->num_items - 1 ? '\n' : ' ');
    }
}

int reflexive(rel_t *rel) {
    char **temp = (char **) malloc(rel->num_items * 2 * sizeof(char *));
    int cur_index = 0;
    // add all elements from relation to new array so we can count number of unique characters
    for (int i = 0; i < rel->num_items; i++) {
        temp[cur_index] = (char *) malloc(strlen(rel->pairs[i]->first) + 1);
        temp[cur_index + 1] = (char *) malloc(strlen(rel->pairs[i]->second) + 1);

        strcpy(temp[cur_index], rel->pairs[i]->first);
        strcpy(temp[cur_index + 1], rel->pairs[i]->second);
        cur_index += 2;
    }

    int num_unique_words = count_unique_words(temp, rel->num_items * 2);
    // printf("Pocet unikatnych charakterov tam %d\n", num_unique_words);
    int count_reflexive_pairs = 0;
    // count how many elements are reflexive
    for(int i = 0; i < rel->num_items; i++) {
        if (strcmp(rel->pairs[i]->first, rel->pairs[i]->second) == 0) {
            count_reflexive_pairs += 1;
        }
    }
    // only if number of reflexive pairs equals unique words, is it a reflexive relation
    if (count_reflexive_pairs == num_unique_words) {
        printf("true\n");
        return 1;
    }
    printf("false\n");
    return 0;

    // free temporary array
    for (int i = 0; i < rel->num_items * 2; i++) {
        free(temp[i]);
    }
    free(temp);
}

int symmetric(rel_t *rel) {
    bool found;
    for(int i = 0; i < rel->num_items; i++){
        found = false;
        if(strcmp(rel->pairs[i]->first, rel->pairs[i]->second) == 0) {
            continue;
        }
        else {
            for(int j = 0; j< rel->num_items; j++){
                if(strcmp(rel->pairs[i]->first, rel->pairs[j]->second) == 0 && strcmp(rel->pairs[i]->second, rel->pairs[j]->first) == 0) {
                    found = true;
                }
            }
        }
        if(!found){
            printf("false\n");
            return 0;       
        }
    }

    printf("true\n");
    return 1;
}
int antisymmetric(rel_t *rel){
    bool found;
    for(int i = 0; i < rel->num_items; i++){
        found = false;
        if(strcmp(rel->pairs[i]->first, rel->pairs[i]->second) == 0) {
            continue;
        }
        else {
            for(int j = 0; j< rel->num_items; j++){
                if(strcmp(rel->pairs[i]->first, rel->pairs[j]->second) == 0 && strcmp(rel->pairs[i]->second, rel->pairs[j]->first) == 0) {
                    found = true;
                }
            }
        }
        if(found){
            printf("false\n");
            return 0;       
        }
    }

    printf("true\n");
    return 1;
}

/*
   int union_function(set_t *first, set_t *second){
   return 1;
   }
   int intersect(set_t * first, set_t *second){
   return 1;
   }
   int minus(set_t * first, set_t *second){
   return 1;
   }
   int subseteq(set_t * first, set_t *second){
   return 1;
   }
   int subset(set_t * first, set_t *second){
   return 1;
   }
   int equals(set_t * first, set_t *second){
   return 1;
   }

// relations
int reflexive(rel_t *rel){
return 1;
}
int transitive(rel_t *rel){
return 1;
}
int function(rel_t *rel){
return 1;
}
int domain(rel_t *rel){
return 1;
}
int injective(rel_t* rel, set_t* first, set_t *second){
return 1;
}
int surjective(rel_t * rel, set_t *first, set_t *second){
return 1;
}
int bijective(rel_t * rel, set_t *first, set_t *second){
return 1;
}
*/


void print_lines(line_t lines[MAX_LINES], int num_count) {
    for (int i = 0; i < num_count-1; i++) {
        printf("set: %d \rrel: %d", lines[i].set->num_items, lines[i].rel->num_items);
    }
}
