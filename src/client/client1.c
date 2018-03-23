#include "../common/projCarcassonneInterface.h"
#include <stdio.h>
#include <stdlib.h>

/* Public functions */

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name(){
  char* name;
  name=(char*)malloc(20*sizeof(char));
  printf("Entrez le nom du joueur\n");
  scanf("%s",name);
  return name;
}

/* Player initialization
 * PARAM:
 * - id: player ID
 * - n_players: number of players in the game
 * PRECOND:
 * - 0 <= id < n_players
 */
void initialize(unsigned int id,
		unsigned int n_players);

/* Computes next move
 * PARAM:
 * - card: the id of the card that has been drawn
 * - previous_moves: ordered list of previous moves starting from the last
 *   move issued by the player.
 * - n_moves: number of moves in previous_moves
 * PRECOND:
 * - previous_moves is an array of at least n_moves elements.
 * - previous_moves is an ordered list of previous moves starting from the last
 *   move of the player. Every move invalidated by the server has action
 *   FAILED. Every move validated by the server is faithfully transcribed.
 * RETURNS:
 * - the next move for the player.
 */
struct move play(enum card_id card,
		 struct move const previous_moves[], size_t n_moves);

/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize();

int main(){
  char const *A = get_player_name();
  printf("%s\n",A);
  return 0;
}
