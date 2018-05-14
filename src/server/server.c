#include "server.h"
#include <dlfcn.h>
#include <time.h>

struct server* server__init(int argc, char **argv)
{
    struct server *this = safe_malloc(sizeof(struct server));

    this->argc = argc;
    this->argv = argv;
    this->is_graphic = DEFAULT_GRAPHIC_MODE_FLAG;
    this->clients_count = DEFAULT_CLIENT_COUNT;

    parse_opts(argc, argv, &(this->is_graphic), &(this->clients_count));

    if (this->is_graphic)
        this->app = app__init();

    return this;
}

void server__run(struct server* s)
{
    if (s->is_graphic) {
        app__run(s->app, s->argc, s->argv, s->clients_count);
    } else {
        if (s->clients_count > 0) {
            struct game *g = game__init(s->argc, s->argv, s->clients_count);
            game__loop(g);
            game__end(g);
            game__free(g);
        } else {
            printf(SRV_PREF"There are no clients"CLR"\n");
        }
    }
}

void server__free(struct server *s)
{
    if (s->is_graphic)
        app_free(s->app);

    free(s);
}

int main (int argc, char** argv)
{
    struct server* server = server__init(argc, argv);
    server__run(server);
    server__free(server);

    return EXIT_SUCCESS;
}
