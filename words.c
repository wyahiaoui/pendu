#include <stdio.h>
#include <string.h>
#include <time.h>

#include "words.h"



char *read_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("Couldn't find file.\n");
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string =(char *) malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);
    string[fsize] = 0;
    return string;
}


word_list str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    count += last_comma < (a_str + strlen(a_str) - 1);
    count++;
    word_list words = { .content = 0, .count = count};
    words.content = (char **)malloc(sizeof(char*) * count);

    if (words.content)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            if (idx < count) {
                *(words.content + idx++) = strdup(token);
                token = strtok(0, delim);
            }
        }
        if (idx == count - 1) {
            *(words.content + idx) = 0;
        }
    }
    return words;
}


void delete_list(word_list words) {
    int i = 0;
    while (words.content[i]) 
    {
        free(words.content[i++]);
    }
    free(words.content);
}

int letter_to_number(char letter) 
{
    int num = 0;
    if (letter >= 'A' && letter <= 'Z')
        num = letter - 'A';
    else if (letter >= 'a' && letter <= 'z')
        num = letter - 'a';
    return num;
}

char *random_word() {
    return dict.content[rand() % dict.count];
}

int create_db (const char *name)
{
    char *str = read_file(name);
    if (!str) return -1;
    dict = str_split(str, '\n');
    free(str);
    srand(time(NULL));
    return 0;
}