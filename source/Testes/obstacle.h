#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../Lib/colenda.h"


typedef struct {
    uint64_t type;
    uint64_t id;
    uint64_t speed;       
    uint64_t reward;        
} obstacle_t;

int 
random_number(int min, int max);


sprite_t 
create_sprite_obstacle(int coord_x, int coord_y, int offset, int reg, int visibility, int speed);

void 
create_sprite_vector_obstacle(sprite_t *vetor_obstaculos, int coord_x, int coord_y, int offset, int reg, int visibility, int speed);

int 
range_min_coord_x(int coord_x_player);

int 
range_max_coord_x(int coord_x_player);

int 
random_obstacle(int cord_x_player, int cord_y_player);

