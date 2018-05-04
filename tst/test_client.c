#include "common_tests_utils.h"
#include "../src/common/utils.h"
#include "../src/client/client.h"
#include "../src/common/card.h"


int test_client__client_init()
{
    printf("%s... ", __func__);

    struct client cli;
    client__init(&cli, 4, 2);

    if ((cli.id == 4)
     && (cli.nb_players == 2)
     && (cli.nb_meeples == MAX_MEEPLES)
     && (set__size(cli.client_board->cards_set) == 1)
     && (cli.client_board->first_card != NULL)
     && (cli.client_board->first_card->type.id == CARD_JUNCTION_CITY)) {
        client__free(&cli);
        return TEST_SUCCESS;
    }

    client__free(&cli);

    return !TEST_SUCCESS;
}


int test_client__client_update_board()
{
    printf("%s... ", __func__);

    struct client cl;
    client__init(&cl, 4, 3);

    struct move const mv1 = {VALID, 1, CARD_CITY_TUNNEL, {1, 0}, WEST, NO_MEEPLE};
    struct move const mv2 = {VALID, 2, CARD_PLAIN_TWO_CITIES, {2, 0}, NORTH, NO_MEEPLE};
    struct move const mv3 = {VALID, 3, CARD_ROAD_STRAIGHT, {0, -1}, NORTH, NO_MEEPLE};
    struct move const previous_moves[] = {mv1, mv2, mv3};

    client__update_board(&cl, previous_moves, 3);

    set__debug_data(cl.client_board->cards_set, false);
    if (set__size(cl.client_board->cards_set) != 4) {
        client__free(&cl);
        return !TEST_SUCCESS;
    }

    client__free(&cl);

    return TEST_SUCCESS;
}


int test_client__client_play_card_success_case()
{
    printf("%s... ", __func__);

    struct client cli;
    client__init(&cli, 4, 2);

    struct move mv = client__play_card(&cli, CARD_ROAD_TURN_LEFT_CITY);

    if (mv.check == VALID) {
        client__free(&cli);
        return TEST_SUCCESS;
    }

    client__free(&cli);

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

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}