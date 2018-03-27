#include "../common/projCarcassonneInterface.h"
#include <stdio.h>

/////////////////////////////////////////////////////////
/// Implementation of interface functions
/////////////////////////////////////////////////////////

char const* get_player_name()
{
  /*
  char* name;
  name = (char*) malloc(20 * sizeof(char));
  printf("Entrez le nom du joueur:\n");
  scanf("%s", name);
  return name;
  */
  return("Joueur1");
}

void initialize(unsigned int id, unsigned int n_players)
{
    //TODO: Is it a client function? OUI TOUTES LES FONCTIONS SONT IMPLÉMENTÉES PAR LE CLIENTS
  printf("initialise fonctionne pour %s\n", get_player_name());
}

struct move play(enum card_id card, struct move const previous_moves[], size_t n_moves)
{
    //TODO
   printf("play fonctionne pour %s\n", get_player_name());
};

/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize()
{
    //TODO: Is it a client function?
   printf("finalize fonctionne pour %s\n", get_player_name());
}

int main()
{
  char const *A = get_player_name();
  printf("%s\n", A);
  return 0;
}
