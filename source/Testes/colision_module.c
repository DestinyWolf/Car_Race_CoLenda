#include "../Lib/colenda.h"
#include "colision_module.h"

//colisão entre player e objeto
int check_colision_player(sprite_t player, obstacle_t obstacle) {
    
    if(((player.coord_x + 10 >= obstacle.coord_x - obstacle.size) || (player.coord_x - 10 <= obstacle.coord_x + obstacle.size)) && ((player.coord_y - 10 <= obstacle.coord_y + obstacle.size) && (player.coord_y + 10 >= obstacle.coord_y - obstacle.size))) {
    return 1;
    }
    
    return 0;
}

//colisão entre tiro e objeto
int check_colision_bullet(sprite_t bullet, obstacle_t obstacle) {

    
    if(((bullet.coord_x + 10 >= obstacle.coord_x - obstacle.size) || (bullet.coord_x - 10 <= obstacle.coord_x + obstacle.size)) && ((bullet.coord_y - 10 <= obstacle.coord_y + obstacle.size) && (bullet.coord_y + 10 >= obstacle.coord_y - obstacle.size))) {
    return 1;
    }
    
    return 0;
}