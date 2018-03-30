#ifndef CLIENTS_CONTAINER_H
#define CLIENTS_CONTAINER_H

#include <stdio.h>
#include <stdlib.h>
#include "../common/utils.h"

enum card_id;
struct move;

struct client
{
    void* lib_ptr;
    const char * (*get_player_name)(void);
    const char * (*initialize)(unsigned int, unsigned int);
    struct move (*play)(enum card_id, const struct move*, size_t);
    void (*finalize)(void);
};

/**
 * @brief Clients list structure
 */
struct client_container
{
    struct client** clients_array;
    unsigned int current_size;
};

/**
 * @brief Inits the client
 * @return a newly allocated struct client pointer
 */
struct client* init_client();

/**
 * @brief Inits the clients structure
 * @param clients_count the number of clients to add
 * @return a newly created client container
 */
struct client_container* init_client_container(unsigned int clients_count);


/**
 * @brief Release memory of the container
 * @param container the client container
 */
void free_client_container(struct client_container* container);

#endif