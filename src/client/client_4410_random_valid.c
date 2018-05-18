#include "../common/interface.h"
#include "common_clients_functions/client.h"
#include <stdio.h>

struct client client;

/////////////////////////////////////////////////////////
/// Implementation of interface functions from client
/////////////////////////////////////////////////////////

struct move client__choose_move_strategy(struct micro_board *board, unsigned int nb_meeples, struct set *possible_moves)
{
    //CLIENT 4410 - RANDOM STRATEGY
    (void) board; //not used in this strategy
    (void) nb_meeples; //not used in this strategy

    assert_not_null(possible_moves, __func__ , "possible_moves parameter");

    if (set__size(possible_moves) == 0)
        exit_on_error("There are no valid possibilities");

    size_t r = rand() % set__size(possible_moves);
    return *((struct move *)set__get_umpteenth_no_copy(possible_moves, r));
}


/////////////////////////////////////////////////////////
/// Implementation of interface functions from common
/////////////////////////////////////////////////////////

char const* get_player_name()
{
    return("client4410-random-valid");
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
    struct set *possible_moves = set__empty(move__copy_op, move__delete_op, move__compare_op, move__debug_op);
    client__populate_possible_moves_list(&client, possible_moves, card);
    struct move m = client__choose_move_strategy(client.board, client.nb_meeples, possible_moves);
    set__free(possible_moves);

    return m;
}

void finalize()
{
    printf(CLI_PREF"Finalizing client named: %s..."CLR"\n", get_player_name());
    client__free(&client);
}
