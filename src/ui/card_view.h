#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include <SDL2/SDL_render.h>
#include "../common/card.h"
#include "image.h"


////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////

struct card_view
{
    struct image *front;
    struct image *back;
    struct card *card_model;
    int is_shown;
    char* front_filename;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS DECLARATION
////////////////////////////////////////////////////////////////////

/**
 * @brief Inits an image
 * @param x the x-axis coordinate
 * @param y the y-axis coordinate
 * @param w the width
 * @param h the height
 * @param app_renderer the app's renderer
 * @param is_first specifies if it's the first card of the game
 * @return a newly allocated pointer of an image
 */
struct card_view *card_view__init(int x, int y, int w, int h, SDL_Renderer *app_renderer, int is_first);


/**
 * @brief Set the front image of the card view
 * @param cv the card view
 * @param c the card id to init the image
 */
void card_view__set_front(struct card_view* cv, enum card_id c);


/**
 * @brief Set the model card of a view card
 * @details Set the model, flip the card and turn it following direction
 * @param cv the card view
 * @param c the card model
 */
void card_view__set_model_card(struct card_view *cv, struct card *c);


/**
 * @brief Set the viewable position following window dimensions
 * @param cv the card view
 * @param x the view center
 * @param y the height
 */
void card_view__set_viewable_position(struct card_view* cv, int x, int y);


/**
 * @brief Render the image on screen
 * @param cv the card view
 */
void card_view__render(struct card_view* cv);


/**
 * @brief Frees memory used by the image
 * @param cv the card view to free
 */
void card_view__free(struct card_view* cv);


////////////////////////////////////////////////////////////////////
///     OPERATORS (COPY, DELETE, COMPARE, DEBUG)
////////////////////////////////////////////////////////////////////

void* card_view__copy_op(struct card_view *c);
void card_view__delete_op(struct card_view *c);
int card_view__compare_op(struct card_view *cv1, struct card_view *cv2);
void card_view__debug_op(const struct card_view *cv);

#endif
