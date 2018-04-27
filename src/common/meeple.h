#ifndef MEEPLE_H
#define MEEPLE_H

#include "common_interface.h"
#include "card.h"

#define MAX_MEEPLES 8

////////////////////////////////////////////////////////////////////
///     STRUCTURES
////////////////////////////////////////////////////////////////////

/**
 * The structure meeple contains information
 * on the meeple's player, position and belonging_area
 */
struct meeple
{
    unsigned int player_id;
    struct card *related_card;
    enum place position;
    enum area_type belonging_area;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize a meeple with the type card_type
 * @param player_id the player owning the meeple
 * @param card the card where meeple is placed
 * @param place the place on the card
 * @return a pointer towards a newly created meeple
 */
struct meeple *meeple__init(unsigned int player_id, struct card *card, enum place place);


/**
 * @brief Release memory allocated to the meeple structure
 * @param m the meeple to free
 */
void meeple__free(struct meeple *m);

#endif