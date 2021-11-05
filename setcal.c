#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

const int MAX_ARGUMENTS = 2; // maximum number of arguments, including name of executable

enum _errors {TOO_FEW_ARGUMENTS, TOO_MANY_ARGUMENTS};

const char *error_strings[] = {"Number of arguments is too small\n",
                                   "Number of arguments is too high\n"};

void errorMessage(int error_code);
const char* getFilename(int argc, char** argv);
bool fileExists(const char* filename);

int main(int argc, char **argv) {
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

    FILE *fp = fopen(filename, "r");

    // if there is an error opening file, exit program
    if (!fp) {
        fprintf(stderr, "[ERROR] cannot open file %s\n", filename);
        return 1;
    }
    const int buffer_length = 1000;
    char buffer[buffer_length];
    while (fgets(buffer, buffer_length, fp)) {
        printf("%s", buffer);
    }

    return 0;
}

void errorMessage(int error_code) {
    const char* error_message = error_strings[error_code];
    fprintf(stderr, "[ERROR] %s\n", error_message);
}

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

bool fileExists(const char* filename) {
    // if filename exists in current directory, return true
    return !access(filename, F_OK);
}

