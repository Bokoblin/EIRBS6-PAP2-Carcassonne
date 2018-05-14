#include "image.h"
#include "../common/utils.h"

////////////////////////////////////////////////////////////////////
///     FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct image *image__init(int x, int y, int w, int h, const char *filename, SDL_Renderer *app_renderer)
{
    struct image* this = safe_malloc(sizeof(struct image));

    this->texture = IMG_LoadTexture(app_renderer, filename);
    if (this->texture == NULL) {
        SDL_Log("IMG_LoadTexture() failed: %s", SDL_GetError());
    } else {
        SDL_QueryTexture(this->texture, NULL, NULL, &this->text_rect.w, &this->text_rect.h);
        this->text_rect.x = x;
        this->text_rect.y = y;
        this->text_rect.w = w == -1 ? this->text_rect.w : w;
        this->text_rect.h = h == -1 ? this->text_rect.h : h;
    }
    this->renderer = app_renderer;
    this->angle = 0;
    this->filename = filename;

    return this;
}

void image__set_renderer(struct image* image, SDL_Renderer* renderer)
{
    image->renderer = renderer;
}

int image__contains(struct image* image, int x, int y)
{
    if (image != NULL)
        return x > image->text_rect.x
                && x < image->text_rect.x + image->text_rect.w
                && y > image->text_rect.y
                && y < image->text_rect.y + image->text_rect.h;
    else
        return false;
}

void image__render(struct image* image)
{
    if (image != NULL) {
        SDL_RenderCopyEx(image->renderer, image->texture, NULL, &image->text_rect, image->angle, NULL, SDL_FLIP_NONE);
    }
}

void image__free(struct image* image)
{
    if (image == NULL)
        return;

    SDL_DestroyTexture(image->texture);
    free(image);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS (COPY, DELETE, COMPARE, DEBUG)
////////////////////////////////////////////////////////////////////

void* image__copy_op(struct image *im)
{
    assert_not_null(im, __func__, "im parameter");

    struct image *new_image = safe_malloc(sizeof(struct image));
    new_image->angle = im->angle;
    new_image->filename = im->filename;
    new_image->text_rect = im->text_rect;
    new_image->renderer = im->renderer;

    new_image->texture = IMG_LoadTexture(new_image->renderer, new_image->filename);
    if (new_image->texture == NULL) {
        printf("old filename was %s\n", im->filename);
        printf("new filename was %s\n", new_image->filename);
        SDL_Log("IMG_LoadTexture() failed: %s", SDL_GetError());
    } else {
        SDL_QueryTexture(new_image->texture, NULL, NULL, NULL, NULL);
    }

    return new_image;
}

void image__delete_op(struct image *im)
{
    image__free(im);
}

void image__debug_op(struct image *im)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (im == NULL)
        printf("NULL");
    else {
        printf("Image (filename: %s, position {%d, %d}, width: %d, height: %d, angle: %lf )\n",
               im->filename, im->text_rect.x, im->text_rect.y, im->text_rect.w, im->text_rect.h, im->angle);
    }
}

int image__compare_op(struct image *im1, struct image * im2)
{
    assert_not_null(im1, __func__, "im1 parameter");
    assert_not_null(im2, __func__, "im2 parameter");

    if (im1 < im2)
        return -1;
    else if (im1 == im2)
        return 0;
    else
        return 1;
}
