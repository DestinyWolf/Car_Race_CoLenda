#include "../Lib/colenda.h"
#include "colision_module.h"

//colisão entre player e bloco
int check_colision_player_block(sprite_t player, background_block_movel_t block) {

    //colisão detectada
    if (((player.coord_x + SPRITE_SIZE >= (block.coord_x * 8)) || (player.coord_x - SPRITE_SIZE <= (block.coord_x * 8) + BLOCK_SIZE)) && (player.coord_y + SPRITE_SIZE <= (block.coord_y * 8) + BLOCK_SIZE)) {
        return 1;
    }
    return 0;
}

//colisão entre player e objeto
int check_colision_player_sprite(sprite_t player, sprite_t obj) {
    if(((player.coord_x + SPRITE_SIZE >= obj.coord_x - SPRITE_SIZE) || (player.coord_x - SPRITE_SIZE <= obj.coord_x + SPRITE_SIZE)) && (player.coord_y + SPRITE_SIZE <= obj.coord_y - SPRITE_SIZE)) {
        return 1;
    }
    return 0;
}

//colisão entre tiro e objeto
int check_colision_shot_sprite(sprite_t bullet, sprite_t obj) {
    if(((bullet.coord_x + SPRITE_SIZE >= obj.coord_x - SPRITE_SIZE) || (bullet.coord_x - SPRITE_SIZE <= obj.coord_x + SPRITE_SIZE)) && (bullet.coord_y + SPRITE_SIZE <= obj.coord_y - SPRITE_SIZE)) {
        return 1;
    }
    return 0;
}

//colisão entre tiro e bloco
int check_colision_shot_block(sprite_t bullet, sprite_t obj) {
    if (((bullet.coord_x + SPRITE_SIZE >= (block.coord_x * 8)) || (bullet.coord_x - SPRITE_SIZE <= (block.coord_x*8) + BLOCK_SIZE)) && (bullet.coord_y + SPRITE_SIZE <= (block.coord_y * 8) + BLOCK_SIZE)) {
        return 1;
    }
    return 0;
}