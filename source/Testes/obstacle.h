#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../Lib/colenda.h"

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
#define ROCK_BLOCK          11
#define TRASH_BAG           12
#define FIRE                13
#define LASER_DIAGONAL2     14
#define HEART               15
#define LOG                 16
#define BLUE_CAR            17
#define GREEN_CAR           18
#define PURPLE_CAR          19
#define YELLOW_CAR          20

typedef struct {
    uint64_t coord_x;       
    uint64_t coord_y;       
    uint64_t offset;
    uint64_t speed;       
    uint64_t reward;
    uint64_t size;
    uint64_t on_frame;        
} obstacle_t;


int 
random_number(int min, int max);

int 
check_for_empity_reg(int sprites_obstacle_status[]);

int 
create_sprite_obstacle(obstacle_t obstacle, int coord_x, int coord_y, int base_reg, int sprites_obstacle_status[]);

void 
initialize_obstacle_vector(obstacle_t vetor_obstaculos[]);

obstacle_t 
select_random_obstacle_from_vector(obstacle_t vetor_obstaculos[]);

int 
range_min_coord_x(int coord_x_player, int limite_min_pista);

int 
range_max_coord_x(int coord_x_player, int limite_max_pista);

void 
move_obstacles(obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[], int unidade, int reg_base_obstacles);

int 
random_obstacle(int cord_x_player, int cord_y_player, int limite_min_pista, int limite_max_pista obstacle_t obstaculos_na_tela[], obstacle_t vetor_obstaculos[], int sprites_obstacle_status[]);
    

