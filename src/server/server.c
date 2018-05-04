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

    m->check = FAILED; //By default
    printf("\tPlayer %d has sent the following move :\n\t", p->id);
    move_debug_op(m);

    //=== Card checking

    struct card *card = card__init(m->card);
    card->pos = m->onto;
    card->orientation = (enum orientation) m->dir;
    int was_card_added = board__add_card(b, card) == SUCCESS;

    //=== Meeple checking

    struct meeple *meeple = meeple__init(m->player, card, m->place);
    int was_meeple_added = meeple == NULL ? NOT_APPLICABLE : board__add_meeple(b, meeple) == SUCCESS;

    //=== Checking sum

    if (was_card_added && was_meeple_added != false) {
        printf("\tThe move is valid.\n");
        m->check = VALID;
    } else {
        printf("\tThe move is invalid.\n");
    }

    card__free(card);
    meeple__free(meeple);
    return m->check == VALID ? true : false;
}

enum card_id draw_until_valid(struct board* b, struct stack *s)
{
    enum card_id ci;
    do {
        ci = card__draw(s);
        printf(SRV_PREF"Drawing a new card (card: %d)..."CLR"\n", ci);
    } while (!board__is_valid_card(b, ci));

    return ci;
}

struct move *build_previous_moves_array(struct queue *moves, unsigned int nb_moves)
{
    assert_not_null(moves, __func__, "moves parameter");

    struct move *moves_array = safe_malloc(sizeof(struct move) * nb_moves);

    for (unsigned int i = 0; i < nb_moves; i++) {
        struct move m = { FAILED, 99, LAST_CARD, { -1, -1}, NORTH, NO_MEEPLE };
        moves_array[i] = m;
    }

    unsigned int cpt = 0;

    while (cpt < nb_moves && !queue__is_empty(moves)) {
        struct move *m = queue__front(moves);
        queue__dequeue(moves);
        if (m->check == VALID) {
            moves_array[cpt] = *m;
            cpt++;
        } else {
            if (queue__is_empty(moves)) { //to avoid infinite loop
                queue__enqueue(moves, m);
                free(m);
                break;
            }
        }
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
    //=== Board initialization

    struct board* board = board__init();
    init_deck(board->drawing_stack);
    board__init_first_card(board);

    if (board->first_card == NULL)
        exit_on_error("Missing first card");

    //=== Player initialization

    for (unsigned int i = 0; i < nb_player; i++)
        init_next_player(players, nb_player);

    //=== Game loop

    while (!stack__is_empty(board->drawing_stack) && nb_player > 1) {
        struct move *moves_array = build_previous_moves_array(board->moves_queue, nb_player);
        enum card_id c = draw_until_valid(board, board->drawing_stack);
        struct player *p = queue__front(players);
        struct move m = p->play(c, moves_array, nb_player);

        queue__dequeue(players);
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

    //TODO after May 4th: Final score counting

    //=== Players finalization

    for (size_t i = 0; i < nb_player; i++)
        finalize_next_player(players);

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

/* Acknowledgement :
 * FIXME : Remaining issues breaking "May the 4th..." goal :
 * - Player may be NULL sometimes
 *      -> Issue is identified ? YES
 *      -> Concerned issues areas: realloc in queue to decrease size after a dequeue (or realloc in enqueue ?)
 *      -> Temporary workaround : Disabling faulty code
 *
 * - Board update is not always done for client causing them placing invalid already placed moves
 *      -> Issue is identified ? NO
 *      -> Concerned issues areas: client__update_board() or add_card_to_board() ?
 *      -> Temporary workaround : NO
 *
 * - Moves are sometimes invalid while they shouldn't :
 *      -> Issue is identified ? NO
 *      -> Concerned issues areas:  client__play_card() or add_card_to_board() ?
 *      -> Temporary workaround : NO
 *
 * - Invalid given card to the player : or client considering it invalid
 *      -> Issue is identified ? NO
 *      -> Concerned issues areas:  client__play_card() or board__is_valid_card() ?
 *      -> Temporary workaround : NO
 */
