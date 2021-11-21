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

