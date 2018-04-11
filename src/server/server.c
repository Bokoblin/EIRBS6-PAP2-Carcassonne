#include <dlfcn.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "../server/function_pointers.h"
#include "../common/utils.h"
#include "../common/queue.h"
#include "../common/stack.h"
#include "../common/card.h"
#include "../common/deck.h"

#define DEFAULT_GRAPHIC_MODE_FLAG 0
#define DEFAULT_CLIENT_COUNT 0

////////////////////////////////////////////////////////////////////
///     FUNCTIONS DECLARATION
////////////////////////////////////////////////////////////////////

void register_players(int argc, const char **argv, struct queue *players, unsigned int nb_players);
int is_valid_play(struct player *p, struct move *m);
struct move *build_previous_moves_array(struct queue *moves, unsigned int nb_moves);
void free_resources(struct queue *players_queue);
void game_main(struct queue *players, unsigned int nb_player);
void init_next_player(struct queue *players_queue, unsigned int nb_player);
void finalize_next_player(struct queue *players_queue);


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
void register_players(int argc, const char **argv, struct queue *players, unsigned int nb_players)
{
    unsigned int nb_players_registered = 0;
    int args_index = 0;

    while (nb_players_registered < nb_players && args_index < argc) {
        const char* current_arg = argv[args_index];
        if (strstr(current_arg, ".so") != NULL) {
            printf("Registering: %s ...", current_arg);

            void* player_lib_ptr = dlopen(current_arg, RTLD_NOW);
            assert_no_dlerror();

            void* player_get_name_ptr = safe_dlsym(player_lib_ptr, "get_player_name");
            void* player_init_ptr = safe_dlsym(player_lib_ptr, "initialize");
            void* player_play_ptr = safe_dlsym(player_lib_ptr, "play");
            void* player_finalize_ptr = safe_dlsym(player_lib_ptr, "finalize");

            struct player* p = player__init(nb_players_registered, player_lib_ptr, player_get_name_ptr,
                                            player_init_ptr, player_play_ptr, player_finalize_ptr);
            queue__enqueue(players, p);
            nb_players_registered++;
            printf("\tPLAYER#%d: %s was registered\n", p->id, p->get_player_name());
            player__free(p);
        }
        args_index++;
    }
}

int is_valid_play(struct player *p, struct move *m)
{
    //TODO : verify that player hasn't cheated
    (void) p;
    (void) m;

    return 1;
}

struct move *build_previous_moves_array(struct queue *moves, unsigned int nb_moves)
{
    if (moves == NULL)
        exit_on_error("NULL moves queue");

    struct move *moves_array = malloc(sizeof(struct move) * nb_moves);
    if (moves_array == NULL)
        exit_on_error("Malloc failure on struct move*");

    else if (!queue__is_empty(moves)) {
        for (unsigned int i = 0; i < nb_moves; i++) {
            struct move *m = queue__front(moves);
            queue__dequeue(moves);
            moves_array[i] = *m;
            queue__enqueue(moves, m);
            free(m);
        }
    }

    return moves_array;
}

void init_next_player(struct queue *players_queue, unsigned int nb_player)
{
    struct player *p = queue__front(players_queue);
    queue__dequeue(players_queue);
    p->initialize(p->id, nb_player);
    queue__enqueue(players_queue, p);
    player__free(p);
}

void finalize_next_player(struct queue *players_queue)
{
    struct player *p = queue__front(players_queue);
    queue__dequeue(players_queue);
    p->finalize();
    queue__enqueue(players_queue, p);
    player__free(p);
}

void game_main(struct queue *players, unsigned int nb_player)
{
    //=== Card stack initialization

    struct stack* drawing_stack = init_deck();

    //=== Moves queue initialization

    struct queue* moves = queue__empty(move_queue_copy_op, move_queue_delete_op, move_queue_debug_op);

    //=== Player initialization

    for (unsigned int i = 0; i < nb_player; i++)
        init_next_player(players, nb_player);

    //=== Game loop

    while (!stack__is_empty(drawing_stack) && nb_player > 0) {
        struct move *moves_array = build_previous_moves_array(moves, nb_player);
        enum card_id c = card__draw(drawing_stack); //TODO: verify card (pop again until valid)
        struct player *p = queue__front(players);
        struct move m = p->play(c, moves_array, nb_player);

        queue__dequeue(players);
        queue__dequeue(moves);

        if (is_valid_play(p, &m)) {
            queue__enqueue(players, p);
            queue__enqueue(moves, &m);
        } else {
            nb_player--;
        }

        player__free(p);
        free(moves_array);
    }

    //=== Players finalization

    for (size_t i = 0; i < nb_player; i++)
        finalize_next_player(players);

    //=== Stack && Queue memory release

    stack__free(drawing_stack);
    queue__free(moves);
}

void free_resources(struct queue *players_queue)
{
    while(!queue__is_empty(players_queue)) {
        struct player *p = queue__front(players_queue);
        queue__dequeue(players_queue);
        dlclose(p->lib_ptr);
        assert_no_dlerror();
        player__free(p);
    }

    queue__free(players_queue);
}

int main(int argc, char** argv)
{
    unsigned int is_graphic = DEFAULT_GRAPHIC_MODE_FLAG;
    unsigned int clients_count = DEFAULT_CLIENT_COUNT;
    parse_opts(argc, argv, &is_graphic, &clients_count);
    srand((unsigned int) time(NULL));

    //=== Register players

    struct queue *players_queue = queue__empty(player_queue_copy_op, player_queue_delete_op, player_queue_debug_op);
    register_players(argc, (const char **) argv, players_queue, clients_count);

    //=== Start the game

    game_main(players_queue, clients_count);

    //=== Free used resources & memory

    free_resources(players_queue);

    return EXIT_SUCCESS;
}
