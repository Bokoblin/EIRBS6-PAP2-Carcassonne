#include "server.h"
#include <dlfcn.h>

int main(int argc, char** argv)
{
    //=== Program setup

    unsigned int is_graphic = DEFAULT_GRAPHIC_MODE_FLAG;
    unsigned int clients_count = DEFAULT_CLIENT_COUNT;
    parse_opts(argc, argv, &is_graphic, &clients_count);

    //=== Game setup, run and free

    if (clients_count > 0) {
        struct game *g = game__init(argc, argv, clients_count);
        game__main(g);
        game__free(g);
    } else {
        printf(SRV_PREF"There are no clients\n"CLR);
    }


    return EXIT_SUCCESS;
}
