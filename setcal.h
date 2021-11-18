#ifndef SETCAL_H
#define SETCAL_H
#include <stdbool.h>

bool enough_arguments(int argc);

// prototypes of functions for universe
bool contains_eng_alphabet_chars (const char *string);
bool is_command (const char *string);
bool is_true_false (char *string);

extern const int UNIVERSE_MAX_CHARS;
extern const int NUM_ARGUMENTS;
extern const int NUM_SET_COMMANDS;
extern const int NUM_REL_COMMANDS;
extern const char *set_commands[9];
extern const char *rel_commands[10];


#endif