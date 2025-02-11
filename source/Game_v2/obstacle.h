#ifndef OBSTACLE
#define OBSTACLE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../Lib/Colenda_Lib/colenda.h"
#include "offset_sprite.h"

typedef struct {
    uint64_t coord_x;       
    uint64_t coord_y;       
    uint64_t offset;
    uint64_t speed;       
    uint64_t reward;
    uint64_t on_frame;        
} obstacle_t;


int range_min_coord_x(int coord_x_player, int limite_min_pista);

int range_max_coord_x(int coord_x_player, int limite_max_pista);

/**
 * \brief função responsavel por gerar um número dentre o limite mínimo e máximo
 * \param[in] min inteiro que representa o menor valor possível
 * \param[in] max inteiro que representa o maior valor possível
 * \return o numero gerado dentro do espectro definido
 */
int 
random_number(int min, int max);

/**
 * \brief função responsavel por checar a disponibilidade de um dos registradores para uso 
 * \param[inout] sprites_obstacle_status vetor de inteiros que guarda a disponibilidade (0: livre) ou não (1: ocupado)
 * \return o numero do registrador vazio ou -1 caso não haja espaço disponível.
 */
int 
check_for_empty_reg(int sprites_obstacle_status[]);

void initialize_obstacle_vector();

/**
 * \brief função responsavel pela criação e renderização de um sprite de obstáculo na tela
 * \param[in] obstacle elemento do tipo obstáculo selecionado para renderização na tela
 * \param[in] coord_x
 * \param[in] coord_y
 * \param[in] base_reg
 * \param[in] sprites_obstacle_status vetor de inteiros que guarda a disponibilidade (0: livre) ou não (1: ocupado)
 * \param[inout] obstaculos_na_tela
 * \return 1 em caso de colisão, 0 caso contrario
 */
int 
create_sprite_obstacle(int player_coord_x, int track_min, int track_max, int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]);

void move_obstacles(int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]);

int all_obstacles_invisibles(int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]);
 
int restart_all_invisible_obstacles(int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]);

int clean_all_obstacles(int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]);
 

    
#endif
