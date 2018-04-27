#include "../common/common_interface.h"
#include "client.h"
#include "../common/utils.h"
#include "../server/board.h"

/////////////////////////////////////////////////////////
/// Implementation of client usefull functions
/////////////////////////////////////////////////////////

int ask_for_player_name(struct client *client){
  client->name = "client_4410_1";
}
