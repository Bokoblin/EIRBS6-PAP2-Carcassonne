#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <SDL_surface.h>
#include <SDL_events.h>
#include <SDL_system.h>
#include "app.h"
#include "card_view.h"
#include "image.h"
#include "text.h"
#include "../common/interface.h"

#define SHELF_TEXT_X 0.05
#define SHELF_TEXT_Y 0.8
#define SHELF_CARD_X 0.02
#define SHELF_CARD_Y 0.88
#define DEFAULT_CARD_SIZE 50
#define PAUSE_SIZE 36

////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////

struct game_view
{
    //=== links (not to free)
    struct game *game;
    struct app* app;

    //=== components
    struct image *table_background_image;
    struct image *pause_image;
    struct text *drawing_stack_text;
    struct text *end_title_text;
    struct set *card_view_set;
    struct set *results_text_set;
    struct stack *card_view_stack;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS DECLARATION
////////////////////////////////////////////////////////////////////

/**
 * @brief Init the game view
 * @param app the application
 * @param game the game model
 * @return a newly allocated pointer
 */
struct game_view* game_view__init(struct app *app, struct game *game);


/**
 * @brief Handle the events occurring in the game
 * @param event the event handling structure
 * @param game_view the game view
 * @return 0 on app state change, 1 otherwise
 */
int game_view__handle_events(SDL_Event *event, struct game_view *game_view);


/**
 * @brief A revised version of the model's until valid card drawing to support view update
 * @param game_view the game
 * @return the card id drawn
 */
enum card_id game_view__draw_until_valid(struct game_view *game_view);


/**
 * @brief Handle the view update when a card is drawn
 * @param game_view the game view
 * @param ci the card id drawn
 * @return the validity of the drawn card (1 if valid, 0 otherwise)
 */
int game_view__handle_card_drawing_view_update(struct game_view *game_view, enum card_id ci);


/**
 * @brief Handle the view update when a valid card is added to the board
 * @param game_view the game view
 * @param m the move
 */
void game_view__handle_valid_play_view_update(struct game_view *game_view, struct move m);


/**
 * @brief Handle game non UI related components
 * @param game_view the game
 */
void game_view__loop(struct game_view *game_view);


/**
 * @brief Handle the end screen init
 * @param game_view
 */
void game_view__init_end(struct game_view *game_view);


/**
 * @brief Update the UI components
 * @param game_view the game view
 */
void game_view__update(struct game_view *game_view);


/**
 * @brief Render the UI components
 * @param game_view the game view
 */
void game_view__render(struct game_view *game_view);


/**
 * @brief Free memory used by the game view
 * @param game_view the game view
 */
void game_view__free(struct game_view *game_view);

#endif
