#include <dlfcn.h>
#include <string.h>
#include <assert.h>
#include "player_container.h"
#include "../common/common_interface.h"
#include "../common/queue.h"
#include "../common/stack.h"
#include "../common/card.h"

#define DEFAULT_GRAPHIC_MODE_FLAG 0
#define DEFAULT_CLIENT_COUNT 0

////////////////////////////////////////////////////////////////////
///     FUNCTIONS DECLARATION
////////////////////////////////////////////////////////////////////

void register_players(int argc, const char **argv, const struct player_container *players, unsigned int nb_players);
struct player* compute_next_player(struct player_container *players);
struct move *build_previous_moves_array(struct queue *moves, unsigned int nb_moves);
void free_resources(const struct player_container *players);
void game_main(struct player_container *players);


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR CARD STACK
////////////////////////////////////////////////////////////////////

void* card_stack_copy_operator(void *given_card)
{
    enum card_id *_given_card = given_card;
    enum card_id *new_card = malloc(sizeof(enum card_id));
    *new_card = *_given_card;
    return new_card;
}

void card_stack_delete_operator(void* given_card)
{
    free(given_card);
}

void card_stack_debug_operator(void* given_card)
{
    enum card_id *_given_card = given_card;
    printf("%d", *_given_card);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS FOR MOVES QUEUE
////////////////////////////////////////////////////////////////////

void* move_queue_copy_operator(struct move* m)
{
    struct move *new_move = malloc(sizeof(struct move));
    new_move->card = m->card;
    new_move->check = m->check;
    new_move->dir = m->dir;
    new_move->onto = m->onto;
    new_move->place = m->place;
    new_move->player = m->player;

    return new_move;
}

void move_queue_delete_operator(struct move* m)
{
    free(m);
}

void move_queue_debug_operator(struct move* m)
{
    printf("=== Display move ===\n");
    printf("\tplayer id: %d\n", m->player);
    printf("\tcard id: %d\n", m->card);
    printf("\tposition: {%d, %d}\n", m->onto.x, m->onto.y);
    printf("\tdirection: %d\n", m->dir);
    printf("\tplace: %d\n", m->place);
}


////////////////////////////////////////////////////////////////////
///     FUNCTIONS IMPLEMENTATION & MAIN
////////////////////////////////////////////////////////////////////

/**
 * @brief Registers all player libraries passed in program arguments
 * @param argc argument count
 * @param argv argument content array
 * @param players the players container
 * @param nb_players the number of players to register
 */
void register_players(int argc, const char **argv, const struct player_container *players, unsigned int nb_players)
{
    unsigned int nb_players_registered = 0;
    int args_index = 0;

    while (nb_players_registered < nb_players && args_index < argc) {
        const char* current_arg = argv[args_index];
        if (strstr(current_arg, ".so") != NULL) {
            printf("Registering: %s\n", current_arg);
            players->player_array[nb_players_registered]->lib_ptr = dlopen(current_arg, RTLD_NOW);
            assert_no_dlerror();
            players->player_array[nb_players_registered]->get_player_name
                    = dlsym(players->player_array[nb_players_registered]->lib_ptr, "get_player_name");
            assert_no_dlerror();
            players->player_array[nb_players_registered]->initialize
                    = dlsym(players->player_array[nb_players_registered]->lib_ptr, "initialize");
            assert_no_dlerror();
            players->player_array[nb_players_registered]->play
                    = dlsym(players->player_array[nb_players_registered]->lib_ptr, "play");
            assert_no_dlerror();
            players->player_array[nb_players_registered]->finalize
                    = dlsym(players->player_array[nb_players_registered]->lib_ptr, "finalize");
            assert_no_dlerror();

            nb_players_registered++;
        }
        args_index++;
    }
}

struct player* compute_next_player(struct player_container *players)
{
    assert(players->player_array != NULL);
    players->current_player = (players->current_player + 1) % players->current_size;
    return players->player_array[players->current_player];
}

void game_main(struct player_container *players)
{
    //=== Card stack initialization

    struct stack* drawing_stack =
            stack__empty(card_stack_copy_operator, card_stack_delete_operator, card_stack_debug_operator);

    for (unsigned int i = 0; i < CARD_NUMBER; i++) {
        //FIXME: Init with accurate card repartition following rules
        //+ shuffle
        struct card_type ct1 = {CARD_MONASTERY_ALONE,
                {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ABBEY}};
        struct card *c = card__empty(ct1);
        stack__push(drawing_stack, c);
    }

    //=== Moves queue initialization

    struct queue* moves = queue__empty(move_queue_copy_operator, move_queue_delete_operator, move_queue_debug_operator);

    //=== Player initialization

    for (unsigned int i = 0; i < players->current_size; i++) {
        players->player_array[i]->initialize(i, players->current_size);
    }

    //=== Game loop

    while (!stack__is_empty(drawing_stack) && players->current_size > 0) {
        enum card_id c = card__draw(drawing_stack);
        struct player *p = compute_next_player(players);
        struct move *moves_array = build_previous_moves_array(moves, players->current_size);
        queue__dequeue(moves);
        struct move m = p->play(c, moves_array, players->current_size);
        queue__enqueue(moves, &m);
        free(moves_array);
    }

    //=== Players finalization

    for (size_t i = 0; i < players->current_size; i++) {
        players->player_array[i]->finalize();
    }

    stack__free(drawing_stack);
    queue__free(moves);
}

struct move *build_previous_moves_array(struct queue *moves, unsigned int nb_moves)
{
    if (moves == NULL)
        exit_on_error("NULL moves queue");

    struct move *moves_array = malloc(sizeof(struct move) * nb_moves);
    if (moves_array == NULL)
        exit_on_error("Malloc failure on struct move*");

    if (!queue__is_empty(moves)) {
        for (unsigned int i = 0; i < nb_moves; i++) {
            struct move *m = (struct move*) queue__dequeue(moves);
            moves_array[i] = *m;
            queue__enqueue(moves, m);
        }
    }

    return moves_array;
}

void free_resources(const struct player_container *players)
{
    for (size_t i = 0; i < players->current_size; i++) {
        if (players->player_array[i]->lib_ptr != NULL) {
            dlclose(players->player_array[i]->lib_ptr);
            assert_no_dlerror();
        }
    }
}

int main(int argc, char** argv)
{
    unsigned int is_graphic = DEFAULT_GRAPHIC_MODE_FLAG;
    unsigned int clients_count = DEFAULT_CLIENT_COUNT;
    parse_opts(argc, argv, &is_graphic, &clients_count);


    //=== Register players

    struct player_container *players = init_player_container(clients_count);
    register_players(argc, (const char **) argv, players, clients_count);

    //=== Check players initialization

    for (size_t i = 0; i < players->current_size; i++) {
        assert(players->player_array[i]->initialize != NULL);
        assert(players->player_array[i]->play != NULL);
        assert(players->player_array[i]->get_player_name != NULL);
        assert(players->player_array[i]->finalize != NULL);
    }

    //=== Display players name

    if (players->current_size > 0)
        printf("Display all players:\n");
    for (size_t i = 0; i < players->current_size; i++) {
        printf("\t- PLAYER#%d: %s\n", (int) (i+1), players->player_array[i]->get_player_name());
    }

    //=== Start the game

    game_main(players);

    //=== Free used resources & memory

    free_resources(players);
    free_player_container(players);

    return EXIT_SUCCESS;
}
