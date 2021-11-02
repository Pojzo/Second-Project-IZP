#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int MAX_ARGUMENTS = 2; // maximum number of arguments, including name of executable

enum _erorr_messages {TOO_FEW_ARGUMENTS, TOO_MANY_ARGUMENTS};

const char *error_msgs[] = {"Number of arguments is too small\n",
                            "Number of arguments is too high\n"};

void errorMessage(int error_code);
const char* getFilename(int argc, char** argv);

int main(int argc, char **argv) {
    const char* filename = getFilename(argc, argv);

    if (filename == NULL) {
        return 1;
    }

    printf("Filename: %s\n", filename);
    return 0;
}

void errorMessage(int error_code) {
    const char* error_message = error_msgs[error_code];
    fprintf(stderr, "[ERROR] %s\n", error_message);
}

const char* getFilename(int argc, char** argv) {
    if (argc < MAX_ARGUMENTS) {
        errorMessage(TOO_FEW_ARGUMENTS);
        return NULL;
    }
    if (argc > MAX_ARGUMENTS) {
        errorMessage(TOO_MANY_ARGUMENTS);
        return NULL;
    }
    return argv[1];
}
