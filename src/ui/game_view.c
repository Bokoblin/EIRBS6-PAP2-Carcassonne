#include <SDL_image.h>
#include "../server/game.h"
#include "../common/utils.h"
#include "../server/board.h"
#include "../common/deck.h"
#include "card_view.h"
#include "game_view.h"
#include "text.h"

////////////////////////////////////////////////////////////////////
///     FILENAME DEFINITIONS / STRINGS
////////////////////////////////////////////////////////////////////

const char* DRAWING_STACK_TEXT = "Card drawing stack";
const char* TABLE_BACKGROUND_IMAGE = "res/game/table.png";


////////////////////////////////////////////////////////////////////
///     FUNCTIONS FROM MODEL EDITED TO SUPPORT VIEW
////////////////////////////////////////////////////////////////////

int game_view__is_valid_play(struct game_view *game_view, struct board *b, struct player *p, struct move *m)
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

        ///================ GAME VIEW EDIT BEGIN
        //FIXME: Angle render is broken
        struct card_view *popped = stack__pop(game_view->card_view_stack);
        card_view__set_model_card(popped, card);
        card_view__set_viewable_position(popped, game_view->app->width / 2, game_view->app->height / 2);
        set__add(game_view->card_view_set, popped);
        card_view__free(popped);
        ///================ GAME VIEW EDIT END
    } else {
        m->check = FAILED;
        printf("\tThe move is invalid.\n");
    }

    card__free(card);
    meeple__free(meeple);
    return m->check == VALID;
}

enum card_id game_view__draw_until_valid(struct game_view *game_view)
{
    int is_valid;
    enum card_id ci;
    do {
        printf(SRV_PREF"The drawing stack contains %zu cards."CLR"\n",
               stack__length(game_view->game->board->drawing_stack));
        enum card_id *p_popped_ci = stack__pop(game_view->game->board->drawing_stack);
        ci = *p_popped_ci;
        free(p_popped_ci);
        printf(SRV_PREF"Drawing a new card (%d)..."CLR"\n", ci);

        ///================ GAME VIEW EDIT BEGIN
        struct card_view *popped_view = stack__peek(game_view->card_view_stack);
        card_view__set_front(popped_view, ci);
        game_view__render(game_view);
        SDL_Delay(500); //to keep in order to see the card on stack before it is moved

        if (board__is_valid_card(game_view->game->board, ci)) {
            is_valid = true;
        } else {
            struct card_view *popped = stack__pop(game_view->card_view_stack);
            card_view__free(popped);
            is_valid = false;
        }
        ///================ GAME VIEW EDIT END
    } while (!is_valid);

    return ci;
}

////////////////////////////////////////////////////////////////////
///     FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct game_view *game_view__init(struct app *app, struct game *game)
{
    assert_not_null(app, __func__, "app parameter");

    struct game_view *this = safe_malloc(sizeof(struct game_view));

    this->game = game;
    this->app = app;

    //=== ADTs init

    this->card_view_stack = stack__empty(card_view__copy_op, card_view__delete_op, card_view__debug_op);
    this->card_view_set = set__empty(card_view__copy_op, card_view__delete_op, card_view__compare_op,
                                     card_view__debug_op);

    //=== Text loading

    this->drawing_stack_text = text__init((int) (SHELF_TEXT_X * app->width), (int) (SHELF_TEXT_Y * app->height), -1, -1,
                                          DRAWING_STACK_TEXT, this->app->renderer, this->app->font);

    //=== Images loading

    this->table_background_image = image__init(0, 0, app->width, app->height, TABLE_BACKGROUND_IMAGE, app->renderer);

    for (int i = 0; i < CARDS_NUMBER-1; i++) {
        int cv_w = DEFAULT_CARD_SIZE;
        int cv_x = (int) (SHELF_CARD_X * app->width + i * ((0.90 * app->width) / CARDS_NUMBER));
        int cv_y = (int) (SHELF_CARD_Y * app->height);
        struct card_view * cv = card_view__init(cv_x, cv_y, cv_w, cv_w, this->app->renderer, false);
        stack__push(this->card_view_stack, cv);
        card_view__free(cv);
    }

    //=== First card's view init

    struct card_view * cv_first = card_view__init(app->width / 2, app->height / 2, DEFAULT_CARD_SIZE, DEFAULT_CARD_SIZE,
                                                  this->app->renderer, true);
    card_view__set_model_card(cv_first, this->game->board->first_card);
    card_view__set_viewable_position(cv_first, app->width / 2, app->height / 2);
    set__add(this->card_view_set, cv_first);
    card_view__free(cv_first);

    //=== Renderer init

    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    SDL_ShowCursor(SDL_DISABLE);

    return this;
}

int game_view__handle_events(SDL_Event *event, struct game_view *game_view)
{
    while(SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                printf("Quitting...\n");
                game_view->app->state = QUIT;
                return false;
            case SDL_KEYUP :
                if (event->key.keysym.sym == SDLK_ESCAPE) {
                    printf("Returning to menu...\n");
                    game_view->app->state = MENU;
                    return false;
                }
                break;
            default:
                break;
        }
    }

    if (stack__is_empty(game_view->game->board->drawing_stack) || game_view->game->nb_players < 2)
        return false;

    return true;
}

void game_view__loop(struct game_view *game_view)
{
    struct move *moves_array = build_previous_moves_array(game_view->game->board->moves_queue);
    enum card_id c = game_view__draw_until_valid(game_view);
    struct player *p = queue__front(game_view->game->players_queue);
    struct move m = p->play(c, moves_array, queue__length(game_view->game->board->moves_queue));

    queue__dequeue(game_view->game->players_queue);
    if (queue__length(game_view->game->board->moves_queue) == game_view->game->nb_players)
        queue__dequeue(game_view->game->board->moves_queue);

    if (game_view__is_valid_play(game_view, game_view->game->board, p, &m)) {
        queue__enqueue(game_view->game->players_queue, p);
        board__check_sub_completion(game_view->game->board);
    } else {
        printf("\tThe player named %s was expelled.\n", p->get_player_name());
        game_view->game->nb_players--;
        finalize_and_free_player(p);
    }

    queue__enqueue(game_view->game->board->moves_queue, &m);

    player__free(p);
    free(moves_array);

    //FIXME: temporary game slowdown method
    SDL_Delay(500);
}

void game_view__update(struct game_view *game_view)
{
    text__update(game_view->drawing_stack_text);
}

void game_view__render(struct game_view *game_view)
{
    SDL_RenderClear(game_view->app->renderer);

    //=== Render images

    image__render(game_view->table_background_image);
    stack__apply_to_all(game_view->card_view_stack, (applying_func_t) card_view__render);
    set__apply_to_all(game_view->card_view_set, (applying_func_t) card_view__render);

    //=== Render text

    text__render(game_view->drawing_stack_text, game_view->app->renderer);

    SDL_RenderPresent(game_view->app->renderer);
}

void game_view__free(struct game_view *game_view)
{
    image__free(game_view->table_background_image);
    text__free(game_view->drawing_stack_text);
    set__free(game_view->card_view_set);
    stack__free(game_view->card_view_stack);
    free(game_view);
}
