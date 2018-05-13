#include "area_type.h"
#include "utils.h"

void* area_type__copy_op(const enum area_type *area)
{
    if (area == NULL)
        return NULL;

    enum area_type *new_area_type = safe_malloc(sizeof(enum area_type));
    *new_area_type = *area;
    return new_area_type;
}

void area_type__delete_op(enum area_type *area)
{
    free(area);
}

int area_type__compare_op(const enum area_type *area1, const enum area_type *area2)
{
    assert_not_null(area1, __func__, "area1 parameter");
    assert_not_null(area2, __func__, "area2 parameter");

    if (area1 < area2)
        return -1;
    else if (area1 == area2)
        return 0;
    else
        return 1;
}

void area_type__debug_op(const enum area_type *area)
{
    setvbuf (stdout, NULL, _IONBF, 0);
    if (area == NULL)
        printf("NULL");
    else
        printf("area type (%d)", *area);
}
