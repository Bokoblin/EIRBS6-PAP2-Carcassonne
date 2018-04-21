#include "../common/common_interface.h"
#include <stdio.h>

/////////////////////////////////////////////////////////
/// Implementation of interface functions
/////////////////////////////////////////////////////////

char const* get_player_name()
{
    return("client4410-2");
}

void initialize(unsigned int id, unsigned int n_players)
{
    (void) id;
    (void) n_players;
    printf("\x1B[35m[CLIENT] Executing placeholder %s::initialize()...\x1B[0m\n", get_player_name());
}

struct move play(enum card_id card, struct move const previous_moves[], size_t n_moves)
{
    (void) card;
    (void) previous_moves;
    (void) n_moves;
    printf("\x1B[35m[CLIENT] Executing placeholder %s::play()...\x1B[0m\n", get_player_name());

    struct move move_placeholder = { FAILED, 1, card, { 0, 1 }, SOUTH, POS_WEST_NORTH}; //small chance to work on first call
    return move_placeholder;
}


void finalize()
{
    printf("\x1B[35m[CLIENT] Executing placeholder %s::finalize()...\x1B[0m\n", get_player_name());
}
