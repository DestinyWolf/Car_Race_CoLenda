#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../Lib/colenda.h"

color_t red = {7, 0, 0};
sprite_t vetor_obstaculos[5];

int random_coord(int min, int max){
    int num, value;
    value = rand();

    //coordenada aleatória gerada
    num = ((rand() % (max - min)) + min);
    return num;
}

sprite_t create_sprite_obstacle(int coord_x, int coord_y, int offset, int reg, int visibility, int speed){
    sprite_t obstacle;
    obstacle.coord_x = coord_x;
    obstacle.coord_y = coord_y;
    obstacle.data_register = reg;
    obstacle.offset = offset;
    obstacle.speed = speed;
    obstacle.visibility = visibility;
    set_sprite(obstacle);
    return obstacle;
}

void create_sprite_vector_obstacle(sprite_t *vetor_obstaculos, int coord_x, int coord_y, int offset, int reg, int visibility, int speed){
    vetor_obstaculos[0] = create_sprite_obstacle(coord_x, coord_y, BOMB, 1, 1, 1);
    vetor_obstaculos[1] = create_sprite_obstacle(coord_x, coord_y, TREE, 2, 1, 2);
    vetor_obstaculos[2] = create_sprite_obstacle(coord_x, coord_y, YELLOW_CAR, 3, 1, 1);
    vetor_obstaculos[3] = create_sprite_obstacle(coord_x, coord_y, PURPLE_CAR, 4, 1, 2);
    vetor_obstaculos[4] = create_sprite_obstacle(coord_x, coord_y, BLUE_CAR, 5, 1, 3);
}


int random_obstacle(int cord_x_car, int cord_y_car){
    int cord_x_obstacle, cord_y_obstacle;
    int value, teste_y, teste_x;
    background_block_movel_t mini_bloco;

    teste_y = cord_y_car + 20;

    //gera coordenada aleatória entre 96 e 536
    cord_x_obstacle = random_coord(96, 536);
    //teste_x =  cord_x_car + 10;
    cord_y_obstacle = random_coord(teste_y, 210);
    create_sprite_obstacle(cord_x_obstacle, cord_y_obstacle, BOMB, 1, 1, 1);

    usleep(500000);
    cord_x_obstacle = random_coord(96, 536);
    //teste_x =  cord_x_car + 10;
    cord_y_obstacle = random_coord(teste_y, 210);
    create_sprite_obstacle(cord_x_obstacle, cord_y_obstacle, TREE, 2, 1, 2);

    // usleep(500000);
    // cord_x_obstacle = random(96, 536);
    // //teste_x =  cord_x_car + 10;
    // cord_y_obstacle = random(teste_y, 210);
    // mini_bloco = {cord_x_obstacle, cord_y_obstacle, red, 1};
    // for i in range(): list_ostacules[i]

    return 0; // sucess
}