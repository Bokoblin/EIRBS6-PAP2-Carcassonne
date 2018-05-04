#include "common_tests_utils.h"
#include "../src/common/utils.h"
#include "../src/client/client.h"
#include "../src/common/card.h"


int test_client__client_init()
{
    printf("%s... ", __func__);

    struct client *cl = safe_malloc(sizeof(struct client));
    client__init(cl, 4, 2);

    if ((cl->id == 4)
     && (cl->nb_players == 2)
     && (cl->nb_meeples == MAX_MEEPLES)
     && (cl->client_board->moves_queue == NULL)
     && (cl->client_board->drawing_stack == NULL)
     && (set__size(cl->client_board->cards_set) == 1)
     && (cl->client_board->first_card != NULL)
     && (cl->client_board->first_card->type.id == CARD_JUNCTION_CITY) ) {
        free(cl);
        return TEST_SUCCESS;
    }

    client__free(cl);
    return !TEST_SUCCESS;

}


int test_client__client_update_board()
{
    printf("%s... ", __func__);

    struct client *cl = safe_malloc(sizeof(struct client));
    client__init(cl, 4, 2);

    struct board *b = board__init();
    struct card *c = card__init(CARD_CITY_ALL_SIDES);
    enum card_id ci = CARD_CITY_ALL_SIDES;
    stack__push(b->drawing_stack, &ci);
    board__init_first_card(b);

    struct move const mv1 = {VALID, 1, CARD_CITY_TUNNEL, {0, 1}, NORTH, NO_MEEPLE};
    struct move const mv2 = {VALID, 2, CARD_PLAIN_TWO_CITIES, {0, -1}, NORTH, NO_MEEPLE};
    struct move const previous_moves[] = {mv1, mv2};

    if (!client__update_board(cl, previous_moves, 2)) {
        client__free(cl);
        card__free(c);
        board__free(b);
        return !TEST_SUCCESS;
    }

    if (b->first_card->type.id != CARD_CITY_ALL_SIDES) {
        client__free(cl);
        card__free(c);
        board__free(b);
        return !TEST_SUCCESS;
    }

    //TODO : to finish --> check if neighbors card(mv1) and card(mv2) have been added to neighbors to c
    //set__debug_data(cl->client_board->cards_set, 0);
    if (set__size(cl->client_board->cards_set) != 3) { //FIXME: Only one card have been added
        client__free(cl);
        card__free(c);
        board__free(b);
        return !TEST_SUCCESS;
    }

    client__free(cl);
    card__free(c);
    board__free(b);

    return TEST_SUCCESS;
}


int test_client__client_play_card_success_case()
{
    printf("%s... ", __func__);

    struct client *cl = safe_malloc(sizeof(struct client));
    client__init(cl, 4, 2);

    struct move mv = client__play_card(cl, CARD_ROAD_TURN_LEFT_CITY);

    if (mv.check == VALID) {
        client__free(cl);
        return TEST_SUCCESS;
    }

    client__free(cl);

    return !TEST_SUCCESS;
}


int main()
{
    printf("----------- TEST CLIENT -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_client__client_init(), &nb_success, &nb_tests);
    print_test_result(test_client__client_update_board(), &nb_success, &nb_tests);
    print_test_result(test_client__client_play_card_success_case(), &nb_success, &nb_tests);

    return TEST_SUCCESS;
}
