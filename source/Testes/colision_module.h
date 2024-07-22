#ifndef COLISION_MODULE_COLENDA
#define COLISION_MODULE_COLENDA

#include "../Lib/colenda.h"
#include "obstacle.h"

/**
 * \brief função responsavel por checar a colisão entre um jogador e um obstaculo
 * \param[in] player struct do tipo sprite do jogador a ser verificada colisão
 * \param[in] obstacle struct do obstaculo que será comparada para verificar se o player colidiu
 * \return 1 em caso de colisão, 0 caso contrario
 */
int check_colision_player(sprite_t player, obstacle_t obstacle);

/**
 * \brief função responsavel por checar a colisão entre um disparo e um obstaculo
 * \param[in] bullet struct do tipo sprite do disparo a ser verificada colisão
 * \param[in] obstacle struct do obstaculo que será comparada para verificar se o disparo colidiu
 * \return 1 em caso de colisão, 0 caso contrario
 */
int check_colision_bullet(sprite_t bullet, obstacle_t obstacle);


#endif