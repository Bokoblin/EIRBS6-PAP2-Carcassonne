#include "game.h"
#include <dlfcn.h>
#include <string.h>
#include <time.h>

struct game *game__init(int argc, char **argv, unsigned int nb_players)
{
    struct game *this = safe_malloc(sizeof(struct game));

    //=== Player registration

    this->nb_players = nb_players;
    this->players_queue = queue__empty(player__copy_op, player__delete_op, player__debug_op);
    register_players(argc, (const char **) argv, this->players_queue, this->nb_players);

    //=== Player init

    for (unsigned int i = 0; i < this->nb_players; i++)
        init_next_player(this->players_queue, this->nb_players);

    //=== Board init

    this->board = board__init();
    board__init_deck_and_first_card(this->board);

    //=== Pseudo-random generator init

    srand((unsigned int) time(NULL));

    return this;
}

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

void init_next_player(struct queue *players_queue, unsigned int nb_player)
{
    struct player *p = queue__front(players_queue);
    queue__dequeue(players_queue);
    p->initialize(p->id, nb_player);
    queue__enqueue(players_queue, p);
    player__free(p);
}

int is_valid_play(struct board *b, struct player *p, struct move *m)
{
    printf(SRV_PREF"Validating move..."CLR"\n");
    printf("\tPlayer %d (%s) has sent the following move :\n\t", p->id, p->get_player_name());
    move__debug_op(m);

    //=== Card checking

    struct card *card = card__init(m->card);
    card->pos = m->onto;
    card->direction = m->dir;
    int was_card_added = board__add_card(b, card) == SUCCESS;

    if(!was_card_added) {
        set__debug(b->cards_set, false);
        printf("\tThe card sent by the client isn't valid.");
    }

    //=== Meeple checking

    struct meeple *meeple = meeple__init(m->player, card, m->place);
    int was_meeple_added = meeple == NULL ? NOT_APPLICABLE : board__add_meeple(b, meeple) == SUCCESS;

    if(!was_meeple_added) {
        set__debug(b->meeples_set, false);
        printf("\tThe meeple position sent by the client isn't valid.");
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
        printf(SRV_PREF"The drawing stack contains %zu cards."CLR"\n", stack__length(s));
        enum card_id *p_popped_ci = stack__pop(s);
        ci = *p_popped_ci;
        free(p_popped_ci);
        printf(SRV_PREF"Drawing a new card (%d)..."CLR"\n", ci);
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

void game__loop(struct game *g)
{
    while (!stack__is_empty(g->board->drawing_stack) && g->nb_players > 1) {
        struct move *moves_array = build_previous_moves_array(g->board->moves_queue);
        enum card_id c = draw_until_valid(g->board, g->board->drawing_stack);
        struct player *p = queue__front(g->players_queue);
        struct move m = p->play(c, moves_array, queue__length(g->board->moves_queue));

        queue__dequeue(g->players_queue);
        if (queue__length(g->board->moves_queue) == g->nb_players)
            queue__dequeue(g->board->moves_queue);

        if (is_valid_play(g->board, p, &m)) {
            queue__enqueue(g->players_queue, p);
            board__check_sub_completion(g->board);
        } else {
            printf("\tThe player named %s was expelled.\n", p->get_player_name());
            g->nb_players--;
            finalize_and_free_player(p);
        }

        queue__enqueue(g->board->moves_queue, &m);

        player__free(p);
        free(moves_array);
    }
}

void game__end(struct game *g)
{
    printf(SRV_PREF"The game has ended."CLR"\n");

    //=== Premature ending check (DEBUG)

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if (!stack__is_empty(g->board->drawing_stack))
        printf("\x1B[31mInvalid game stopping, the drawing card stack still contains %zu cards! \x1B[0m\n",
               stack__length(g->board->drawing_stack));
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    //=== Final score counting

    printf(SRV_PREF"Displaying final score..."CLR"\n");

    //TODO: [Due for May 18th] Final score counting
    printf("\tWork in progress"CLR"\n");
}

void finalize_and_free_player(struct player *p)
{
    p->finalize();
    dlclose(p->lib_ptr);
    assert_no_dlerror();
}

void game__free(struct game *g)
{
    if (g == NULL)
        return;

    queue__apply_to_all(g->players_queue, (applying_func_t) finalize_and_free_player);
    board__free(g->board);
    queue__free(g->players_queue);
    free(g);
}
