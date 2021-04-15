#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "words.h"
#include "logic.h"

#define INT_SIZE sizeof(int) * 8

#define ALPHABET_NUMBER 26
static const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int used_alpha = 0x3ffffff;


static inline int unset_flag (int value, int flag) 
{
    return (value & ~(1 << flag));
}

static inline int set_flag (int value, int flag) 
{
    return (value | (1 << flag));
}
static inline int check_flag(int value, int index) 
{
    return (value & (1 << index));
}


void print_available () 
{
    for (int i = 0; i < ALPHABET_NUMBER; i++) {
        if (check_flag(used_alpha, i))
            printf("%c\t", alphabet[i]);
        else 
            printf("-\t");
    }
    printf("\n");
}

char new_letter ()
{
    char c = getchar();
    while (getchar() != '\n');
    used_alpha = unset_flag(used_alpha, letter_to_number(c));
    return c;
}

void print_hidden (char *word, int found_set, char hid) {
    int len = strlen(word);
    for (int i = 0; i < len; i++)
        if (check_flag(found_set, i))
            printf("%c", word[i]);
        else 
            printf("%c", hid);
    printf(" ");
}


int bits_to_count (int bits)
{
    int count = 0;
    for (int i = 0; i < INT_SIZE; i++)
    {
        if (bits & 1)
            count++;
        bits >>= 1;
    }
    return count;
}

typedef struct int_list {
    int *content;
    int count;
}int_list;

int_list find_index(char *word, char c)
{
    int len = strlen (word);
    int indexes[len];
    int count = 0;
    for (int i = 0; i < len; i++) {
        if (*(word + i) == c) {
            indexes[count++] = i;
        }
    }
    int_list ret = { .content = NULL, .count = count};
    if (!count) return ret;
    ret.content = (int *) malloc (sizeof (int) * count );
    memset(ret.content, 0, count * sizeof(int));
    memcpy(ret.content, indexes, count * sizeof(int)); 
    return ret;
}



int game_done(game g) 
{
    if (bits_to_count(g.found_set) == strlen(g.actual_word))
        return found;
    if (g.tentative < 0)
        return tentative;
    if (g.time < 0)
        return time_over;
    return 0;
}


void print_gameinfo(game g1) 
{
    printf("There is %d tentative yet et %f seconds\n", g1.tentative, g1.time);
    printf("What is the hidden word ? ");
    print_hidden(g1.actual_word, g1.found_set, '*');
    printf("\n");
    print_available();
    printf("Press ? to get help\n");
}

char help_char (game *g1)
{
    int r;
    do {
        r = rand() % strlen(g1->actual_word);
    }
    while (check_flag(g1->found_set, r)); 
    return g1->actual_word[r];
}

void pendu(game *g1) 
{
    print_gameinfo(*g1);
    char c = new_letter();
    if (c == '?') {
        c = help_char(g1);
    }
    int_list ind = find_index(g1->actual_word, c);
    if (ind.content) {
        for (int i = 0; i < ind.count; i++) {
            g1->found_set = set_flag(g1->found_set, ind.content[i]);
        }
    }
    else {
        g1->tentative--;
    }  
}


void moteus(game *g1) 
{
    char prop[g1->word_len];
    g1->found_set = set_flag(g1->found_set, 0);
    printf("Vous have %d and %f seconds\n", g1->tentative, g1->time);
    print_hidden(g1->actual_word, g1->found_set, '-');
    printf("[%s] ", g1->matched);
    printf(" word with %d caracters ", g1->word_len);
    scanf("%s", prop);
    for (int i = 0; i < g1->word_len; i++) {
        if (g1->actual_word[i] == prop[i]) 
            g1->found_set = set_flag(g1->found_set, i);
        for (int j = 0; j < g1->word_len; j++) {
            if (i != j && g1->actual_word[j] == prop[i]) {
                g1->matched[j] = prop[i] - 32; 
            }
        }
    }
    g1->tentative--;
}

int calc_score (game g) 
{
    int err = (max_tent - g.tentative);
    if (err <= 1)
        return 4;
    else if (err <= 3)
        return 3;
    else if (err <= 5)
        return 2;
    else if (err == 6) return 1;
    return 0;

}

void update_ranking (player p) {

}

void update_score (player p, int score) {
    p.score += score;
}
int end_game (game g1) {
    int score = 0;
    printf("The word is %s\n", g1.actual_word);
    switch (g1.actual_state)
    {
        case found:
            g1.win = true;
            score = calc_score(g1);
            printf("Congr, the word was found\n");
            break;
        case time_over:
            printf("Time is Over!\n");
            break;
        case tentative:
            printf("Tentative used out!\n");
            break;
        default:
            break;
    }
    return score;
}

int start_session (void  (*method)(game *))
{
    create_db("english.txt");
    game g1 = create_game();
    while (!(g1.actual_state = game_done(g1))) {
        // clock_t begin = clock();
        
        method(&g1);
        // g1.time -= (clock() - begin); 
    }
    return end_game(g1);
}



int main ()
{
    signal(SIGALRM,end_game);
    player p = create_player("Wissem");
    int score = start_session(moteus);
    update_score(p, score);
    update_ranking(p);
    return 0;
}