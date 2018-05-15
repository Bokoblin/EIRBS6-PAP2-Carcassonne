#include "menu_view.h"
#include "../common/utils.h"

////////////////////////////////////////////////////////////////////
///     FILENAME DEFINITIONS / STRINGS
////////////////////////////////////////////////////////////////////

const char* TITLE_TEXT = "Project Carcassonne";
const char *CLIENT_TEXT = " clients are ready to play";

const char* TITLE_IMAGE = "res/menu/title.png";
const char* PLAY_BUTTON_IMAGE = "res/menu/button_play.png";
const char* QUIT_BUTTON_IMAGE = "res/menu/button_quit.png";


////////////////////////////////////////////////////////////////////
///     FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct menu *menu__init(struct app *app, unsigned int client_count)
{
    assert_not_null(app, __func__, "app parameter");
    struct menu *this = safe_malloc(sizeof(struct menu));

    this->app = app;
    this->client_count = client_count;

    //=== Sprites loading

    this->title_image = image__init((int) (this->app->width / 2 - 300), (int) (0.1 * this->app->height),
                                    -1, -1, TITLE_IMAGE, app->renderer);
    this->play_image = image__init((int) (0.3 * this->app->width - 90), (int) (0.75 * this->app->height),
                                   -1, -1, PLAY_BUTTON_IMAGE, app->renderer);
    this->quit_image = image__init((int) (0.7 * this->app->width) - 90, (int) (0.75 * this->app->height),
                                   -1, -1, QUIT_BUTTON_IMAGE, app->renderer);

    //=== Text loading

    this->title_text = text__init(app->width / 2, (int) (0.03 * app->height), -1, -1, TITLE_TEXT,
                                              app->renderer, app->font);
    text__set_origin(this->title_text, X_CENTERED);

    char *content = malloc(sizeof(char) * strlen(CLIENT_TEXT) + 2);
    sprintf(content, client_count < 10 ? " %d%s" : "%d%s", client_count, CLIENT_TEXT);

    this->client_count_text = text__init(app->width / 2, (int) (0.90 * app->height), -1, -1, content,
                                          app->renderer, app->font);
    text__set_origin(this->client_count_text, X_CENTERED);
    free(content);

    //=== Renderer init

    SDL_SetRenderDrawColor(app->renderer, COLOR_COAL_MINE);
    SDL_RenderClear(app->renderer);

    SDL_ShowCursor(SDL_ENABLE);

    return this;
}

int menu__handle_events(SDL_Event *event, struct menu *menu)
{
    while(SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                printf(UI_PREF"Quitting..."CLR"\n");
                menu->app->state = QUIT;
                return false;
                case SDL_MOUSEBUTTONUP :
                    if (image__contains(menu->play_image, event->button.x, event->button.y)) {
                        if (menu->client_count != 0) {
                            printf(UI_PREF"Launching game..."CLR"\n");
                            menu->app->state = GAME;
                        } else {
                            printf(UI_PREF"No client available to play!"CLR"\n");
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Warning",
                                                     "Can't launch the game. \nThere are no clients available.", NULL);
                        }
                        return false;
                    }
                    if (image__contains(menu->quit_image, event->button.x, event->button.y)) {
                        printf(UI_PREF"Quitting..."CLR"\n");
                        menu->app->state = QUIT;
                        return false;
                    }
                    break;
            default:
                break;
        }
    }

    return true;
}

void menu__next_step(struct menu *menu)
{
    (void) menu;

    //Improvement
    //Dynamically check number of players instead of relying only on console args
    //HOW:  - [-d] option
    //      - non-blocking 3-seconds interval check
}

void menu__update(struct menu *menu)
{
    text__update(menu->title_text);
    text__update(menu->client_count_text);
}

void menu__render(struct menu *menu)
{
    SDL_RenderClear(menu->app->renderer);

    image__render(menu->title_image);
    image__render(menu->play_image);
    image__render(menu->quit_image);
    text__render(menu->title_text);
    text__render(menu->client_count_text);

    SDL_RenderPresent(menu->app->renderer);
}

void menu__free(struct menu* menu)
{
    image__free(menu->title_image);
    image__free(menu->play_image);
    image__free(menu->quit_image);
    text__free(menu->title_text);
    text__free(menu->client_count_text);
    free(menu);
}
