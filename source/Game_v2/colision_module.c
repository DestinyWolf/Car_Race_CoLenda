#include "../Lib/Colenda_Lib/colenda.h"
#include "colision_module.h"


int check_colision_player(sprite_t player, obstacle_t obstacle) {
    
    /*caso alguma parte da area do jogador esteja dentro da area do objeto*/
    if(((player.coord_x - 10 < obstacle.coord_x - 10 && player.coord_x + 10 > obstacle.coord_x - 10 && player.coord_x + 10 < obstacle.coord_x + 10) || (player.coord_x - 10 < obstacle.coord_x + 10 && player.coord_x - 10 > obstacle.coord_x - 10 && player.coord_x + 10 > obstacle.coord_x + 10) ) && ((player.coord_y - 10 <= obstacle.coord_y + obstacle.size) && (player.coord_y + 10 >= obstacle.coord_y - obstacle.size))) {
        
        return 1; /*retorna 1 indicando que houve uma colisão*/
    }
    
    return 0;     /*caso contrario, retorna 0*/
}


int check_colision_bullet(sprite_t bullet, obstacle_t obstacle) {

    /*caso alguma parte da area do tiro esteja dentro da area do obstaculo*/
    if(((bullet.coord_x - 10 < obstacle.coord_x - 10 && bullet.coord_x + 10 > obstacle.coord_x - 10 && bullet.coord_x + 10 < obstacle.coord_x + 10) || (bullet.coord_x - 10 < obstacle.coord_x + 10 && bullet.coord_x - 10 > obstacle.coord_x - 10 && bullet.coord_x + 10 > obstacle.coord_x + 10)) && ((bullet.coord_y - 10 <= obstacle.coord_y + obstacle.size) && (bullet.coord_y + 10 >= obstacle.coord_y - obstacle.size))) {
        
        /*retorna 1, indicando que houve uma colisão*/
        return 1;
    }
    
    /*caso contrario, retorna 0*/
    return 0;
}