#ifndef COLISION_MODULE_COLENDA
#define COLISION_MODULE_COLENDA

#include "../Lib/colenda.h"

#define SPRITE_SIZE 10
#define BLOCK_SIZE 8

int check_colision_player_block(sprite_t player, background_block_movel_t block);
int check_colision_player_sprite(sprite_t player, sprite_t obj);
int check_colision_shot_sprite(sprite_t bullet, sprite_t obj);
int check_colision_shot_block(sprite_t bullet, sprite_t obj);

#endif