#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <SDL_surface.h>
#include <SDL_events.h>
#include <SDL_system.h>
#include "app.h"
#include "image.h"
#include <time.h>

#define SHELF_TEXT_X 0.05
#define SHELF_TEXT_Y 0.8
#define SHELF_CARD_X 0.02
#define SHELF_CARD_Y 0.88
#define DEFAULT_CARD_SIZE 50

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
    struct text *drawing_stack_text;
    struct set *card_view_set;
    struct stack *card_view_stack;

    //=== loop delay
    clock_t time_start_ms;
    clock_t delay_ms;
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
 * @brief Handle game non UI related components
 * @param game_view the game
 */
void game_view__loop(struct game_view *game_view);


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
