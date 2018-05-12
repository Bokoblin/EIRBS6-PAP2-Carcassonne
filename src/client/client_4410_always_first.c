#include "../common/common_interface.h"
#include "../common/com_func_ptrs.h"
#include "common_clients_functions/client.h"
#include <stdio.h>

struct client client;

/////////////////////////////////////////////////////////
/// Implementation of interface functions from client
/////////////////////////////////////////////////////////

struct move client__chose_move_strategy(struct set *possible_moves)
{
    //CLIENT 4410 - ALWAYS FIRST STRATEGY

    assert_not_null(possible_moves, __func__ , "on possible_moves parameter");

    if (set__size(possible_moves) == 0)
        exit_on_error("There are no valid possibilities");

    return *((struct move *)set__get_umpteenth_no_copy(possible_moves, 0));
}


/////////////////////////////////////////////////////////
/// Implementation of interface functions from common
/////////////////////////////////////////////////////////

char const* get_player_name()
{
    return("client4410-always_first");
}

void initialize(unsigned int id, unsigned int n_players)
{
    printf(CLI_PREF"Initializing client named: %s..."CLR"\n", get_player_name());
    client__init(&client, id, n_players);
}

struct move play(enum card_id card, struct move const previous_moves[], size_t n_moves)
{
    printf(CLI_PREF"Executing play for client named: %s..."CLR"\n", get_player_name());

    //=== Updating client own board
    client__update_board(&client, previous_moves, n_moves);

    //=== Choosing next move following board and drawn card
    struct set *possible_moves = set__empty(move_copy_op, move_delete_op, move_compare_op, move_debug_op);
    client__populate_possible_moves_list(&client, possible_moves, card);
    struct move m = client__chose_move_strategy(possible_moves);
    set__free(possible_moves);

    return m;
}

void finalize()
{
    printf(CLI_PREF"Finalizing client named: %s..."CLR"\n", get_player_name());
    client__free(&client);
}
