#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

const unsigned int MAX_ROWS = 1000;
const unsigned int U_MAX_ITEM_LEN = 30;
const int MAX_ARGUMENTS = 2; // maximum number of arguments, including name of executable
const unsigned int NUM_SET_COMMANDS = 9;
const unsigned int NUM_RELATION_COMMANDS = 10;

const char* set_commands[] = {"empty", "card", "complement", "union",
                            "intersect", "minus", "subseteq", "subset",
                            "equals"};

const char *relation_commands[] = {"reflexive", "symmetric", "antisymmetric",
                                   "transitive", "function", "domain", "codomain", 
                                   "injective", "surjective", "bijective"};

typedef struct Universe {
    const char **items;
    size_t num_items;
} universe_t;

typedef struct Pair {
    const char* first;
    const char* second;
} pair_t;

typedef struct Relation {
    struct Pair *pairs;
    size_t num_items;
} relation_t;


// error constants
enum _errors {TOO_FEW_ARGUMENTS, TOO_MANY_ARGUMENTS};

// string for errors
const char *error_strings[] = {"Number of arguments is too small\n",
    "Number of arguments is too high\n"};

void errorMessage(int error_code);

// file related functions
const char* getFilename(int argc, char** argv);
bool fileExists(const char* filename);

// universe related functions
// helper functions
bool isCommand(const char *word);
bool containsEnglishLetters(const char *word);

universe_t *createUniverse();
int universeAddItem(universe_t* U, const char* item);
void printUniverse(universe_t* U);
bool universeValidItem(const char* item);


int main(int argc, char **argv) {

    // name of file we're gonna be working with
    const char* filename = getFilename(argc, argv);

    if (filename == NULL) {
        // if no filename argument is given, exit program
        return 1;
    }
    // if file doesnt exist, exit program
    if (!fileExists(filename)) {
        fprintf(stderr, "[ERROR] file %s does not exist\n", filename);
        return 1;
    }
    struct Universe *U = createUniverse();
    if (!U) {
        fprintf(stderr, "Couldnt create Universe\n");
        return 1;
    }
    const int num_elements = 6;
    const char *data[] = {"Hello", "World", "My", "Name", "Is", "Peter"};
    (void) data;
    for (int i = 0; i < num_elements; i++)  {
        universeAddItem(U, data[i]);
    }

    printUniverse(U);

    return 0;
}

// function prints error message given an error code
void errorMessage(int error_code) {
    const char* error_message = error_strings[error_code];
    fprintf(stderr, "[ERROR] %s\n", error_message);
}

// returns name of file from command line, if arguments are in wrong format, return NULL pointer
const char* getFilename(int argc, char** argv) {
    // if number of arguments exceeds the maximum 
    if (argc < MAX_ARGUMENTS) {
        errorMessage(TOO_FEW_ARGUMENTS);
        return NULL;
    }
    // if maximum arguments is lower than the maximum
    if (argc > MAX_ARGUMENTS) {
        errorMessage(TOO_MANY_ARGUMENTS);
        return NULL;
    }
    // return filename
    return argv[1];
}

// returns true if file 'filename' exists in current directory
bool fileExists(const char* filename) {
    // if filename exists in current directory, return true
    return !access(filename, F_OK);
}

// returns pointer to a newly created structure universe
universe_t* createUniverse() {
    universe_t *universe = (universe_t*) malloc(sizeof(universe_t)); // allocate space for universe structure
    universe->items = malloc(0); // allocate zero bytes for the elements
    universe->num_items = 0; // number of elements is zero at the beinning

    return universe;
}

int universeAddItem(universe_t *U, const char* item) {
    if (!U) { // if U is NULL, it hasn't been initialized
        fprintf(stderr, "Structure is not initialized\n");
        return 0;
    }
    U->items = realloc(U->items, (U->num_items + 1) * sizeof(char *)); // temporary variable, in case realloc doesnt go well
    if (!U->items) { // if realloc didnt go well, return false
        return 0;
    }
    U->items[U->num_items++] = item; // assign new item to the end of array, while also increasing its size by one 
    return 1;
}

// prints all elements from universe
void printUniverse(universe_t* U) {
    if (!U) { // if the universe hasn't been initialized
        fprintf(stderr, "Structure is not initialized\n");
        return;
    }
    for (size_t i = 0; i < U->num_items; i++) {
        // print the universe, if we're on last character, print end of line, else print space
        printf("%s%c\n", U->items[i], i == U->num_items - 1 ? '\n' : ' ');
    }
}

// returns true if word contains only lowercase and uppercase english characters
bool containsEnglishLetters(const char* word) {
    char c;
    for (size_t i = 0; i < strlen(word); i++) {
        c = word[i];
        // if letter is not a letter form enligsh alphabet, return false
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            return false;
        }
    }
    return true;
}

bool isCommand(const char* word) {
    // iterate over set commands
    for (size_t i = 0; i < NUM_SET_COMMANDS; i++) {
        // if word equals current command, return false
        if (strcmp(set_commands[i], word)) {
            return false;
        }
    }
    // iterate over relation commands
    for (size_t i = 0; i < NUM_RELATION_COMMANDS; i++) {
        if (strcmp(relation_commands[i], word)) {
            return false;
        }
    }
    // word cant be equal to 'true'
    if (strcmp("true", word)) {
        return false;
    }
    // word cant be equal to 'false'
    if (strcmp("false", word)) {
        return false;
    }
    
    // if word has passed all conditions, return true
    return true;
}

// check if valid for universe
bool universeValidItem(const char* item) {
    // if length of item exceeds maximum allowed length, its not valid
    if (strlen(item) > U_MAX_ITEM_LEN) {
        fprintf(stderr, "Length of word exceeds maximum length\n");
        return false;
    }
    
    // check other conditions
    if (!containsEnglishLetters(item)) {
        fprintf(stderr, "Item must contains only lowercase and uppercase english characters\n");
        return false;
    }
    if (isCommand(item)) {
        fprintf(stderr, "Item cannot be command or true/false\n");
        return false;
    }

    return true;
}

