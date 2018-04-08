#ifndef PLAYER_CONTAINER_H
#define PLAYER_CONTAINER_H

#include "../common/common_interface.h"
#include "../common/utils.h"
#include "player.h"

/**
 * @brief players list structure
 */
struct player_container
{
    struct player** player_array;
    unsigned int current_size;
    unsigned int current_player;
};

/**
 * @brief Inits the players structure
 * @param nb_player the number of players to add
 * @return a newly created player container
 */
struct player_container* init_player_container(unsigned int nb_player);


/**
 * @brief Release memory of the container
 * @param container the player container
 */
void free_player_container(struct player_container *container);

#endif