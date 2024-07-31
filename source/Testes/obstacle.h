#ifndef OBSTACLE
#define OBSTACLE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../Lib/colenda.h"
#include "offset_sprite.h"

/*
#define CONE                0
#define SINGLE_WOOD         1
#define DOUBLE_WOOD         2
#define BOMB                3
#define BRICK               4
#define BRICK_BLOCK_1       5
#define BRICK_BLOCK_2       6
#define SCREWDRIVER         7
#define TOOL                8
#define ROCK_1              9
#define ROCK_2              10
#define LASER_VERTICAL      11
#define ROCK_BLOCK          12
#define TRASH_BAG           13
#define TRASH               14
#define HYDRANT             15
#define SPIKE               16
#define FIRE                17
#define RED_CAR             18
#define BLUE_CAR            19
#define N1                  20
#define N2                  21
#define N3                  22
#define X                   23
#define JOYSTICK            24
*/

typedef struct {
    uint64_t coord_x;       
    uint64_t coord_y;       
    uint64_t offset;
    uint64_t speed;       
    uint64_t reward;
    uint64_t size;
    uint64_t on_frame;        
} obstacle_t;

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
 * \param[in] sprites_obstacle_status vetor de inteiros que guarda a disponibilidade (0: livre) ou não (1: ocupado)
 * \return o numero do registrador vazio ou -1 caso não haja espaço disponível.
 */
int 
check_for_empty_reg(int sprites_obstacle_status[]);

/**
 * \brief função responsavel pela criação e renderização de um sprite de obstáculo na tela
 * \param[in] obstacle elemento do tipo obstáculo selecionado para renderização na tela
 * \param[in] coord_x
 * \param[in] coord_y
 * \param[in] base_reg
 * \param[in] sprites_obstacle_status vetor de inteiros que guarda a disponibilidade (0: livre) ou não (1: ocupado)
 * \param[in] obstaculos_na_tela
 * \return 1 em caso de colisão, 0 caso contrario
 */
int 
create_sprite_obstacle(obstacle_t obstacle, int coord_x, int coord_y, int base_reg, int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]);

void initialize_obstacle_vector(obstacle_t vetor_obstaculos[]);

obstacle_t select_random_obstacle_from_vector(obstacle_t vetor_obstaculos[]);

int range_min_coord_x(int coord_x_player, int limite_min_pista);

int range_max_coord_x(int coord_x_player, int limite_max_pista);
 
void move_obstacles(obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[], int unidade, int reg_base_obstacles);

int all_obstacles_invisibles(obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[]);
 
int restart_all_invisible_obstacles(obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[]);

int clean_all_obstacles(obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[]);
 
int random_obstacle(int cord_x_player, int cord_y_player, int limite_min_pista, int limite_max_pista, obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[], obstacle_t vetor_obstaculos[]);
    

