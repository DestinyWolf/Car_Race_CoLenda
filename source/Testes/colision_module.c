#include "../Lib/colenda.h"
#include "colision_module.h"

//colisão entre player e objeto
int check_colision_player(sprite_t player, obstacle_t obstacle) {
    if(obstacle.type == 0) {
        if (((player.coord_x + 10 >= (obstacle.coord_x * 8) - (obstacle.size*8)) || (player.coord_x - 10 <= (obstacle.coord_x * 8) + (obstacle.size * 8))) && ((player.coord_y - 10 <= (obstacle.coord_y * 8) + (obstacle.size * 8 - 1)) && (player.coord_y + 10 >= obstacle.coord_y * 8 - (obstacle.size * 8 - 1)))) {
            return 1;
        }
    } else {
        if(((player.coord_x + 10 >= obstacle.coord_x - obstacle.size) || (player.coord_x - 10 <= obstacle.coord_x + obstacle.size)) && ((player.coord_y - 10 <= obstacle.coord_y + obstacle.size) && (player.coord_y + 10 >= obstacle.coord_y - obstacle.size))) {
        return 1;
        }
    }
    return 0;
}

//colisão entre tiro e objeto
int check_colision_bullet(sprite_t bullet, obstacle_t obstacle) {

    if(obstacle.type == 0) {
        if (((bullet.coord_x + 10 >= (obstacle.coord_x * 8) - (obstacle.size * 8 - 1)) || (bullet.coord_x - 10 <= (obstacle.coord_x * 8) + (obstacle.size * 8 - 1))) && ((bullet.coord_y - 10 >= (obstacle.coord_y * 8) + (obstacle.size * 8 - 1)) && (bullet.coord_y + 10 <= obstacle.coord_y * 8 - (obstacle.size * 8 - 1)))) {
            return 1;
        }
    } else {
        if(((bullet.coord_x + 10 >= obstacle.coord_x - obstacle.size) || (bullet.coord_x - 10 <= obstacle.coord_x + obstacle.size)) && ((bullet.coord_y - 10 <= obstacle.coord_y + obstacle.size) && (bullet.coord_y + 10 >= obstacle.coord_y - obstacle.size))) {
        return 1;
        }
    }
    return 0;
}