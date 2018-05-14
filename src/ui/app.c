#include "app.h"
#include "../common/utils.h"
#include "menu_view.h"
#include "game_view.h"
#include "../server/game.h"

////////////////////////////////////////////////////////////////////
///     FILENAME DEFINITIONS
////////////////////////////////////////////////////////////////////

const char *ROBOTO_CONDENSED_FONT = "res/font/Roboto_Condensed.ttf";


////////////////////////////////////////////////////////////////////
///     FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct app* app__init()
{
    printf(UI_PREF"Starting..."CLR"\n");

    if (SDL_Init(SDL_INIT_VIDEO) != SUCCESS) {
        SDL_Log("SDL_Init() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() != SUCCESS) {
        SDL_Log("TTF_Init() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //=== App init

    struct app* this = safe_malloc(sizeof(struct app));
    this->width = SCREEN_WIDTH;
    this->height = SCREEN_HEIGHT;
    this->bpp = SCREEN_BPP;
    this->title = "Projet Carcassonne (by Team 4410)";
    this->state = MENU;

    this->font = TTF_OpenFont(ROBOTO_CONDENSED_FONT, 25);
    if (this->font == NULL) {
        SDL_Log("TTF_OpenFont() failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //=== Window init

    this->window = SDL_CreateWindow(this->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   this->width, this->height, SDL_WINDOW_RESIZABLE);

    //=== Renderer init

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);

    //=== Event init

    this->event = safe_malloc(sizeof(SDL_Event));

    return this;
}

void app__run(struct app* app, int argc, char **argv, unsigned int clients_count)
{
    while (app->state != QUIT) {
        if (app->state == MENU) {
            struct menu *menu = menu__init(app, clients_count);

            while (app->state == MENU && menu__handle_events(app->event, menu)) {
                menu__next_step(menu);
                menu__update(menu);
                menu__render(menu);
                SDL_Delay(1000 / FRAMERATE);
            }

            menu__free(menu);
        }
        if (app->state == GAME) {
            struct game *game_model = game__init(argc, argv, clients_count);
            struct game_view *game_view = game_view__init(app, game_model);

            while (app->state == GAME && game_view__handle_events(app->event, game_view)) {
                game_view__loop(game_view);
                game_view__update(game_view);
                game_view__render(game_view);
                SDL_Delay(1000 / FRAMERATE);
            }

            game__end(game_model);
            game_view__free(game_view);
            game__free(game_model);
        }
    }
}

void app_free(struct app* app)
{
    SDL_DestroyWindow(app->window);
    SDL_DestroyRenderer(app->renderer);
    TTF_CloseFont(app->font);
    free(app->event);
    free(app);

    SDL_Quit();
    TTF_Quit();
}
