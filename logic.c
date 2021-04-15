#include <stdlib.h>
#include <string.h>

#include "logic.h"
#include "words.h"

player create_player(const char *name) 
{
    player player = { .pseudo = name, .score = 0, .played_game = NULL}; 
    return player;
}


game create_game() 
{
    game game = {.actual_word = "ccaabb",
    .tentative = max_tent, .time = max_time, .win = false, .found_set = 0};
    game.actual_state = playing;
    game.word_len = strlen(game.actual_word);
    game.matched = (char *) malloc(game.word_len);
    memset(game.matched, ' ', game.word_len);
    return game;
}