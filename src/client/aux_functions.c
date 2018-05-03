#include "../common/common_interface.h"
#include "client.h"
#include "../server/board.h"
#include "../common/meeple.h"
#include "../common/com_func_ptrs.h"

/////////////////////////////////////////////////////////
/// Structures and functions needed by play_card()
/////////////////////////////////////////////////////////

struct triple_card_direction
{
    struct card* card_in_board;
    enum direction card_in_board_direction;
    enum orientation drawn_card_orientation;
};

void* tcd_copy_op(struct triple_card_direction *tcd)
{
    assert_not_null(tcd, __func__, "m parameter");

    struct triple_card_direction *new_tcd = safe_malloc(sizeof(struct triple_card_direction));
    new_tcd->card_in_board = tcd->card_in_board;
    new_tcd->card_in_board_direction = tcd->card_in_board_direction;
    new_tcd->drawn_card_orientation = tcd->drawn_card_orientation;

    return new_tcd;
}

void tcd_delete_op(struct triple_card_direction *tcd)
{
    free(tcd);
}

int tcd_compare_op(struct triple_card_direction* tcd1, struct triple_card_direction* tcd2)
{
    assert_not_null(tcd1, __func__, "tcd1 parameter");
    assert_not_null(tcd2, __func__, "tcd2 parameter");

    if (card_compare_op(tcd1->card_in_board, tcd2->card_in_board) == 0) {
        if (tcd1->card_in_board_direction < tcd2->card_in_board_direction)
            return -1;
        else if (tcd1->card_in_board_direction == tcd2->card_in_board_direction)
            return 0;
        else
            return 1;
    }
    else
        return card_compare_op(tcd1->card_in_board, tcd2->card_in_board) == 0;
}

void tcd_debug_op(struct triple_card_direction *tcd)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (tcd == NULL)
        printf("NULL");
    else
        printf("TCD (card in board ptr: %p, card in board direction: %d, drawn card orientation: %d)",
               tcd->card_in_board, tcd->card_in_board_direction, tcd->drawn_card_orientation);
}


/////////////////////////////////////////////////////////
/// Implementation of client useful functions
/////////////////////////////////////////////////////////

void client__init(struct client *client, unsigned int id, unsigned int n_players)
{
    client->id = id;
    client->nb_players = n_players;
    client->nb_meeples = MAX_MEEPLES;
    client->client_board = board__init();
}

void client__free(struct client *client)
{
  board__free(client->client_board);
}

int client__update_board(struct client *client, struct move const previous_moves[], size_t n_moves)
{
    assert_not_null(previous_moves, __func__, "moves parameter");

    for (size_t i = 0 ;i < n_moves; i++){
        struct move move = previous_moves[i];
        if (move.check == VALID){
            struct card *c = card__init(move.card);
            c->pos = move.onto;
            card__set_orientation(c, (enum orientation) move.dir);
            if(board__add_card(client->client_board, c)){
                card__free(c);
                return true;
            }
        }
    }
    return false;
}


struct move client__play_card(struct client *client, enum card_id card)
{
    struct set *possible_cards = set__empty(tcd_copy_op, tcd_delete_op, tcd_compare_op, tcd_debug_op);
    struct card *drawn_card = card__init(card);

    //=== Add all pairs of card and possible direction where drawn card can be linked

    //This is done for each card of the set, for each orientation of drawn card, for each side of card1 and card2
    // => 4*4*4*number of cards already in the board with less than 4 neighbours

    for (size_t i = 0; i < set__size(client->client_board->cards_set); i++) {
        struct card *umpteenth_card = set__get_umpteenth_no_copy(client->client_board->cards_set, i);
        if (card__get_neighbour_number(umpteenth_card) < 4) {
            for (int j = 0; j < DIRECTION_NUMBER; j++) {
                for (int k = 0; k < DIRECTION_NUMBER; k++) {
                    for (int l = 0; l < DIRECTION_NUMBER; l++) {
                        drawn_card->orientation = (enum orientation) l;
                        if (card__are_matching_directions(drawn_card, umpteenth_card, (enum direction) j, (enum direction) k)
                                && umpteenth_card->neighbors[k] == NULL) {
                            struct triple_card_direction *tcd = safe_malloc(sizeof(struct triple_card_direction));
                            tcd->card_in_board = umpteenth_card;
                            tcd->card_in_board_direction = (enum direction) k;
                            tcd->drawn_card_orientation = (enum orientation) l;
                            set__add(client->client_board->cards_set, tcd);
                        }
                    }
                }
            }
        }
    }


    //=== Choose a card among possible ones

    if (set__size(possible_cards) == 0)
        exit_on_error("It's impossible to obtain no valid cards as server must send valid cards");

    struct triple_card_direction *chosen_tcd = set__get_umpteenth_no_copy(client->client_board->cards_set,
            rand() % set__size(possible_cards));

    //=== Build move with chosen card in board, orientation and direction

    struct move played_move;
    played_move.player = client->id;
    played_move.card = card;
    played_move.onto = card__get_position_at_direction(chosen_tcd->card_in_board, chosen_tcd->card_in_board_direction);
    played_move.dir = (enum direction) chosen_tcd->drawn_card_orientation;
    played_move.place = NO_MEEPLE;


    card__free(drawn_card);
    set__free(possible_cards);

    return played_move;
}
