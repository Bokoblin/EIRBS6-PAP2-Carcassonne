#include <stdlib.h>
#include "test_utils.h"
#include "../src/common/common_interface.h"
#include "../src/common/card.h"

void* operator_copy(void* given_card)
{
    enum card_id *_given_card = given_card;
    enum card_id *new_card = malloc(sizeof(enum card_id));
    *new_card = *_given_card;
    return new_card;
}

void operator_delete(void* given_card)
{
    free(given_card);
}

void operator_debug(void* given_card)
{
    enum card_id *_given_card = given_card;
    printf("%d ", *_given_card);
}

int test_card__empty()
{
    printf("%s... ", __func__);

    struct card_type ct = {CARD_JUNCTION_CITY, { ROAD, CITY, ROAD, ROAD}}; //FIXME: check if it's that

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

    printf("- Not tested yet - ");

    return !SUCCESS;
}


int test_card__get_area()
{
   printf("%s... ", __func__);


   struct card_type ct = {CARD_MONASTERY_ALONE,
           {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ABBEY}};

   struct card *c = card__empty(ct);

   for (int i = 1 ; i < (MAX_ZONES-1) ; i++) {
        if (card__get_area(c, (enum place) i) != FIELD) {
            card__free(c);
            return !SUCCESS;
        }
   }

   if (card__get_area(c, POS_CENTER) != ABBEY) {
       card__free(c);
       return !SUCCESS;
   }

   card__free(c);

   return SUCCESS;

}


int test_card__are_matching_direction_success_case()
{
    printf("%s... ", __func__);

    struct card_type ct1 = {CARD_MONASTERY_ALONE,
            {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ABBEY}};

    struct card_type ct2 = {CARD_MONASTERY_ROAD,
            {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, ABBEY}};

    struct card *c1 = card__empty(ct1);
    struct card *c2 = card__empty(ct2);

    if (card__are_matching_direction(c1, c2, EAST)) {
        card__free(c1);
        card__free(c2);
        return SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return !SUCCESS;
}

int test_card__are_matching_direction_failure_case()
{
    printf("%s... ", __func__);

    struct card_type ct1 = {CARD_MONASTERY_ALONE,
            {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ABBEY}};

    struct card_type ct2 = {CARD_PLAIN_CITY_ROAD,
            {FIELD, FIELD, ROAD, FIELD, ROAD, FIELD, CITY, CITY, CITY, CITY, CITY, CITY, FIELD}};

    struct card *c1 = card__empty(ct1);
    struct card *c2 = card__empty(ct2);

    if (!card__are_matching_direction(c1, c2, EAST)) {
        card__free(c1);
        card__free(c2);
        return SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return !SUCCESS;
}

int test_card__link_at_direction()
{
    printf("%s... ", __func__);

    struct card_type ct1 = {CARD_MONASTERY_ALONE,
            {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ABBEY}};

    struct card_type ct2 = {CARD_MONASTERY_ROAD,
            {FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, FIELD, ROAD, FIELD, FIELD, FIELD, ABBEY}};

    struct card *c1 = card__empty(ct1);
    struct card *c2 = card__empty(ct2);

    card__link_at_direction(c1, c2, EAST);

    if ((c1->neighbors[EAST] == c2) && (c2->neighbors[WEST] == c1)) {
        card__free(c1);
        card__free(c2);
        return SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return !SUCCESS;
}


int test_card__draw()
{
    printf("%s... ", __func__);

    struct stack *s = stack__empty(&operator_copy, &operator_delete, &operator_debug);

    enum card_id id =  CARD_JUNCTION_CITY;

    stack__push(s, &id);

    enum card_id cid = card__draw(s);

    if (cid != CARD_JUNCTION_CITY) {
        stack__free(s);
        return !SUCCESS;
    }

    stack__free(s);

    return SUCCESS;
}


int test_card__set_orientation()
{
    printf("%s... ", __func__);

    struct card_type ct = { CARD_JUNCTION_CITY, { ROAD, CITY, ROAD, ROAD}};

    struct card *c = card__empty(ct);

    card__set_orientation(c, NORTH_TO_WEST);

    if (c->orientation == NORTH_TO_WEST) {
        card__free(c);
        return SUCCESS;
    }

    card__free(c);

    return !SUCCESS;
}



int main()
{
    printf("----------- TEST CARD -----------\n");

    print_success(test_card__empty());
    print_success(test_card__place());
    print_success(test_card__get_area());
    print_success(test_card__are_matching_direction_success_case());
    print_success(test_card__are_matching_direction_failure_case());
    print_success(test_card__link_at_direction());
    print_success(test_card__draw());
    print_success(test_card__set_orientation());

    return SUCCESS;
}
