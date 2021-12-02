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
    size_t num_items;
} universe_t;


typedef struct Set {
    char **items;
    size_t num_items;
} set_t; 

typedef struct Pair {
    char *first;
    char *second;
} pair_t;

typedef struct Relation {
    pair_t **pairs;
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

// prototypes of functions for relations
rel_t *rel_ctor();
int rel_load(universe_t *U, rel_t *rel, char *buffer);
int rel_add_pair(universe_t *U, rel_t *rel, const char *first, const char *second);
void rel_print(rel_t *rel);

bool contains_eng_alphabet_chars(const char *string);
bool is_command(const char *string);
bool is_set_command(const char *string);
bool is_rel_command(const char *string);
bool is_true_false(const char *string);

line_t *line_load(universe_t *U, char *buffer);
int process_command(line_t **lines, int num_lines, const char *buffer);
int process_set_command(line_t **lines, int num_words, char **words, int num_lines);
int process_rel_command(line_t **lines, int num_words, char **words, int num_lines);

void print_lines(line_t **lines, int line_count);
// prototypes for functions over sets and relations
// sets
int empty(set_t *set);
int card(set_t *set);
int complement(set_t *set);
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
int function(rel_t *rel);
int domain(rel_t *rel);
int codomain(rel_t *rel);
int injective(rel_t* rel, set_t* first, set_t *second);
int surjective(rel_t * rel, set_t *first, set_t *second);
int bijective(rel_t * rel, set_t *first, set_t *second);


int run(FILE *fp);

int main (int argc, char **argv) {
<<<<<<< HEAD
=======

>>>>>>> parent of 594e94d (Before change)
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
    return is_rel_command(string) || is_set_command(string);
}

// returns true if given string matches with a set command
bool is_set_command (const char *string) {

    int string_len = strlen(string);
    char *lower = (char *) malloc(string_len);
    for (int i = 0; i < string_len; i++) {

        lower[i] = tolower(string[i]);
    }

    for (int i = 0; i < NUM_SET_COMMANDS; i++) {
        if (strcmp(lower, set_commands[i]) == 0) {
            return true;
        }
    }
    
    return false;
}

// returns true if given string matches with a relation command
bool is_rel_command (const char *string) {

    int string_len = strlen(string);
    char *lower = (char *) malloc(string_len);

    for (int i = 0; i < string_len; i++) {
        lower[i] = tolower(string[i]);
    }

    for (int i = 0; i < NUM_REL_COMMANDS; i++) {
        if (strcmp(lower, rel_commands[i]) == 0) {
            return true;
        }
    }

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

// return pointer to newly created relation
rel_t *rel_ctor () {
    rel_t *rel = (rel_t*) malloc(sizeof(rel_t));
    rel->pairs = malloc(sizeof(pair_t));
    rel->num_items = 0;

    return rel;
}


int rel_load (universe_t *U, rel_t *rel, char *buffer) {
    char **words = NULL;
    int num_words = 0;
    split_string(&words, buffer, &num_words);
    // if input to split_string() is invalid or there is only one word, input is invalid
    if (num_words == -1 || num_words == 1) {
        fprintf(stderr, "[ERROR] Invalid definition of a relation\n");
        return 0;
    }
    // there must be an odd number of words 
    if (num_words % 2 == 0) {
        fprintf(stderr, "[ERROR] Invalid definition of a relation\n");
        return 0;
    }

    // start after 'R', always increment by two
    // (dad mom) (boy girl)
    // take '(dad' and 'mom')
    for (int i = 1; i < num_words; i += 2) {
        if(!rel_add_pair(U, rel, words[i], words[i + 1])) {
            return 0;
        }
    }

    return 1;
}

// add pair to relation
int rel_add_pair(universe_t *U, rel_t *rel, const char *first, const char *second) {
    // left element must begin with (
    if (first[0] != '(') {
        return 0;
    }

    // right element must end with )
    if (second[strlen(second) - 1] != ')') {
        return 0;
    }
    // first string without (
    char *modified_first = (char *) malloc(strlen(first) - 1);
    memcpy(modified_first, first + 1, strlen(first) - 1);

    // second string without )
    char *modified_second = (char *) malloc(strlen(second) - 1);
    memcpy(modified_second, second, strlen(second) - 1);

    // find first string in universe
    char *new_item_first = find_string(U->items, U->num_items, modified_first);

    // find second string in universe
    char *new_item_second = find_string(U->items, U->num_items, modified_second);

    // if either of the words is not contained in universe, return 0;
    if(new_item_first == NULL || new_item_second == NULL) { 
        printf("%s %s\n", modified_first, modified_second);
        fprintf (stderr, "[ERROR] Relation must only contain items from universe\n");
        return 0;
    }
    // increase number of items by one
    rel->num_items++;
    // allocate memory for pointer to new pair
    rel->pairs = (pair_t **) realloc(rel->pairs, rel->num_items * sizeof(pair_t*));

    // index to add new pair to 
    int cur_index = rel->num_items - 1;
    // allocate memory for new pair
    rel->pairs[cur_index] = (pair_t *) malloc(sizeof(pair_t));

    // allocate memory for pointer to first word of the pair
    rel->pairs[cur_index]->first = (char *) malloc(strlen(new_item_first));

    // allocate memoryf for pointer to second word of the pair
    rel->pairs[cur_index]->second = (char *) malloc(strlen(new_item_second));

    // copy first word to pair 
    strcpy(rel->pairs[cur_index]->first, new_item_first);

    // copy second word to pair
    strcpy(rel->pairs[cur_index]->second, new_item_second);

    return 1;
}

void rel_print(rel_t *rel) {
    printf("R: ");
    for (size_t i = 0; i < rel->num_items; i++) {
        printf("(%s, %s) ", rel->pairs[i]->first, rel->pairs[i]->second);
    }
    printf("\n");
}


line_t *line_load(universe_t *U, char *buffer) {
    line_t *line = (line_t *) malloc(sizeof(line_t));
    // if line starts with R
    if (buffer[0] == 'R') {
        rel_t *rel = rel_ctor();
        // if loading realtion was successful
        if(!rel_load(U, rel, buffer)) { 
            fprintf(stderr, "[ERROR] Failed to load relation.\n");
            return NULL;
        }
        rel_print(rel);
        line->set = NULL;
        line->rel = rel;

        return line;
    }

    printf("%d\n", __LINE__);
    // if loading set was successful
    set_t* set = set_ctor();
    if (set_load(U, set, buffer)) {
        fprintf(stderr, "[ERROR] Failed to load set.\n");
        return NULL;
    }
    line->set = set;
    line->rel = NULL;
    set_print(set);

    // if everything went well, return 1
    return line;
}

// returns 1 if processing command was successful
int process_command(line_t **lines, int num_lines, const char* buffer) {
    printf("%d\n", __LINE__);

    char **words = NULL;
    int num_words;

    printf("%d\n", __LINE__);
    if (buffer[1] && buffer[1] != ' ') {
        fprintf(stderr, "[ERROR] Invalid definition of command.\n");
        return 0;
    }

    printf("%d\n", __LINE__);
    split_string(&words, buffer, &num_words);

    printf("%d\n", __LINE__);
    if (num_words == -1 || num_words == 1) {
        fprintf(stderr,"[ERROR] invalid definition of command\n");
        return 0;
    }

    printf("%d\n", __LINE__);
    if (is_set_command(words[1])) {
        if (!process_set_command(lines, num_words, words, num_lines)) {
            fprintf(stderr, "[ERROR] Invalid definition of command\n");
            return 0;
        }
    }
    else if (is_rel_command(words[1])) {
    printf("%d\n", __LINE__);
    else if (is_set_command(words[1])) {
        if (!process_rel_command(lines, num_words, words, num_lines)) {
            fprintf(stderr, "[ERROR] Invalid definition of command\n");
            return 0;
        }
    }
    else {
    printf("%d\n", __LINE__);
        printf("%d\n", __LINE__);
        fprintf(stderr, "[ERROR] Invalid definition of command\n");
        return 0;
    }

    printf("%d\n", __LINE__);
    return 1;
}

int process_set_command(line_t *lines, int num_words, char **words, int num_lines) {
    (void) lines;
    (void) num_words;
    (void) words;
    (void) num_lines;
    // if number of words is 3, there are only three possible funcdtions
    if (num_words == 3) {
        // current command 
        const char* command = words[1];
        int line = atoi(words[2]);
        line -= 2;
        // if num lines is negative or greater than number of lines stored
        if (line < 0 || line > num_lines - 1) {
            return 0;
        }
        printf("%d\n", line);
        set_t *set = lines[line]->set;
        printf("%d\n", line);
        // if no set was defined on the line, return 0
        if (set == NULL) {
            return 0; 
        }
        
        if (strcmp(command, "empty") == 0) {
            // empty(); // TODO
        }
        else if (strcmp(command, "card") == 0) {
            // card(); // TODO
        }
        else if (strcmp(command, "complement") == 0) {       
            // antisymmetric(); // TODO
        }
    }
    // if number of words is 4, there are 6 possible functions
    else if (num_words == 4) {
        const char* command = words[1];
        if (strcmp(command, "union") == 0) {
            // union_function(); // TODO
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


int process_rel_command(line_t **lines, int num_words, char **words, int num_lines) {
    (void) lines;
    (void) num_lines;
    if (num_words == 3) {
        const char *command = words[1];
        if (strcmp(command, "reflexive")) {
            // return reflexive();
        }
        else if (strcmp(command, "symmetric")) {
            // return symmetric();
        }
        else if (strcmp(command, "antisymmetric")) {
            // return antisymmetric();
        }
        else if (strcmp(command, "transitive")) {
            // return transitive();
        }
        else if (strcmp(command, "function")) {
            // return function();
        }
        else if (strcmp(command, "domain")) {
            // return function();
        }
        else if (strcmp(command, "codomain")) {
            // return codomain();
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

// main program, returns 1 when an error occurs
int run(FILE *fp) {
    line_t **lines = (line_t **) malloc(sizeof(line_t*));
    int line_count = 0;
    char buffer[BUFFER_LEN];
    universe_t U;
    bool command_only = false;
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
            lines = (line_t **) realloc(lines, (line_count + 1) * sizeof(line_t *));
            // relations and sets must be defined before reading commands
            if (command_only) {
                return 1;
            }

            // if buffer on index 1 exists and it is not space
            if (buffer[1] && buffer[1] != ' ') {
                fprintf(stderr, "[ERROR] Invalid input.\n");
                return 1;
            }
            line_t *new_line = NULL;
            new_line = line_load(&U, buffer);
            if (new_line == NULL) {
                return 1;
            }
            else{
                lines[line_count] = new_line;
            }
        }
        // if line starts with 'C', we're ready for reading commands
        if (buffer[0] == 'C') {
            print_lines(lines, line_count);
            printf("som tu\n");
            // if buffer on index 1 exists and it is not space 
            if (buffer[1] && buffer[1] == ' ') {
                command_only = true;
                if (!process_command(lines, line_count, buffer)) {
                    return 1;

                }
            }
            else {
                
            }
        }
        ++line_count;
    }
    return 0;
}

int empty(set_t *set){
	return 1;
}
int card(set_t *set){
	return 1;
}
int complement(set_t *set){
	return 1;
}
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
int symmetric(rel_t *rel){
	return 1;
}
int antisymmetric(rel_t *rel){
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
int codomain(rel_t *rel){
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

void print_lines(line_t **lines, int num_lines) {
    for (int i = 0; i < num_lines; i++) {
        if (lines[i] == NULL) {
            printf("set: NULL");
        }
        else {
            printf("set: NOT NULL");
        }
        if (lines[i] == NULL) {
            printf("rel: NULL");
        }
        else {
            printf("rel: NOT NULL");
        }
    }
}
