#ifndef PLAYER_H
#define PLAYER_H

#include "../common/common_interface.h"
#include "../common/utils.h"

struct player
{
    void* lib_ptr;
    const char * (*get_player_name)(void);
    const char * (*initialize)(unsigned int, unsigned int);
    struct move (*play)(enum card_id, const struct move*, size_t);
    void (*finalize)(void);
};

/**
 * @brief Inits the player
 * @return a newly allocated struct player pointer
 */
struct player* init_player();

/**
 * @brief Release memory of a player
 * @param p the player
 */
void free_player(struct player *p);

#endif