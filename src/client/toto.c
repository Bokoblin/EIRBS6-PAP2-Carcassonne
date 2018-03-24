#include "../common/projCarcassonneInterface.h"
#include <stdio.h>

/////////////////////////////////////////////////////////
/// Implementation of interface functions
/////////////////////////////////////////////////////////

char const* get_player_name()
{
    return "toto";
}

void initialize(unsigned int id, unsigned int n_players)
{
    //TODO: Is it a client function?
}

struct move play(enum card_id card, struct move const previous_moves[], size_t n_moves)
{
    //TODO
};

/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize()
{
    //TODO: Is it a client function?
}

int main()
{
    char const *A = get_player_name();
    printf("%s\n", A);
    return 0;
}
