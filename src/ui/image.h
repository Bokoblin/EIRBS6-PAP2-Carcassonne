#ifndef IMAGE_H
#define IMAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL_image.h>


////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////

struct image
{
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_Rect text_rect;
    double angle;
    char *filename;
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
 * @param filename the image file name
 * @param app_renderer the app's renderer
 * @return a newly allocated pointer of an image
 */
struct image *image__init(int x, int y, int w, int h, const char *filename, SDL_Renderer *app_renderer);


/**
 * @brief Sets the renderer of an image
 * @param image
 * @param renderer
 */
void image__set_renderer(struct image* image, SDL_Renderer* renderer);


/**
 * @brief Replace the image texture
 * @param im the image
 * @param filename the filename to create the texture
 */
void image__set_texture(struct image* im, char *filename);


/**
 * @brief Checks if coordinates belong to an image
 * @param image the image
 * @param x the x-axis coordinate
 * @param y the y-axis coordinate
 * @return 1 on matching, 0 otherwise
 */
int image__contains(struct image* image, int x, int y);


/**
 * @brief Renders an image on its renderer
 * @param image the image to render
 */
void image__render(struct image* image);


/**
 * @brief Frees memory used by the image
 * @param image the image to free
 */
void image__free(struct image* image);


////////////////////////////////////////////////////////////////////
///     OPERATORS (COPY, DELETE, COMPARE, DEBUG)
////////////////////////////////////////////////////////////////////

void* image__copy_op(struct image *im);
void image__delete_op(struct image *im);
void image__debug_op(struct image *im);
int image__compare_op(struct image *im1, struct image * im2);


#endif
