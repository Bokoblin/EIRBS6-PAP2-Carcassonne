#include <SDL_image.h>
#include "../server/game.h"
#include "../common/utils.h"
#include "../server/board.h"
#include "../common/deck.h"
#include "game_view.h"

////////////////////////////////////////////////////////////////////
///     FILENAME DEFINITIONS / STRINGS
////////////////////////////////////////////////////////////////////

const char* END_TEXT = "Final results";
const char* DRAWING_STACK_TEXT = "Card drawing stack";
const char* TABLE_BACKGROUND_IMAGE = "res/game/table.png";
const char* PAUSE_IMAGE = "res/game/pause.png";
const char* RESUME_IMAGE = "res/game/resume.png";


////////////////////////////////////////////////////////////////////
///     FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct game_view *game_view__init(struct app *app, struct game *game)
{
    assert_not_null(app, __func__, "app parameter");

    struct game_view *this = safe_malloc(sizeof(struct game_view));

    this->scale_amount = 0;
    this->game = game;
    this->app = app;

    //=== ADTs init

    this->card_view_stack = stack__empty(card_view__copy_op, card_view__delete_op, card_view__debug_op);
    this->card_view_set = set__empty(card_view__copy_op, card_view__delete_op, card_view__compare_op,
                                     card_view__debug_op);
    this->results_text_set = set__empty(text__copy_op, text__delete_op, text__compare_op, text__debug_op);

    //=== Text loading

    this->drawing_stack_text = text__init((int) (SHELF_TEXT_X * app->width), (int) (SHELF_TEXT_Y * app->height), -1, -1,
                                          DRAWING_STACK_TEXT, this->app->renderer, this->app->font);

    this->end_title_text = text__init(app->width / 2, (int) (0.05 * app->height), -1, -1,
                                      END_TEXT, this->app->renderer, this->app->font);
    text__set_origin(this->end_title_text, X_CENTERED);

    //=== Images loading

    this->table_background_image = image__init(0, 0, app->width, app->height, TABLE_BACKGROUND_IMAGE, app->renderer);

    this->pause_image = image__init((int) (0.95 * app->width), (int) (0.025 * app->height), PAUSE_SIZE, PAUSE_SIZE,
                                    PAUSE_IMAGE, app->renderer);

    for (int i = 0; i < CARDS_NUMBER-1; i++) {
        int cv_w = DEFAULT_CARD_SIZE;
        int cv_x = (int) (SHELF_CARD_X * app->width + i * ((0.90 * app->width) / CARDS_NUMBER));
        int cv_y = (int) (SHELF_CARD_Y * app->height);
        struct card_view * cv = card_view__init(cv_x, cv_y, cv_w, cv_w, this->app->renderer, false);
        stack__push(this->card_view_stack, cv);
        card_view__free(cv);
    }

    //=== First card's view init

    struct card_view * cv_first = card_view__init(app->width / 2, (int) ((SHELF_TEXT_Y * app->height) / 2),
                                                  DEFAULT_CARD_SIZE, DEFAULT_CARD_SIZE, this->app->renderer, true);
    card_view__set_model_card(cv_first, this->game->board->first_card);
    set__add(this->card_view_set, cv_first);
    card_view__free(cv_first);

    //=== Renderer init

    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    SDL_ShowCursor(SDL_ENABLE);

    return this;
}

int game_view__handle_events(SDL_Event *event, struct game_view *game_view)
{
    while(SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                printf(UI_PREF"Quitting..."CLR"\n");
                game_view->app->state = QUIT;
                return false;
            case SDL_KEYUP :
                if (event->key.keysym.sym == SDLK_ESCAPE) {
                    printf(UI_PREF"Returning to menu..."CLR"\n");
                    game_view->app->state = MENU;
                    return false;
                }
                break;
            case SDL_MOUSEBUTTONUP :
                if (game_view->app->state != END) {
                    if (image__contains(game_view->pause_image, event->button.x, event->button.y)) {
                        if (game_view->app->state == GAME) {
                            printf(UI_PREF"Pausing game..."CLR"\n");
                            game_view->app->state = PAUSE;
                            image__set_texture(game_view->pause_image, (char *) RESUME_IMAGE);
                        } else {
                            printf(UI_PREF"Resuming game..."CLR"\n");
                            game_view->app->state = GAME;
                            image__set_texture(game_view->pause_image, (char *) PAUSE_IMAGE);
                        }

                        return true;
                    }
                }
                break;
            default:
                break;
        }
    }

    if (game_view->app->state != END) {
        if (stack__is_empty(game_view->game->board->drawing_stack) || game_view->game->nb_players < 2) {
            printf(UI_PREF"Moving to the end screen..."CLR"\n");
            game_view->app->state = END;
            return false;
        }
    }

    return true;
}

struct card_view *get_card_view_at_dir(struct set * s, enum direction d)
{
    struct card_view *found = set__get_umpteenth_no_copy(s, 0);

    switch (d)
    {
        case WEST:
            found = set__get_umpteenth_no_copy(s, 0);
            break;
        case EAST:
            found = set__get_umpteenth_no_copy(s, set__size(s) - 1);
            break;
        case NORTH:
            for (size_t i = 0; i < set__size(s); i++) {
                struct card_view *umpteenth = set__get_umpteenth_no_copy(s, i);
                if (umpteenth->card_model->pos.y > found->card_model->pos.y)
                    found = umpteenth;
            }
            break;
        case SOUTH:
            for (size_t i = 0; i < set__size(s); i++) {
                struct card_view *umpteenth = set__get_umpteenth_no_copy(s, i);
                if (umpteenth->card_model->pos.y < found->card_model->pos.y)
                    found = umpteenth;
            }
            break;
    }

    return found;
}

void game_view__update_board_size(struct game_view *game_view)
{
    const int MARGIN = 5;
    double board_width = game_view->app->width;
    double board_height = 0.75 * game_view->app->height;

    while (get_card_view_at_dir(game_view->card_view_set, NORTH)->image->text_rect.y <= MARGIN
            || (get_card_view_at_dir(game_view->card_view_set, SOUTH)->image->text_rect.y
                + get_card_view_at_dir(game_view->card_view_set, SOUTH)->image->text_rect.h) >= board_height - MARGIN
            || get_card_view_at_dir(game_view->card_view_set, WEST)->image->text_rect.x <= MARGIN
            || (get_card_view_at_dir(game_view->card_view_set, EAST)->image->text_rect.x
                + get_card_view_at_dir(game_view->card_view_set, EAST)->image->text_rect.w) >= board_width - MARGIN) {
        game_view->scale_amount++;
        for (size_t i = 0; i < set__size(game_view->card_view_set); i++) {
            struct card_view *cv = set__get_umpteenth_no_copy(game_view->card_view_set, i);
            card_view__set_viewable_position(cv, board_width / 2, board_height / 2, game_view->scale_amount);
        }
    }
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

        is_valid = game_view__handle_card_drawing_view_update(game_view, ci);
    } while (!is_valid);

    return ci;
}

int game_view__handle_card_drawing_view_update(struct game_view *game_view, enum card_id ci)
{
    int is_valid;

    struct card_view *popped_view = stack__peek(game_view->card_view_stack);
    card_view__set_front(popped_view, ci);
    game_view__update(game_view);
    game_view__render(game_view);
    SDL_Delay(800); //to keep in order to see the card on stack before it is moved

    if (board__is_valid_card(game_view->game->board, ci)) {
        is_valid = true;
    } else {
        struct card_view *popped = stack__pop(game_view->card_view_stack);
        card_view__free(popped);
        is_valid = false;
    }

    return is_valid;
}

void game_view__handle_valid_play_view_update(struct game_view *game_view, struct move m)
{
    //Getting model card
    struct card *card = board__retrieve_card_by_position(game_view->game->board->cards_set, m.onto);

    //Getting view card
    struct card_view *popped = stack__pop(game_view->card_view_stack);

    //Updating the view card
    card_view__set_model_card(popped, card);

    //Adding the view card to the set and freeing base pointer
    set__add(game_view->card_view_set, popped);
    card_view__free(popped);
}

void game_view__loop(struct game_view *game_view)
{
    if (game_view->app->state == GAME) {
        struct move *moves_array = build_previous_moves_array(game_view->game->board->moves_queue);
        enum card_id c = game_view__draw_until_valid(game_view);
        struct player *p = queue__front(game_view->game->players_queue);
        struct move m = p->play(c, moves_array, queue__length(game_view->game->board->moves_queue));

        queue__dequeue(game_view->game->players_queue);
        if (queue__length(game_view->game->board->moves_queue) == game_view->game->nb_players)
            queue__dequeue(game_view->game->board->moves_queue);

        if (is_valid_play(game_view->game->board, p, &m)) {
            game_view__handle_valid_play_view_update(game_view, m);
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
    }
}

void game_view__init_end(struct game_view *game_view)
{
    for (unsigned int i = 0; i < game_view->game->nb_players; i++) {
        struct player *p = queue__front(game_view->game->players_queue);
        queue__dequeue(game_view->game->players_queue);

        char message[30];
        sprintf(message, "Player %d     %d", p->id, p->score);
        struct text *t_player = text__init((int) (0.10 * game_view->app->width),
                                           (int) (0.20 * game_view->app->height + (50 * i)),
                                           -1, -1, message, game_view->app->renderer, game_view->app->font);

        set__add(game_view->results_text_set, t_player);

        queue__enqueue(game_view->game->players_queue, p);
        player__free(p);
    }
}

void game_view__update(struct game_view *game_view)
{
    if (game_view->app->state == GAME || game_view->app->state == PAUSE) {
        stack__apply_to_all(game_view->card_view_stack, (applying_func_t) card_view__update);
        set__apply_to_all(game_view->card_view_set, (applying_func_t) card_view__update);
        text__update(game_view->drawing_stack_text);

        double board_width = game_view->app->width;
        double board_height = 0.75 * game_view->app->height;
        for (size_t i = 0; i < set__size(game_view->card_view_set); i++) {
            struct card_view *cv = set__get_umpteenth_no_copy(game_view->card_view_set, i);
            card_view__set_viewable_position(cv, board_width / 2, board_height / 2, game_view->scale_amount);
        }
        game_view__update_board_size(game_view);
    } else {
        text__update(game_view->end_title_text);
        set__apply_to_all(game_view->results_text_set, (applying_func_t) text__update);
    }
}

void game_view__render(struct game_view *game_view)
{
    SDL_RenderClear(game_view->app->renderer);

    if (game_view->app->state == GAME || game_view->app->state == PAUSE) {
        //Render images
        image__render(game_view->table_background_image);
        image__render(game_view->pause_image);
        stack__apply_to_all(game_view->card_view_stack, (applying_func_t) card_view__render);
        set__apply_to_all(game_view->card_view_set, (applying_func_t) card_view__render);

        //Render text
        text__render(game_view->drawing_stack_text);
    } else {
        //Render images
        image__render(game_view->table_background_image);

        //Render text
        text__render(game_view->end_title_text);
        set__apply_to_all(game_view->results_text_set, (applying_func_t) text__render);
    }

    SDL_RenderPresent(game_view->app->renderer);
}

void game_view__free(struct game_view *game_view)
{
    image__free(game_view->table_background_image);
    image__free(game_view->pause_image);
    text__free(game_view->drawing_stack_text);
    text__free(game_view->end_title_text);
    set__free(game_view->card_view_set);
    set__free(game_view->results_text_set);
    stack__free(game_view->card_view_stack);
    free(game_view);
}
