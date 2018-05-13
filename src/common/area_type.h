#ifndef AREA_TYPE_H
#define AREA_TYPE_H

/**
 * @brief Area types
 * Lists all the possible area types on a card
 */
enum area_type
{
    ROAD,
    CITY,
    FIELD,
    INTERSECTION,
    ABBEY,
    INVALID_AREA,
};


////////////////////////////////////////////////////////////////////
///     OPERATORS (COPY, DELETE, COMPARE, DEBUG)
////////////////////////////////////////////////////////////////////

void* area_type__copy_op(const enum area_type *area);
void area_type__delete_op(enum area_type *area);
int area_type__compare_op(const enum area_type *area1, const enum area_type *area2);
void area_type__debug_op(const enum area_type *area);

#endif
