#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../Lib/colenda.h"
#include "obstacle.h"

// color_t red = {7, 0, 0};
// color_t black = {0, 0, 0};
// color_t blue = {0, 0, 3};
// color_t white = {7,7,7};

int sprites_obstacle_status[10];

obstacle_t vetor_obstaculos[10];

// obstacles antigos
obstacle_t bomb = {
    .coord_x,
    .coord_y,
    .speed= 2,
    .reward = 20
};

obstacle_t tree = {
    .speed = 1,
    .reward = 10
};

obstacle_t yellow_car = {
    .speed = 3,
    .reward = 40
};

obstacle_t purple_car = {
    .speed = 4,
    .reward = 50
};

obstacle_t blue_car = {
    .speed = 3,
    .reward = 40
};



// cor, speed e reward mini_bloco = red, 1;


int random_number(int min, int max){
    int num, value;
    value = rand();

    //coordenada aleatória gerada
    num = ((rand() % (max - min)) + min);
    return num;
}

sprite_t create_sprite_obstacle(int coord_x, int coord_y, int offset, int reg, int visibility, int speed, int reward){
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

void set_vector_obstacle(obstacle_t vetor_obstaculos[], int coord_x, int coord_y, int offset, int reg, int visibility, int speed){
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

int range_min_coord_x(int coord_x_player, int limite_min_pista){
    int range_min = limite_min_pista;
     
    if((coord_x_player - 50) <= 96){
        range_min = limite_min_pista;
    }
    else{
        range_min = coord_x_player - 50;
    }
    return range_min;
}

int range_max_coord_x(int coord_x_player, int limite_max_pista){
    int range_max = limite_max_pista;
     
    if(coord_x_player + 50 >= 536){
        range_max = limite_max_pista;
    }else range_max = coord_x_player + 50;
    return range_max;
}

void move_obstacle(obstacle_t obstacle){
    while()
}

int valida_coordenada(int coord_x){
    //coordenada dentro do range
    return coord;
}

//movimento do obstáculo e desaparecimento depois



int random_obstacle(int cord_x_player, int cord_y_player){
    int cord_x_obstacle, cord_y_obstacle, i_sprite;
    int value, teste_x;

    //gera coordenada aleatória entre 96 e 536
    cord_x_obstacle = random_number(range_min_coord_x(cord_x_player), range_max_coord_x(cord_x_player));
    cord_y_obstacle = 10; //0+10

    //Teste criando e setando 1 obstaculo bomba
    create_sprite_obstacle(cord_x_obstacle, cord_y_obstacle, BOMB, 1, 1, 1);
    sleep(1);

    //select random sprite from vector
    
    create_sprite_obstacle(cord_x_obstacle, cord_y_obstacle, TREE, 2, 1, 2);

    // usleep(500000);
    // cord_x_obstacle = random(96, 536);
    // //teste_x =  cord_x_car + 10;
    // cord_y_obstacle = random(teste_y, 210);
    // mini_bloco = {cord_x_obstacle, cord_y_obstacle, red, 1};
    // for i in range(): list_ostacules[i]

    return 0; // sucess
}