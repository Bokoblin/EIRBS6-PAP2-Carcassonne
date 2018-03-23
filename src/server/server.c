#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "../common/projCarcassonneInterface.h"

int main(int argc, char **argv)
{
  void *handle;
  char const* (*get_client1_name)(void);
  char *error;
  
  handle = dlopen("../install/client1.so", RTLD_NOW);
  /* If an error has occurred */
  if (!handle) {
    fputs (dlerror(), stderr);
    exit(1);
  }
  
  get_client1_name = dlsym(handle, "get_player_name");
  /* If the symbol searched wasn't found */
  if ((error = dlerror()) != NULL)  {
    fputs(error, stderr);
    exit(1);
  }


  /* TEST ZONE !!*/
  char const *player_name = get_client1_name();
  printf("%s", player_name);
  
  dlclose(handle);
  return EXIT_SUCCESS;
}
