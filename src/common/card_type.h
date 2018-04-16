#ifndef CARD_TYPE_DEF_H
#define CARD_TYPE_DEF_H

#include "common_interface.h"
#include "area_type.h"

#define MAX_ZONES (LAST_POS-1)

////////////////////////////////////////////////////////////////////
///     STRUCTURES
////////////////////////////////////////////////////////////////////

/**
 * @brief card_type
 * Describes the card of that type
 */
struct card_type
{
    enum card_id id;
    enum area_type areas[MAX_ZONES]; // indexed by enum place
};


////////////////////////////////////////////////////////////////////
///     CARD TYPE FUNCTIONS
////////////////////////////////////////////////////////////////////

struct card_type card__id_to_type(enum card_id id);


#endif // CARD_TYPE_DEF_H