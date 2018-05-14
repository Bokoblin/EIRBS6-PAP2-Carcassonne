#include "card_view.h"
#include "../common/utils.h"

////////////////////////////////////////////////////////////////////
///     DEFINITION
////////////////////////////////////////////////////////////////////

const char *CARD_BACK_DARK_IMAGE = "res/game/cards/back_dark.jpg";
const char *CARD_BACK_LIGHT_IMAGE = "res/game/cards/back_light.jpg";


////////////////////////////////////////////////////////////////////
///     FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct card_view *card_view__init(int x, int y, int w, int h, SDL_Renderer *app_renderer, int is_first)
{
    struct card_view* this = safe_malloc(sizeof(struct card_view));
    this->front = NULL;
    this->back = image__init(x, y, w, h, is_first ? CARD_BACK_DARK_IMAGE : CARD_BACK_LIGHT_IMAGE, app_renderer);
    this->back->angle = (double) NORTH;
    this->card_model = NULL;
    this->is_shown = false;
    this->front_filename = malloc(29 * sizeof(char));
    strcpy(this->front_filename, "");

    return this;
}

void card_view__set_front(struct card_view* cv, enum card_id c)
{
    if (cv->front != NULL)
        image__free(cv->front);

    sprintf(cv->front_filename, "res/game/cards/card_%d.jpg", c);
    cv->front = image__init(cv->back->text_rect.x, cv->back->text_rect.y, cv->back->text_rect.w, cv->back->text_rect.h,
                            cv->front_filename, cv->back->renderer);
    cv->is_shown = true;
}

void card_view__set_model_card(struct card_view *cv, struct card *c)
{
    if (cv->front != NULL)
        image__free(cv->front);

    sprintf(cv->front_filename, "res/game/cards/card_%d.jpg", c->type.id);
    cv->front = image__init(cv->back->text_rect.x, cv->back->text_rect.y, cv->back->text_rect.w, cv->back->text_rect.h,
                            cv->front_filename, cv->back->renderer);

    cv->card_model = c;
    cv->front->angle = 90 * c->direction;
    cv->back->angle = 90 * c->direction;
    cv->is_shown = true;
}

void card_view__set_viewable_position(struct card_view* cv, int x, int y)
{
    assert_not_null(cv, __func__, "cv parameter");

    if (cv->front == NULL || cv->card_model == NULL)
        exit_on_error("Set the card model first");

    int view_x = x + cv->front->text_rect.w * cv->card_model->pos.x - (cv->back->text_rect.w / 2);
    //-1 due to model position design guidelines
    int view_y = y + cv->front->text_rect.h * (-1 * cv->card_model->pos.y) - (cv->back->text_rect.h / 2);

    cv->back->text_rect.x = view_x;
    cv->back->text_rect.y = view_y;
    cv->front->text_rect.x = view_x;
    cv->front->text_rect.y = view_y;
}

void card_view__render(struct card_view* cv)
{
    image__render(cv->is_shown ? cv->front : cv->back);
}

void card_view__free(struct card_view* cv)
{
    if (cv == NULL)
        return;

    image__free(cv->front);
    image__free(cv->back);
    cv->card_model = NULL;
    free(cv->front_filename);

    free(cv);
}


////////////////////////////////////////////////////////////////////
///     OPERATORS (COPY, DELETE, COMPARE, DEBUG)
////////////////////////////////////////////////////////////////////

void* card_view__copy_op(struct card_view *c)
{
    assert_not_null(c, __func__, "c parameter");

    struct card_view *new_card_view = safe_malloc(sizeof(struct card_view));
    new_card_view->card_model = c->card_model == NULL ? NULL : card__copy_op(c->card_model);
    new_card_view->back = c->back == NULL ? NULL : image__copy_op(c->back);
    new_card_view->front = c->front == NULL ? NULL : image__copy_op(c->front);
    new_card_view->is_shown = c->is_shown;
    new_card_view->front_filename = malloc(29 * sizeof(char));
    strcpy(new_card_view->front_filename, c->front_filename);

    return new_card_view;
}

void card_view__delete_op(struct card_view *c)
{
    card_view__free(c);
}

int card_view__compare_op(struct card_view *cv1, struct card_view *cv2)
{
    assert_not_null(cv1, __func__, "cv1 parameter");
    assert_not_null(cv2, __func__, "cv2 parameter");

    return card__compare_op(cv1->card_model, cv2->card_model);
}

void card_view__debug_op(const struct card_view *cv)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (cv == NULL)
        printf("NULL");
    else {
        printf("Card View (shown: %d, front: ", cv->is_shown);
        cv->front == NULL ?  printf("NULL, ") : image__debug_op(cv->front);
        printf("back: ");
        cv->back == NULL ?  printf("NULL, ") : image__debug_op(cv->back);
        printf("card model: ");
        cv->card_model == NULL ?  printf("NULL, \n") : card__debug_op(cv->card_model);
        printf(")\n");
    }
}
