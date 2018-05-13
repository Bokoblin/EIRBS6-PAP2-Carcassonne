#include "client.h"
#include "../../common/utils.h"
#include "../../common/ADT/stack.h"

void client__init(struct client *client, unsigned int id, unsigned int n_players)
{
    client->id = id;
    client->nb_players = n_players;
    client->nb_meeples = MAX_MEEPLES;
    client->board = micro_board__init();
}

void client__free(struct client *client)
{
    micro_board__free(client->board);
    //note: don't free client, it isn't dynamically allocated
}

void client__update_board(struct client *client, struct move const previous_moves[], size_t n_moves)
{
    assert_not_null(client, __func__, "client");
    assert_not_null(client->board, __func__, "client->board");
    assert_not_null(previous_moves, __func__, "moves parameter");

    for (size_t i = 0; i < n_moves; i++) {
        struct move move = previous_moves[i];
        if (move.check == VALID) {
            struct card *c = card__init(move.card);
            c->pos = move.onto;
            c->direction = move.dir;
            if (micro_board__is_card_adding_possible(client->board, c)) {
                if (micro_board__add_card_to_board(client->board, c) != SUCCESS) {
                    card__debug_op(c);
                    set__debug(client->board->cards_set, false);
                    exit_on_error("\n The card sent by the server hasn't been added");
                }
            } else {
                card__debug_op(c);
                set__debug(client->board->cards_set, false);
                exit_on_error("\nThe card sent my the server can't be added");
            }
            card__free(c);
        } else {
            client->nb_players--;
        }
    }
}

struct move move__init(unsigned int player_id, enum card_id ci, struct position onto, enum direction north_dir)
{
    struct move m;
    m.check = VALID;
    m.player = player_id;
    m.card = ci;
    m.onto = onto;
    m.dir = north_dir;
    m.place = NO_MEEPLE;

    return m;
}

void client__populate_possible_moves_list(struct client *c, struct set *possible_moves, enum card_id ci)
{
    assert_not_null(c, __func__ , "on b parameter");

    if (ci == LAST_CARD)
        exit_on_error("Card is invalid");

    struct card *drawn_card = card__init(ci);

    //=== Populate valid moves following current board setup

    size_t length = set__size(c->board->cards_set);
    for (size_t set_it = 0; set_it < length; set_it++) {
        struct card *umpteenth = set__get_umpteenth_no_copy(c->board->cards_set, set_it);

        if (card__get_neighbour_number(umpteenth) < NB_DIRECTIONS) {
            //Foreach orientation chosen for the drawn card, we look for matches
            for (enum direction i_north_dir = NORTH; i_north_dir < NB_DIRECTIONS; i_north_dir++) {
                drawn_card->direction = i_north_dir;
                //Foreach directions around neighbour, we look for matches
                for (enum direction i_drawn_dir = NORTH; i_drawn_dir < NB_DIRECTIONS; i_drawn_dir++) {
                    enum direction i_umpteenth_dir = (i_drawn_dir + 2) % NB_DIRECTIONS;

                    if (card__are_matching_directions(drawn_card, umpteenth, i_drawn_dir, i_umpteenth_dir)
                            && umpteenth->neighbors[i_umpteenth_dir] == NULL) {
                        drawn_card->pos = card__get_position_at_direction(umpteenth, i_umpteenth_dir);

                        //We check for a full match
                        if (micro_board__is_card_adding_possible(c->board, drawn_card)) {
                            struct position onto = card__get_position_at_direction(umpteenth, i_umpteenth_dir);
                            struct move m = move__init(c->id, ci, onto, i_north_dir);
                            set__add(possible_moves, &m);
                        }
                    }
                }
            }
        }
    }

    card__free(drawn_card);
}

void client__debug(struct client *client)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    printf("=== Player Debug ===\n");
    printf("Client: client id: %d, player number: %d, meeple number: %d\n",
           client->id, client->nb_players, client->nb_meeples);
    printf("Board: first card: %d and card set: \n", client->board->first_card->type.id);
    printf("Card set: ");
    set__debug(client->board->cards_set, false);
}
