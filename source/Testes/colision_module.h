#ifndef COLISION_MODULE_COLENDA
#define COLISION_MODULE_COLENDA

#include "../Lib/colenda.h"
#include "obstacle.h"

int check_colision_player(sprite_t player, obstacle_t obstacle);
int check_colision_bullet(sprite_t bullet, obstacle_t obstacle);


#endif