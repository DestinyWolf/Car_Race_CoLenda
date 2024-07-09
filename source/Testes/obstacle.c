#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../Lib/colenda.h"
#include "obstacle.h"

color_t red = {7, 0, 0};
color_t black = {0, 0, 0};
color_t blue = {0, 0, 3};
color_t white = {7,7,7};

obstacle_t vetor_obstaculos[10];

// obstacles
obstacle_t bomb = {
    .id = 1,
    .type = sprite_t,
    .speed= 2,
    .reward = 20
};

obstacle_t tree = {
    .id = 2,
    .type = sprite_t,
    .speed = 1,
    .reward = 10
};

obstacle_t yellow_car = {
    .id = 3,
    .type = sprite_t,
    .speed = 3,
    .reward = 40;
};

obstacle_t purple_car = {
    .id = 4,
    .type = sprite_t,
    .speed = 4,
    .reward = 50;
};

obstacle_t blue_car = {
    .id = 5,
    .type = sprite_t,
    .speed = 3,
    .reward = 40
};

obstacle_t small_block = {
    .id = 6,
    .type = background_block_movel_t,
    .speed = 3,
    .reward = 60
};

obstacle_t red_vertical_line = {
    .id = 7,
    .type = background_block_movel_t,
    .speed = 3,
    .reward = 30
};

obstacle_t big_green_block = {
    .id = 8,
    .type = background_block_movel_t,
    .speed = 2,
    .reward = 10
};

obstacle_t blue_block = {
    .id = 9,
    .type = background_block_movel_t,
    .speed = 3,
    .reward = 20
};

obstacle_t red_block = {
    .id = 10,
    .type = background_block_movel_t,
    .speed = 4,
    .reward = 30
};

// cor, speed e reward mini_bloco = red, 1;


int random_number(int min, int max){
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

void create_sprite_vector_obstacle(obstacle_t *vetor_obstaculos, int coord_x, int coord_y, int offset, int reg, int visibility, int speed){
    vetor_obstaculos[0] = bomb;
    vetor_obstaculos[1] = tree;
    vetor_obstaculos[2] = yellow_car;
    vetor_obstaculos[3] = blue_car;
    vetor_obstaculos[4] = purple_car;

    // VETOR DE SPRITES CRIADOS - coordenadas fixas
    // vetor_obstaculos[0] = create_sprite_obstacle(coord_x, coord_y, BOMB, 1, 1, 1);
    // vetor_obstaculos[1] = create_sprite_obstacle(coord_x, coord_y, TREE, 2, 1, 2);
    // vetor_obstaculos[2] = create_sprite_obstacle(coord_x, coord_y, YELLOW_CAR, 3, 1, 1);
    // vetor_obstaculos[3] = create_sprite_obstacle(coord_x, coord_y, PURPLE_CAR, 4, 1, 2);
    // vetor_obstaculos[4] = create_sprite_obstacle(coord_x, coord_y, BLUE_CAR, 5, 1, 3);
}

int range_min_coord_x(int coord_x_player){
    int range_min = 96;
     
    if((coord_x_player - 50) <= 96){
        range_min = 96;
    }
    else{
        range_min = coord_x_player - 50;
    }
    return range_min;
}

int range_max_coord_x(int coord_x_player){
    int range_max = 536;
     
    if(coord_x_player + 50 >= 536){
        range_max = 536;
    }else range_max = coord_x_player + 50;
    return range_max;
}


int random_obstacle(int cord_x_player, int cord_y_player){
    int cord_x_obstacle, cord_y_obstacle, i_sprite;
    int value, teste_y, teste_x;
    background_block_movel_t mini_bloco;

    teste_y = cord_y_player + 20;
    //gera coordenada aleatória entre 96 e 536
    cord_x_obstacle = random_number(range_min_coord_x(cord_x_player), range_max_coord_x(cord_x_player));
    //teste_x =  cord_x_car + 10;
    cord_y_obstacle = random_number(teste_y, 100);
    //Teste criando e setando 1 obstaculo bomba
    create_sprite_obstacle(cord_x_obstacle, cord_y_obstacle, BOMB, 1, 1, 1);
    usleep(500000);

    //select random sprite from vector
    

    cord_x_obstacle = random_number(96, 536);
    //teste_x =  cord_x_car + 10;
    cord_y_obstacle = random_number(teste_y, 100);
    create_sprite_obstacle(cord_x_obstacle, cord_y_obstacle, TREE, 2, 1, 2);

    // usleep(500000);
    // cord_x_obstacle = random(96, 536);
    // //teste_x =  cord_x_car + 10;
    // cord_y_obstacle = random(teste_y, 210);
    // mini_bloco = {cord_x_obstacle, cord_y_obstacle, red, 1};
    // for i in range(): list_ostacules[i]

    return 0; // sucess
}