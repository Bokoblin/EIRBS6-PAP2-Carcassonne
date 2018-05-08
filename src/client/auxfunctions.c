#include "../common/common_interface.h"
#include "client.h"
#include "../server/board.h"
#include "../common/meeple.h"
#include "../common/com_func_ptrs.h"

/////////////////////////////////////////////////////////
/// Structures and functions needed by play_card()
/////////////////////////////////////////////////////////

struct triplet_card_direction
{
    struct card* umpteenth_card;
    enum card_side umpteenth_card_side;
    enum direction drawn_card_north_direction;
};

void* tcd_copy_op(struct triplet_card_direction *tcd)
{
    assert_not_null(tcd, __func__, "tcd parameter");

    struct triplet_card_direction *new_tcd = safe_malloc(sizeof(struct triplet_card_direction));
    new_tcd->umpteenth_card = tcd->umpteenth_card;
    new_tcd->umpteenth_card_side = tcd->umpteenth_card_side;
    new_tcd->drawn_card_north_direction = tcd->drawn_card_north_direction;

    return new_tcd;
}

void tcd_delete_op(struct triplet_card_direction *tcd)
{
    free(tcd);
}

int tcd_compare_op(struct triplet_card_direction* tcd1, struct triplet_card_direction* tcd2)
{
    assert_not_null(tcd1, __func__, "tcd1 parameter");
    assert_not_null(tcd2, __func__, "tcd2 parameter");

    if (card_compare_op(tcd1->umpteenth_card, tcd2->umpteenth_card) == 0) {
        if (tcd1->umpteenth_card_side < tcd2->umpteenth_card_side)
            return -1;
        else if (tcd1->umpteenth_card_side == tcd2->umpteenth_card_side)
            return 0;
        else
            return 1;
    }
    else
        return card_compare_op(tcd1->umpteenth_card, tcd2->umpteenth_card) == 0;
}

void tcd_debug_op(struct triplet_card_direction *tcd)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (tcd == NULL)
        printf("NULL");
    else
        printf("\tTCD (umpteenth card ptr: %p, umpteenth card side: %d, drawn card direction: %d)\n",
               tcd->umpteenth_card, tcd->umpteenth_card_side, tcd->drawn_card_north_direction);
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

    //Initialization of the first card of the board
    enum card_id ci_first = CARD_ROAD_STRAIGHT_CITY;
    stack__push(client->client_board->drawing_stack, &ci_first);
    board__init_first_card(client->client_board);
}

void client__free(struct client *client)
{
    board__free(client->client_board);
    //note: don't free client, it isn't dynamically allocated
}

void client__update_board(struct client *client, struct move const previous_moves[], size_t n_moves)
{
    assert_not_null(client, __func__, "client");
    assert_not_null(client->client_board, __func__, "client->client_board");
    assert_not_null(previous_moves, __func__, "moves parameter");

    for (size_t i = 0; i < n_moves; i++) {
        struct move move = previous_moves[i];
        if (move.check == VALID) {
            struct card *c = card__init(move.card);
            c->pos = move.onto;
            c->direction = move.dir;
            board__add_card(client->client_board, c);
            card__free(c);
        } else {
            client->nb_players--;
        }
    }
}


struct move client__play_card(struct client *client, enum card_id card)
{
    assert_not_null(client, __func__ , "on client parameter");

    if (card == LAST_CARD)
        exit_on_error("Card is invalid");

    struct set *possible_cards = set__empty(tcd_copy_op, tcd_delete_op, tcd_compare_op, tcd_debug_op);
    struct card *drawn_card = card__init(card);

    //=== Add all pairs of card and possible direction where drawn card can be linked

    //This is done for each card of the set, for each direction of drawn card, for each side of card1 and card2
    // => 4*4*4*number of cards in the board having less than 4 neighbours

    size_t length = set__size(client->client_board->cards_set);
    for (size_t set_it = 0; set_it < length; set_it++) {
        struct card *umpteenth_card = set__get_umpteenth_no_copy(client->client_board->cards_set, set_it);

        if (card__get_neighbour_number(umpteenth_card) < SIDES_NUMBER) {
            for (int i_drawn_card_side = 0; i_drawn_card_side < SIDES_NUMBER; i_drawn_card_side++) {
                for (int i_umpteenth_card_side = 0; i_umpteenth_card_side < SIDES_NUMBER; i_umpteenth_card_side++) {
                    for (int dir_index = 0; dir_index < DIRECTION_NUMBER; dir_index++) {
                        drawn_card->direction = (enum direction) dir_index;

                        if (card__are_matching_sides(drawn_card, umpteenth_card, (enum card_side) i_drawn_card_side,
                                                     (enum card_side) i_umpteenth_card_side)
                                && umpteenth_card->neighbors[i_umpteenth_card_side] == NULL) {
                            struct triplet_card_direction tcd;
                            tcd.umpteenth_card = umpteenth_card;
                            tcd.umpteenth_card_side = (enum card_side) i_umpteenth_card_side;
                            tcd.drawn_card_north_direction = (enum direction) dir_index;
                            set__add(possible_cards, &tcd);
                        }
                    }
                }
            }
        }
    }

    //=== Choose a card among possible ones

    if (set__size(possible_cards) == 0)
        exit_on_error("It's impossible to obtain no valid cards as server must send valid cards");

    struct triplet_card_direction *chosen_tcd = set__get_umpteenth_no_copy(possible_cards,
            rand() % set__size(possible_cards));

    //=== Build move with chosen card in board, direction and direction

    assert_not_null(chosen_tcd, __func__ , "chosen_tcd");
    assert_not_null(chosen_tcd->umpteenth_card, __func__ , "chosen_tcd->umpteenth_card");

    struct move played_move;
    played_move.check = VALID;
    played_move.player = client->id;
    played_move.card = card;
    played_move.onto = card__get_position_at_side(chosen_tcd->umpteenth_card, chosen_tcd->umpteenth_card_side);
    played_move.dir = chosen_tcd->drawn_card_north_direction;
    played_move.place = NO_MEEPLE;


    card__free(drawn_card);
    set__free(possible_cards);

    return played_move;
}

void client__debug(struct client *client)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    printf("=== Player Debug ===\n");
    printf("Client: client id: %d, player number: %d, meeple number: %d\n",
           client->id, client->nb_players, client->nb_meeples);
    printf("Board: first card: %d, sets and queue: \n", client->client_board->first_card->type.id);
    printf("Card set: ");
    set__debug_data(client->client_board->cards_set, false);
    printf("Meeple set: ");
    set__debug_data(client->client_board->meeples_set, false);
}
