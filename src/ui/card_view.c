#include "card_view.h"
#include "../common/utils.h"

////////////////////////////////////////////////////////////////////
///     DEFINITION
////////////////////////////////////////////////////////////////////

const char *CARD_BACK_DARK_IMAGE = "res/game/cards/back_dark.png";
const char *CARD_BACK_LIGHT_IMAGE = "res/game/cards/back_light.png";


////////////////////////////////////////////////////////////////////
///     FUNCTIONS IMPLEMENTATION
////////////////////////////////////////////////////////////////////

struct card_view *card_view__init(int x, int y, int w, int h, SDL_Renderer *app_renderer, int is_first)
{
    struct card_view* this = safe_malloc(sizeof(struct card_view));
    this->image = image__init(x, y, w, h, is_first ? CARD_BACK_DARK_IMAGE : CARD_BACK_LIGHT_IMAGE, app_renderer);
    this->image->angle = (double) NORTH;
    this->card_model = NULL;

    return this;
}

void card_view__set_front(struct card_view* cv, enum card_id c)
{
    char name[29];
    sprintf(name, "res/game/cards/card_%d.png", c);
    image__set_texture(cv->image, name);
}

void card_view__set_model_card(struct card_view *cv, struct card *c)
{
    cv->card_model = c;
    card_view__set_front(cv, c->type.id);
}

void card_view__set_viewable_position(struct card_view *cv, double board_center_x, double board_center_y, int scale_amount)
{
    assert_not_null(cv, __func__, "cv parameter");

    if (cv->image == NULL || cv->card_model == NULL)
        exit_on_error("Set the card model first");

    //=== Cards scaling

    if (DEFAULT_CARD_SIZE - scale_amount > 5)  {
        cv->image->text_rect.w = DEFAULT_CARD_SIZE - scale_amount;
        cv->image->text_rect.h = DEFAULT_CARD_SIZE - scale_amount;
    }

    //=== Cards positionning

    const int VPOS_REVERSE = -1; //due to model position design guidelines
    double cv_new_x = board_center_x + cv->image->text_rect.w * cv->card_model->pos.x - (cv->image->text_rect.w / 2);
    double cv_new_y = board_center_y + cv->image->text_rect.h *
            (VPOS_REVERSE * cv->card_model->pos.y) - (cv->image->text_rect.h / 2);

    cv->image->text_rect.x = (int) cv_new_x;
    cv->image->text_rect.y = (int) cv_new_y;
}

void card_view__update(struct card_view* cv)
{
    if (cv->card_model != NULL) {
        //Fix for SDL rotation order different than imposed order
        switch (cv->card_model->direction) {
            case NORTH:
            case SOUTH:
                cv->image->angle = 90 * cv->card_model->direction;
                break;
            case WEST:
            case EAST:
                cv->image->angle = 90 * ((cv->card_model->direction + 2) % NB_DIRECTIONS);
                break;
        }
    }
}

void card_view__render(struct card_view* cv)
{
    image__render(cv->image);
}

void card_view__free(struct card_view* cv)
{
    if (cv == NULL)
        return;

    image__free(cv->image);
    cv->image = NULL;
    cv->card_model = NULL;

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
    new_card_view->image = c->image == NULL ? NULL : image__copy_op(c->image);

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
        printf("Card View (image: ");
        cv->image == NULL ?  printf("NULL, ") : image__debug_op(cv->image);
        printf(", card model: ");
        cv->card_model == NULL ?  printf("NULL, \n") : card__debug_op(cv->card_model);
        printf(")\n");
    }
}
