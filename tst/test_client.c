#include "common_tests_utils.h"
#include "../src/client/common_clients_functions/client.h"
#include "../src/common/com_func_ptrs.h"


int test_client__client_init()
{
    printf("%s... ", __func__);

    struct client cli;
    client__init(&cli, 4, 2);

    if ((cli.id == 4)
            && (cli.nb_players == 2)
            && (cli.nb_meeples == MAX_MEEPLES)
            && (set__size(cli.board->cards_set) == 1)
            && (cli.board->first_card != NULL)
            && (cli.board->first_card->type.id == FIRST_CARD_ID)) {
        client__free(&cli);
        return TEST_SUCCESS;
    }

    client__free(&cli);

    return !TEST_SUCCESS;
}


int test_client__client_update_board_valid_moves()
{
    printf("%s... ", __func__);

    struct client cl;
    unsigned int nb_players = 3;
    client__init(&cl, 4, nb_players);

    struct move const mv1 = {VALID, 1, CARD_CITY_TUNNEL, {1, 0}, EAST, NO_MEEPLE};
    struct move const mv2 = {VALID, 2, CARD_PLAIN_TWO_CITIES, {1, -1}, NORTH, NO_MEEPLE};
    struct move const mv3 = {VALID, 3, CARD_ROAD_STRAIGHT, {0, -1}, NORTH, NO_MEEPLE};
    struct move const previous_moves[] = {mv1, mv2, mv3};

    if (set__size(cl.board->cards_set) != 1) {
        client__free(&cl);
        return !TEST_SUCCESS;
    }

    client__update_board(&cl, previous_moves, nb_players);

    if (set__size(cl.board->cards_set) != 4) {
        client__free(&cl);
        return !TEST_SUCCESS;
    }

    client__free(&cl);

    return TEST_SUCCESS;
}

int test_client__client_update_board_invalid_moves()
{
    printf("%s... ", __func__);

    struct client cl;
    unsigned int nb_players = 2;
    client__init(&cl, 4, nb_players);

    struct move const mv1 = {FAILED, 1, CARD_CITY_TUNNEL, {1, 0}, EAST, NO_MEEPLE};
    struct move const mv2 = {FAILED, 2, CARD_PLAIN_TWO_CITIES, {2, 0}, NORTH, NO_MEEPLE};
    struct move const previous_moves[] = {mv1, mv2};

    if (set__size(cl.board->cards_set) != 1) {
        client__free(&cl);
        return !TEST_SUCCESS;
    }

    client__update_board(&cl, previous_moves, nb_players);

    if (set__size(cl.board->cards_set) != 1) {
        client__free(&cl);
        return !TEST_SUCCESS;
    }

    client__free(&cl);

    return TEST_SUCCESS;
}


int test_client__client_update_board_mixed_moves()
{
    printf("%s... ", __func__);

    struct client cl;
    unsigned int nb_players = 3;
    client__init(&cl, 4, nb_players);

    struct move const mv1 = {VALID, 1, CARD_CITY_TUNNEL, {1, 0}, EAST, NO_MEEPLE};
    struct move const mv2 = {FAILED, 2, CARD_PLAIN_TWO_CITIES, {2, 0}, NORTH, NO_MEEPLE};
    struct move const mv3 = {VALID, 3, CARD_ROAD_STRAIGHT, {0, -1}, NORTH, NO_MEEPLE};
    struct move const previous_moves[] = {mv1, mv2, mv3};

    if (set__size(cl.board->cards_set) != 1) {
        client__free(&cl);
        return !TEST_SUCCESS;
    }

    set__debug(cl.board->cards_set, false);
    client__update_board(&cl, previous_moves, nb_players);
    set__debug(cl.board->cards_set, false);

    if (set__size(cl.board->cards_set) != 3) {
        client__free(&cl);
        return !TEST_SUCCESS;
    }

    client__free(&cl);

    return TEST_SUCCESS;
}


int test_client__client_populate_possible_moves_list()
{
    printf("%s... ", __func__);

    struct client cl;
    unsigned int nb_players = 3;
    client__init(&cl, 4, nb_players);

    struct move const mv1 = {VALID, 1, CARD_CITY_TUNNEL, {1, 0}, EAST, NO_MEEPLE};
    struct move const mv2 = {FAILED, 2, CARD_PLAIN_TWO_CITIES, {2, 0}, NORTH, NO_MEEPLE};
    struct move const mv3 = {VALID, 3, CARD_ROAD_STRAIGHT, {0, -1}, NORTH, NO_MEEPLE};
    struct move const previous_moves[] = {mv1, mv2, mv3};

    client__update_board(&cl, previous_moves, nb_players);

    struct set *possible_moves = set__empty(move_copy_op, move_delete_op, move_compare_op, move_debug_op);
    client__populate_possible_moves_list(&cl, possible_moves, CARD_PLAIN_CITY_ROAD);

    if (set__is_empty(possible_moves)) {
        client__free(&cl);
        set__free(possible_moves);
        return !TEST_SUCCESS;
    }

    client__free(&cl);
    set__free(possible_moves);

    return TEST_SUCCESS;
}


int test_client__client_debug()
{
    printf("%s... ", __func__);

    struct client cli;
    client__init(&cli, 4, 2);

    client__debug(&cli);

    client__free(&cli);

    return TEST_SUCCESS;
}


int main()
{
    printf("----------- TEST CLIENT -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_client__client_init(), &nb_success, &nb_tests);
    print_test_result(test_client__client_update_board_valid_moves(), &nb_success, &nb_tests);
    print_test_result(test_client__client_update_board_invalid_moves(), &nb_success, &nb_tests);
    print_test_result(test_client__client_update_board_mixed_moves(), &nb_success, &nb_tests);
    print_test_result(test_client__client_populate_possible_moves_list(), &nb_success, &nb_tests);
    print_test_result(test_client__client_debug(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}