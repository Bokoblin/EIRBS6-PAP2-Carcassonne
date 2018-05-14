#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#include <SDL_surface.h>
#include <SDL_events.h>
#include <SDL_timer.h>
#include <SDL_image.h>
#include "app.h"
#include "image.h"
#include "text.h"

////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////

struct menu
{
    //=== model
    unsigned int client_count;

    //=== view
    struct app* app;
    struct image *title_image;
    struct image *play_image;
    struct image *quit_image;
    struct text *title_text;
    struct text *client_count_text;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS DECLARATION
////////////////////////////////////////////////////////////////////

/**
 * @brief Init the menu
 * @param app the application
 * @param client_count the number of client ready to play
 * @return a newly allocated pointer
 */
struct menu *menu__init(struct app *app, unsigned int client_count);


/**
 * @brief Handle the events occurring in the menu
 * @param event the event handling structure
 * @param menu the menu
 * @return 0 on app state change, 1 otherwise
 */
int menu__handle_events(SDL_Event *event, struct menu *menu);


/**
 * @brief Handle menu non UI related components
 * @param menu the menu
 */
void menu__next_step(struct menu* menu);


/**
 * @brief Update the UI components
 * @param menu the menu
 */
void menu__update(struct menu* menu);


/**
 * @brief Render the UI components
 * @param menu the menu
 */
void menu__render(struct menu* menu);


/**
 * @brief Free memory used by the menu
 * @param menu the menu
 */
void menu__free(struct menu* menu);

#endif
