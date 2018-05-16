#ifndef APP_H
#define APP_H

#include <SDL.h>
#include <SDL_ttf.h>

////////////////////////////////////////////////////////////////////
///     DEFINITIONS
////////////////////////////////////////////////////////////////////

#define SCREEN_WIDTH 950
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32
#define FRAMERATE 30
#define COLOR_COAL_MINE 51, 51, 51, 255
#define UI_PREF "\x1B[33m[UI] "


////////////////////////////////////////////////////////////////////
///     STRUCTURES
////////////////////////////////////////////////////////////////////

/**
 * The AppState enum defines the different
 * states that the app can take while running.
 */
enum app_state
{
    MENU,
    GAME,
    PAUSE,
    END,
    QUIT
};


/**
 * The App structure contains app running needed services
 */
struct app
{
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    char* title;
    enum app_state state;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event *event;
    TTF_Font *font;
};

////////////////////////////////////////////////////////////////////
///     FUNCTIONS DECLARATION
////////////////////////////////////////////////////////////////////

/**
 * @brief Inits the application
 * @return the newly allocated app pointer
 */
struct app* app__init();


/**
 * @brief Run the app
 * @param app
 * @param argc
 * @param argv
 * @param clients_count
 */
void app__run(struct app* app, int argc, char **argv, unsigned int clients_count);


/**
 * @brief Frees memory used by the app
 * @param app the application
 */
void app_free(struct app* app);

#endif
