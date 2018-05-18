#include <SDL_log.h>
#include <SDL_ttf.h>
#include "text.h"
#include "../common/utils.h"

struct text *text__init(int x, int y, int w, int h, const char *content, SDL_Renderer *renderer, TTF_Font *font)
{
    struct text* this = safe_malloc(sizeof(struct text));

    this->font = font;
    this->renderer = renderer;
    this->content = (char *) content;
    this->color.r = 255;
    this->color.g = 255;
    this->color.b = 255;
    this->color.a = 255;
    this->text_origin = DEFAULT;

    this->surface = TTF_RenderText_Blended(font, content, this->color);
    if (this->surface == NULL) {
        SDL_Log("TTF_RenderText_Solid() failed: %s", SDL_GetError());
    }

    this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
    if (this->texture == NULL) {
        SDL_Log("SDL_CreateTextureFromSurface() failed: %s", SDL_GetError());
    } else {
        SDL_QueryTexture(this->texture, NULL, NULL, &this->texture_rect.w, &this->texture_rect.h);
        this->texture_rect.x = x;
        this->texture_rect.y = y;
        this->texture_rect.w = w == -1 ? this->texture_rect.w : w;
        this->texture_rect.h = h == -1 ? this->texture_rect.h : h;
        this->texture_rect_origin = this->texture_rect;
    }

    return this;
}

void text__set_origin(struct text* text, enum text_origin origin)
{
    text->texture_rect = text->texture_rect_origin;
    text->text_origin = origin;

    switch (origin)
    {
        case DEFAULT:
            break;
        case X_CENTERED:
            text->texture_rect.x = text->texture_rect.x - (text->texture_rect.w / 2);
            text->texture_rect.y = text->texture_rect.y;
            break;
        case Y_CENTERED:
            text->texture_rect.x = text->texture_rect.x;
            text->texture_rect.y = text->texture_rect.y - text->texture_rect.h / 2;
            break;
        case FULL_CENTERED:
            text->texture_rect.x = text->texture_rect.x - text->texture_rect.w / 2;
            text->texture_rect.y = text->texture_rect.y - text->texture_rect.h / 2;
            break;
    }
}

void text__set_color(struct text* text, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    text->color.r = r;
    text->color.g = g;
    text->color.b = b;
    text->color.a = a;

    SDL_SetTextureColorMod(text->texture, r, g, b);
    SDL_SetTextureAlphaMod(text->texture, a);
}

void text__set_content(struct text* text, const char *content)
{
    SDL_FreeSurface(text->surface);

    text->surface = TTF_RenderText_Blended(text->font, content, text->color);
    if (text->surface == NULL) {
        SDL_Log("TTF_RenderText_Solid() failed: %s", SDL_GetError());
    }

    SDL_DestroyTexture(text->texture);

    text->texture = SDL_CreateTextureFromSurface(text->renderer, text->surface);
    if (text->texture == NULL) {
        SDL_Log("SDL_CreateTextureFromSurface() failed: %s", SDL_GetError());
    } else {
        SDL_QueryTexture(text->texture, NULL, NULL, &text->texture_rect.w, &text->texture_rect.h);
    }
}

int text__contains(struct text* text, int x, int y)
{
    if (text != NULL)
        return x > text->texture_rect.x
                && x < text->texture_rect.x + text->texture_rect.w
                && y > text->texture_rect.y
                && y < text->texture_rect.y + text->texture_rect.h;
    else
        return false;
}

void text__update(struct text* text)
{
    (void) text;
}

void text__render(struct text *text)
{
    if (text != NULL) {
        SDL_RenderCopy(text->renderer, text->texture, NULL, &text->texture_rect);
    }
}

void text__free(struct text* text)
{
    if (text == NULL)
        return;
    
    SDL_DestroyTexture(text->texture);
    SDL_FreeSurface(text->surface);
    free(text);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS (COPY, DELETE, COMPARE, DEBUG)
////////////////////////////////////////////////////////////////////

void* text__copy_op(struct text *t)
{
    assert_not_null(t, __func__, "t parameter");

    struct text *new_text = text__init(t->texture_rect.x, t->texture_rect.y, t->texture_rect.w, t->texture_rect.h,
                                          t->content, t->renderer, t->font);
    new_text->color = t->color;
    new_text->text_origin = t->text_origin;

    return new_text;
}

void text__delete_op(struct text *t)
{
    text__free(t);
}

void text__debug_op(struct text *t)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (t == NULL)
        printf("NULL");
    else {
        printf("Text (content: %s, position {%d, %d}, width: %d, height: %d, origin: %d )\n", t->content,
               t->texture_rect.x, t->texture_rect.y, t->texture_rect.w, t->texture_rect.h, t->text_origin);
    }
}

int text__compare_op(struct text *t1, struct text * t2)
{
    assert_not_null(t1, __func__, "t2 parameter");
    assert_not_null(t2, __func__, "t2 parameter");

    if (t1 < t2)
        return -1;
    else if (t1 == t2)
        return 0;
    else
        return 1;
}
