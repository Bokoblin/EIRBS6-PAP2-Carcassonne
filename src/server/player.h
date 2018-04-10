#ifndef PLAYER_H
#define PLAYER_H

#include "../common/common_interface.h"
#include "../common/utils.h"

struct player
{
    unsigned int id;
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
struct player *player__init(unsigned int id, void *lib_ptr, void *get_name_ptr, void *init_ptr,
                            void *play_ptr, void *final_ptr);

/**
 * @brief Release memory of a player
 * @param p the player
 */
void player__free(struct player *p);

#endif