#ifndef TEXT_H
#define TEXT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_surface.h>
#include <SDL_system.h>


////////////////////////////////////////////////////////////////////
///     ENUMERATION FOR TEXT ORIGIN
////////////////////////////////////////////////////////////////////

enum text_origin
{
    DEFAULT,
    X_CENTERED,
    Y_CENTERED,
    FULL_CENTERED
};

////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////

struct text
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_Color color;
    SDL_Rect texture_rect_origin;
    SDL_Rect texture_rect;
    TTF_Font *font;
    char *content;
    enum text_origin text_origin;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS DECLARATION
////////////////////////////////////////////////////////////////////

/**
 * @brief Inits a text
 * @param x the x-axis coordinate
 * @param y the y-axis coordinate
 * @param w the width
 * @param h the height
 * @param content the text content
 * @param renderer the app's renderer
 * @param font the text's font
 * @return a newly allocated pointer of an text
 */
struct text *text__init(int x, int y, int w, int h, const char *content, SDL_Renderer *renderer, TTF_Font *font);


/**
 * @brief Defines text position origin
 * @param text
 * @param origin the origine between DEFAULT and
 */
void text__set_origin(struct text* text, enum text_origin origin);


/**
 * @brief Sets the color of a text
 * @param text the text
 * @param r red pixels
 * @param g green pixels
 * @param b blue pixels
 * @param a alpha canal
 */
void text__set_color(struct text* text, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


/**
 * @brief Set the text's content
 * @param text the text struct
 * @param content the nex content
 */
void text__set_content(struct text* text, const char *content);


/**
 * @brief Checks if coordinates belong to a text
 * @param text the text
 * @param x the x-axis coordinate
 * @param y the y-axis coordinate
 * @return 1 on matching, 0 otherwise
 */
int text__contains(struct text* text, int x, int y);


/**
 * @brief Updates a text
 * @param text the text
 */
void text__update(struct text* text);


/**
 * @brief Renders a text on its renderer
 * @param text the text to render
 */
void text__render(struct text *text);


/**
 * @brief Frees memory used by the text
 * @param text the text to free
 */
void text__free(struct text* text);


////////////////////////////////////////////////////////////////////
///     OPERATORS (COPY, DELETE, COMPARE, DEBUG)
////////////////////////////////////////////////////////////////////

void* text__copy_op(struct text *t);
void text__delete_op(struct text *t);
void text__debug_op(struct text *t);
int text__compare_op(struct text *t1, struct text * t2);


#endif
