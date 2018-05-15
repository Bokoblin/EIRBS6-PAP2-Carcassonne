#ifndef SERVER_H
#define SERVER_H

#include "game.h"
#include "../ui/app.h"

#define DEFAULT_GRAPHIC_MODE_FLAG 0
#define DEFAULT_CLIENT_COUNT 0

////////////////////////////////////////////////////////////////////
///     STRUCTURE
////////////////////////////////////////////////////////////////////

struct server
{
    int argc;
    char ** argv;
    unsigned int is_graphic;
    unsigned int clients_count;
    struct app* app;
};


////////////////////////////////////////////////////////////////////
///     FUNCTIONS
////////////////////////////////////////////////////////////////////

/**
 * @brief Inits the server
 * @param argc the program arguments count
 * @param argv the program arguments array
 * @return a newly created server
 */
struct server* server__init(int argc, char **argv);


/**
 * @brief Runs the server
 * @param s the server
 */
void server__run(struct server* s);


/**
 * @brief Runs the server console app part
 * @param s the server
 */
void server__run_console_app(struct server* s);


/**
 * @brief Runs the server graphic app part
 * @param s the server
 */
void server__run_graphic_app(struct server* s);


/**
 * @brief Frees the memory used by the server
 * @param s the server
 */
void server__free(struct server *s);


#endif
