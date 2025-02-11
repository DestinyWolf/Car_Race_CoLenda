#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "obstacle.h"
#include "offset_sprite.h"

// obstacles 
obstacle_t cone = {
    .offset = CONE,
    .speed= 8,
    .reward = 14,
    .on_frame = 0
};

obstacle_t single_wood = {
    .offset = SINGLE_WOOD,
    .speed= 12,
    .reward = 25,
    .on_frame = 0

};

obstacle_t double_wood = {
    .offset = DOUBLE_WOOD,
    .speed= 5,
    .reward = 10,
    .on_frame =0
};

obstacle_t bomb = {
    .offset = BOMB,
    .speed= 10,
    .reward = 20,
    .on_frame = 0
};

obstacle_t brick = {
    .offset = BRICK,
    .speed= 15,
    .reward = 30,
    .on_frame = 0
};

obstacle_t brick_block_1 = {
    .offset = BRICK_BLOCK_1,
    .speed= 5,
    .reward = 10,
    .on_frame = 0
};

obstacle_t brick_block_2 = {
    .offset = BRICK_BLOCK_2,
    .speed= 10,
    .reward = 15,
    .on_frame = 0
};

obstacle_t screwdriver = {
    .offset = SCREWDRIVER,
    .speed= 2,
    .reward = 20,
    .on_frame = 0
};

obstacle_t tool = {
    .offset = TOOL,
    .speed= 2,
    .reward = 20,
    .on_frame = 0
};

obstacle_t rock_1 = {
    .offset = ROCK_1,
    .speed= 2,
    .reward = 5,
    .on_frame = 0
};

obstacle_t rock_2 = {
    .offset = ROCK_2,
    .speed= 2,
    .reward = 5,
    .on_frame = 0
};

obstacle_t rock_block = {
    .offset = ROCK_BLOCK,
    .speed= 4,
    .reward = 8,
    .on_frame = 0
};

obstacle_t trash_bag = {
    .offset = TRASH_BAG,
    .speed= 6,
    .reward = 11,
    .on_frame = 0
}; 


obstacle_t trash = {
    .offset = TRASH,
    .speed= 2,
    .reward = 5,
    .on_frame = 0
};

obstacle_t hydrant = {
    .offset = HYDRANT,
    .speed= 2,
    .reward = 4,
    .on_frame = 0
};

obstacle_t spike = {
    .offset = SPIKE,
    .speed= 1,
    .reward = 3,
    .on_frame = 0
};

obstacle_t vetor_obstaculos[16];

//func to gen a max cordinate for obstacle 

int range_min_coord_x(int coord_x_player, int limite_min_pista){
    int range_min;
    if((coord_x_player - 50) <= limite_min_pista){
        range_min = limite_min_pista + 10;
    }
    else{
        range_min = coord_x_player - 50;
    }
    return range_min;
}

int range_max_coord_x(int coord_x_player, int limite_max_pista){
    int range_max;
    
    if(coord_x_player + 50 >= limite_max_pista){
        range_max = limite_max_pista - 11;
    }else range_max = coord_x_player + 50;
    return range_max;
}

int random_number(int min, int max){
    int num;

    //coordenada aleatória gerada
    num = (rand() % (max - min +1 ) + min);
    return num;
}



int check_for_empty_reg(int sprites_obstacle_status[]){
    int i =0, status, reg;

    while(i < 10){
        if(sprites_obstacle_status[i] == 0){
            //reg = base_reg + i;
            sprites_obstacle_status[i] = 1; //status registrador ocupado
            return i; 
        }
        i++;
    }
    return -1; //não há registrador vazio.
}

//alloc sprites in array

void initialize_obstacle_vector(){
    srand(time(NULL));

    vetor_obstaculos[0] = cone;
    vetor_obstaculos[1] = single_wood;
    vetor_obstaculos[2] = double_wood;
    vetor_obstaculos[3] = bomb;
    vetor_obstaculos[4] = brick;
    vetor_obstaculos[5] = brick_block_1;
    vetor_obstaculos[6] = brick_block_2;
    vetor_obstaculos[7] = screwdriver;
    vetor_obstaculos[8] = tool;
    vetor_obstaculos[9] = rock_1;
    vetor_obstaculos[10] = rock_2;
    vetor_obstaculos[11] = rock_block;
    vetor_obstaculos[12] = trash_bag;
    vetor_obstaculos[13] = trash;
    vetor_obstaculos[14] = hydrant;
    vetor_obstaculos[15] = spike;
    vetor_obstaculos[13] = trash;
    vetor_obstaculos[14] = hydrant;
    vetor_obstaculos[15] = spike;
}

int create_sprite_obstacle(int player_coord_x, int track_min, int track_max, int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]) {
    sprite_t new_obstacle;
    int index = 0;
    int reg = 20 + check_for_empty_reg(sprites_obstacle_status) ;

    if (reg == 19){
        return -1;
    }

    index = random_number(0, 16);

    new_obstacle.coord_x = random_number(range_min_coord_x(player_coord_x, track_min), range_max_coord_x(player_coord_x, track_max));
    new_obstacle.coord_y = 0;
    new_obstacle.data_register = reg;
    new_obstacle.visibility = 1;
    new_obstacle.offset = vetor_obstaculos[index].offset;


    obstaculos_na_tela[reg - 20].coord_x = new_obstacle.coord_x;
    obstaculos_na_tela[reg - 20].coord_y = new_obstacle.coord_y;
    obstaculos_na_tela[reg - 20].offset = new_obstacle.offset;
    obstaculos_na_tela[reg - 20].on_frame = new_obstacle.visibility;
    obstaculos_na_tela[reg - 20].reward = vetor_obstaculos[index].reward;
    obstaculos_na_tela[reg - 20].speed = vetor_obstaculos[index].speed;
    
    set_sprite(new_obstacle);

    return 1;
}

void move_obstacles(int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]) {
    sprite_t new_sprite;
    int new_obstacle_coord_y;


    for(int i = 0; i < 10; ++i) {
        if (sprites_obstacle_status[i] == 1) {
            new_obstacle_coord_y = obstaculos_na_tela[i].speed + obstaculos_na_tela[i].coord_y;
            if (new_obstacle_coord_y >= 471) {
                obstaculos_na_tela[i].on_frame = 0;
                sprites_obstacle_status[i] = 0;
                new_sprite.coord_x = 1;
                new_sprite.coord_y = 1;
                new_sprite.data_register = 20 + i;
                new_sprite.visibility = 0;
                new_sprite.offset = 0;
                
            } else {
                new_sprite.coord_x = obstaculos_na_tela[i].coord_x;
                new_sprite.coord_y = new_obstacle_coord_y;
                new_sprite.data_register = 20 + i;
                new_sprite.visibility = 1;
                new_sprite.offset = obstaculos_na_tela[i].offset;
            }
            set_sprite(new_sprite);
        }
    }
}

int all_obstacles_invisibles(int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]){
    sprite_t invisible_sprite;

    for (int i = 0; i < 10; i++)
    {   
        invisible_sprite.coord_x = obstaculos_na_tela[i].coord_x;
        invisible_sprite.coord_y = obstaculos_na_tela[i].coord_y;
        invisible_sprite.offset = obstaculos_na_tela[i].offset;
        invisible_sprite.speed = obstaculos_na_tela[i].speed;
        invisible_sprite.data_register = 20 + i; //20 + i
        invisible_sprite.visibility = 0;

        set_sprite(invisible_sprite);
    }
}

int restart_all_invisible_obstacles(int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]){
    sprite_t visible_sprite;

    for (int i = 0; i < 10; i++)
    {   
        visible_sprite.coord_x = obstaculos_na_tela[i].coord_x;
        visible_sprite.coord_y = obstaculos_na_tela[i].coord_y;
        visible_sprite.offset = obstaculos_na_tela[i].offset;
        visible_sprite.speed = obstaculos_na_tela[i].speed;
        visible_sprite.data_register = 20 + i; //20 + i
        visible_sprite.visibility = (obstaculos_na_tela[i].on_frame == 1) ? 1:0;

        //preserva o obstáculo
        obstaculos_na_tela[i].on_frame = 1;
        sprites_obstacle_status[i] = 1;

        if (obstaculos_na_tela[i].on_frame) {
            set_sprite(visible_sprite);
        }
    }
}

int clean_all_obstacles(int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]){
    sprite_t new_sprite;
    int reg_base = 20;
    
    for (int i = 0; i < 10; i++)
    {   
        /*Restart sprite*/
        new_sprite.coord_x = 1;
        new_sprite.coord_y = 1;
        new_sprite.offset = 0;
        new_sprite.speed = 0;
        new_sprite.data_register = reg_base + i; //20 + i
        new_sprite.visibility = 0;

        obstaculos_na_tela[i].on_frame = 0;
        sprites_obstacle_status[i] = 0;

        set_sprite(new_sprite);
    }
}
