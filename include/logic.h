#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>

static int max_tent = 8;
static double max_time = 1800;

typedef enum {playing, found, time_over, tentative} state;

typedef struct game {
    char *actual_word;
    int word_len;
    char *matched; // only for moteus
    int found_set;
    int tentative;
    double time; // could change
    state actual_state;
    bool win;
}game;


typedef struct player {
    const char *pseudo;
    int score;
    int ranking;
    game *played_game;
}player;

game create_game();
player create_player(const char *name);
#endif