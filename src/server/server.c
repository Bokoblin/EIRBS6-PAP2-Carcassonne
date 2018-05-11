#include "server.h"
#include <dlfcn.h>
#include <string.h>
#include <time.h>

void register_players(int argc, const char **argv, struct queue *players, unsigned int nb_players)
{
    unsigned int nb_players_registered = 0;
    int args_index = 0;

    while (nb_players_registered < nb_players && args_index < argc) {
        const char* current_arg = argv[args_index];
        if (strstr(current_arg, ".so") != NULL) {
            printf(SRV_PREF"Registering: %s..."CLR"\n", current_arg);

            void* player_lib_ptr = dlopen(current_arg, RTLD_NOW);
            assert_no_dlerror();
            struct player* p = player__init(nb_players_registered, player_lib_ptr);

            p->get_player_name = dlsym(player_lib_ptr, "get_player_name"); assert_no_dlerror();
            p->initialize = dlsym(player_lib_ptr, "initialize"); assert_no_dlerror();
            p->play = dlsym(player_lib_ptr, "play"); assert_no_dlerror();
            p->finalize = dlsym(player_lib_ptr, "finalize"); assert_no_dlerror();

            queue__enqueue(players, p);
            nb_players_registered++;
            printf("\tPLAYER#%d: %s was registered.\n", p->id, p->get_player_name());
            player__free(p);
        }
        args_index++;
    }
}

int is_valid_play(struct board *b, struct player *p, struct move *m)
{
    printf(SRV_PREF"Validating move..."CLR"\n");
    printf("\tPlayer %d has sent the following move :\n\t", p->id);
    move_debug_op(m);

    //=== Card checking

    struct card *card = card__init(m->card);
    card->pos = m->onto;
    card->direction = m->dir;
    int was_card_added = board__add_card(b, card) == SUCCESS;

    if(!was_card_added) {
        set__debug_data(b->cards_set, false);
        printf("\t[SERVER] The card sent by the client isn't valid.");
        //exit(EXIT_FAILURE);
    }

    //=== Meeple checking

    struct meeple *meeple = meeple__init(m->player, card, m->place);
    int was_meeple_added = meeple == NULL ? NOT_APPLICABLE : board__add_meeple(b, card, meeple) == SUCCESS;

    if(!was_meeple_added) {
        set__debug_data(b->meeples_set, false);
        printf("\t[SERVER] The meeple position sent by the client isn't valid.");
        //exit(EXIT_FAILURE);
    }

    //=== Checking sum

    if (was_card_added && was_meeple_added) {
        printf("\tThe move is valid.\n");
        m->check = VALID;
    } else {
        m->check = FAILED;
        printf("\tThe move is invalid.\n");
    }

    card__free(card);
    meeple__free(meeple);
    return m->check == VALID;
}

enum card_id draw_until_valid(struct board* b, struct stack *s)
{
    enum card_id ci;
    do {
        enum card_id *p_popped_ci = stack__pop(s);
        ci = *p_popped_ci;
        free(p_popped_ci);
        printf(SRV_PREF"Drawing a new card (card: %d)..."CLR"\n", ci);
    } while (!board__is_valid_card(b, ci));

    return ci;
}

struct move *build_previous_moves_array(struct queue *moves)
{
    assert_not_null(moves, __func__, "moves parameter");

    size_t n_moves = queue__length(moves);
    struct move *moves_array = safe_malloc(sizeof(struct move) * n_moves);

    size_t cpt = 0;

    while (cpt < n_moves && !queue__is_empty(moves)) {
        struct move *m = queue__front(moves);
        queue__dequeue(moves);
        moves_array[cpt] = *m;
        cpt++;
        queue__enqueue(moves, m);
        free(m);
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
    if (nb_player == 0)
        return;

    //=== Board initialization

    struct board* board = board__init();
    init_deck(board->drawing_stack);
    board__init_first_card(board);

    //=== Player initialization

    for (unsigned int i = 0; i < nb_player; i++)
        init_next_player(players, nb_player);

    //=== Game loop

    while (!stack__is_empty(board->drawing_stack) && nb_player > 1) {
        struct move *moves_array = build_previous_moves_array(board->moves_queue);
        enum card_id c = draw_until_valid(board, board->drawing_stack);
        struct player *p = queue__front(players);
        struct move m = p->play(c, moves_array, queue__length(board->moves_queue));

        queue__dequeue(players);
        if (queue__length(board->moves_queue) == nb_player)
            queue__dequeue(board->moves_queue);
        
        if (is_valid_play(board, p, &m)) {
            queue__enqueue(players, p);
            board__check_sub_completion(board);
        } else {
            printf("\tThe player named %s was expelled.\n", p->get_player_name());
            nb_player--;
            p->finalize();
            free_player_resources(p);
        }

        queue__enqueue(board->moves_queue, &m);

        player__free(p);
        free(moves_array);
    }

    //=== Final score counting

    //TODO: Final score counting

    //=== Players finalization

    for (size_t i = 0; i < nb_player; i++)
        finalize_next_player(players);

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if (!stack__is_empty(board->drawing_stack))
        printf("\x1B[31mInvalid game stopping, the drawing card stack still contains %zu cards! \x1B[0m\n",
                   stack__length(board->drawing_stack));
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    //=== Board memory release

    board__free(board);
}

void free_player_resources(struct player *p)
{
    dlclose(p->lib_ptr);
    assert_no_dlerror();
}

void free_resources(struct queue *players_queue)
{
    if (players_queue == NULL)
        return;

    while(!queue__is_empty(players_queue)) {
        struct player *p = queue__front(players_queue);
        queue__dequeue(players_queue);
        free_player_resources(p);
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

    struct queue *players_queue = queue__empty(player_copy_op, player_delete_op, player_debug_op);
    register_players(argc, (const char **) argv, players_queue, clients_count);

    //=== Start the game

    game_main(players_queue, clients_count);

    //=== Free used resources & memory

    free_resources(players_queue);

    return EXIT_SUCCESS;
}
