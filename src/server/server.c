#include <dlfcn.h>
#include <string.h>
#include <assert.h>
#include "../server/client_container.h"
#include "../common/common_interface.h"
#include "../common/queue.h"
#include "../common/stack.h"
#include "../common/card.h"

#define DEFAULT_GRAPHIC_MODE_FLAG 0
#define DEFAULT_CLIENT_COUNT 0

////////////////////////////////////////////////////////////////////
///     FUNCTIONS DECLARATION
////////////////////////////////////////////////////////////////////

void register_clients(int argc, const char **argv, const struct client_container *clients, unsigned int clients_count);
struct move *build_previous_moves_array(struct queue *moves, unsigned int nb_moves);
void free_resources(const struct client_container *clients);
void game_main(const struct client_container *players);


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
 * @brief Registers all client libraries passes in program arguments
 * @param argc argument count
 * @param argv argument content array
 * @param clients the clients container
 * @param clients_count the clients to register count
 */
void register_clients(int argc, const char **argv, const struct client_container *clients, unsigned int clients_count)
{
    unsigned int nb_clients_registered = 0;
    int args_index = 0;

    while (nb_clients_registered < clients_count && args_index < argc) {
        const char* current_arg = argv[args_index];
        if (strstr(current_arg, ".so") != NULL) {
            printf("Registering: %s\n", current_arg);
            clients->clients_array[nb_clients_registered]->lib_ptr = dlopen(current_arg, RTLD_NOW);
            assert_no_dlerror();
            clients->clients_array[nb_clients_registered]->get_player_name
                    = dlsym(clients->clients_array[nb_clients_registered]->lib_ptr, "get_player_name");
            assert_no_dlerror();
            clients->clients_array[nb_clients_registered]->initialize
                    = dlsym(clients->clients_array[nb_clients_registered]->lib_ptr, "initialize");
            assert_no_dlerror();
            clients->clients_array[nb_clients_registered]->play
                    = dlsym(clients->clients_array[nb_clients_registered]->lib_ptr, "play");
            assert_no_dlerror();
            clients->clients_array[nb_clients_registered]->finalize
                    = dlsym(clients->clients_array[nb_clients_registered]->lib_ptr, "finalize");
            assert_no_dlerror();

            nb_clients_registered++;
        }
        args_index++;
    }
}

struct client* compute_next_player(const struct client_container *players)
{
    assert(players->clients_array != NULL);
    //TODO (placeholder for now)
    return players->clients_array[0];
}

void game_main(const struct client_container *players)
{
    struct stack* drawing_stack =
            stack__empty(card_stack_copy_operator, card_stack_delete_operator, card_stack_debug_operator);
    struct queue* moves = queue__empty(move_queue_copy_operator, move_queue_delete_operator, move_queue_debug_operator);

    for (unsigned int i = 0; i < players->current_size; i++) {
        players->clients_array[i]->initialize(i, players->current_size);
    }

    while (!stack__is_empty(drawing_stack) && players->current_size > 0) {
        enum card_id c = card__draw(drawing_stack);
        struct client *p = compute_next_player(players);
        struct move *moves_array = build_previous_moves_array(moves, players->current_size);
        queue__dequeue(moves);
        struct move m = p->play(c, moves_array, 0);
        queue__enqueue(moves, &m);
        free(moves_array);
    }

    for (size_t i = 0; i < players->current_size; i++) {
        players->clients_array[i]->finalize();
    }

    stack__free(drawing_stack);
    queue__free(moves);
}

struct move *build_previous_moves_array(struct queue *moves, unsigned int nb_moves)
{
    struct move *moves_array = malloc(sizeof(struct move) * nb_moves);
    for (unsigned int i = 0; i < nb_moves; i++) {
        struct move *m = (struct move*) queue__dequeue(moves);
        moves_array[i] = *m;
        queue__enqueue(moves, m);
    }
    return moves_array;
}

void free_resources(const struct client_container *clients)
{
    for (size_t i = 0; i < clients->current_size; i++) {
        if (clients->clients_array[i]->lib_ptr != NULL) {
            dlclose(clients->clients_array[i]->lib_ptr);
            assert_no_dlerror();
        }
    }
}

int main(int argc, char** argv)
{
    unsigned int is_graphic = DEFAULT_GRAPHIC_MODE_FLAG;
    unsigned int clients_count = DEFAULT_CLIENT_COUNT;
    parse_opts(argc, argv, &is_graphic, &clients_count);


    //=== Register clients

    struct client_container *clients = init_client_container(clients_count);
    register_clients(argc, (const char **) argv, clients, clients_count);

    //=== Check clients initialization

    for (size_t i = 0; i < clients->current_size; i++) {
        assert(clients->clients_array[i]->initialize != NULL);
        assert(clients->clients_array[i]->play != NULL);
        assert(clients->clients_array[i]->get_player_name != NULL);
        assert(clients->clients_array[i]->finalize != NULL);
    }

    //=== Display clients name

    if (clients->current_size > 0)
        printf("Display all players:\n");
    for (size_t i = 0; i < clients->current_size; i++) {
        printf("\t- PLAYER#%d: %s\n", (int) (i+1), clients->clients_array[i]->get_player_name());
    }

    //=== Start the game

    game_main(clients);

    //=== Free used resources & memory

    free_resources(clients);
    free_client_container(clients);

    return EXIT_SUCCESS;
}
