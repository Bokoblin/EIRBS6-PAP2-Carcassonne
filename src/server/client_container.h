#ifndef CLIENTS_STRUCT_H
#define CLIENTS_STRUCT_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Clients list structure
 */
struct client_container
{
    void** clients_pointer_array;
    char** clients_files_array;
    size_t current_size;
    size_t max_size;
};


/**
 * @brief Inits the clients structure
 * @param clients_count the number of clients to add
 *
 * @author Arthur
 * @date 24/03/18
 */
struct client_container* init_client_container(unsigned int clients_count);


/**
 * @brief Release memory of the container
 * @param container the client container
 *
 * @author Arthur
 * @date 24/03/18
 */
void free_client_container(struct client_container* container);

#endif