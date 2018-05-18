#ifndef PLAYER_H
#define PLAYER_H

#include "../common/interface.h"
#include "../common/utils.h"

#define DEFAULT_MEEPLE_NUMBER 8 //With one for score counting

////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////

struct player
{
    unsigned int score;
    unsigned int nb_meeples;
    unsigned int id;
    void* lib_ptr;
    const char * (*get_player_name)(void);
    const char * (*initialize)(unsigned int, unsigned int);
    struct move (*play)(enum card_id, const struct move*, size_t);
    void (*finalize)(void);
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////

/**
 * @brief Inits the player
 * @return a newly allocated struct player pointer
 */
struct player *player__init(unsigned int id, void *lib_ptr);


/**
 * @brief Print the player id, name and score
 * @param p the player
 */
void player__print_name_and_score(struct player *p);


/**
 * @brief Release memory of a player
 * @param p the player
 */
void player__free(struct player *p);


////////////////////////////////////////////////////////////////////
///     OPERATORS (COPY, DELETE, COMPARE, DEBUG)
////////////////////////////////////////////////////////////////////

void* player__copy_op(const struct player *p);
void player__delete_op(struct player *p);
int player__compare_op(const struct player *p1, const struct player *p2);
void player__debug_op(const struct player *p);

#endif
