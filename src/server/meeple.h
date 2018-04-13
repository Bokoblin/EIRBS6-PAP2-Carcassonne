#ifndef MEEPLE_H
#define MEEPLE_H

/**
 * The structure meeple contains information
 * on the meeple's player, position and belonging_area
 */
struct meeple
{
    unsigned int player_id;
    enum place position;
    enum area_type belonging_area;
};

#endif