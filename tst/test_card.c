#include <stdlib.h>
#include "test_utils.h"
#include "../src/common/card.h"

int test_card__empty()
{
    printf("%s... ", __func__);

    struct card_type ct = { CARD_JUNCTION_CITY, { ROAD, CITY, ROAD, ROAD}}; //FIXME: check if it's that

    struct card* c = card__empty(ct);

    if (c->type.id != CARD_JUNCTION_CITY) {
        card__free(c);
        return !SUCCESS;
    }

    if (c->orientation != DEFAULT_ORIENTATION) {
        card__free(c);
        return !SUCCESS;
    }

    card__free(c);

    return SUCCESS;
}


int test_card__place()
{
    printf("%s... ", __func__);

    //TODO : test_card__place

    return !SUCCESS;
}


int test_card__draw()
{
    printf("%s... ", __func__);

    //TODO : test_card__draw

    return !SUCCESS;
}


int test_card__clockwise_rotation()
{
    printf("%s... ", __func__);

    //TODO : test_card__clockwise_rotation

    return !SUCCESS;
}


int test_card__trigo_rotation()
{
    printf("%s... ", __func__);

    //TODO : test_card__trigo_rotation

    return !SUCCESS;
}


int main()
{
    printf("----------- TEST STACK -----------\n");

    print_success(test_card__empty());
    print_success(test_card__place());
    print_success(test_card__draw());
    print_success(test_card__clockwise_rotation());
    print_success(test_card__trigo_rotation());

    return SUCCESS;
}
