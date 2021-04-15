#ifndef WORDS_H
#define WORDS_H

#include <stdlib.h>

#define STR_LEN 64

typedef struct word_list {
    char **content;
    size_t count;
}word_list;

static word_list dict;

char *random_word();
int create_db ();
int letter_to_number(char letter);

#endif