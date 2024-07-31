#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "obstacle.h"
#include "offset_sprite.h"

//obstacle_t vetor_obstaculos[16];

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

int create_sprite_obstacle(obstacle_t obstacle, int coord_x, int coord_y, int base_reg, int sprites_obstacle_status[], obstacle_t obstaculos_na_tela[]){
    sprite_t obstacle_sprite;
    int index, reg;
    
    base_reg = 20;
    index = check_for_empty_reg(sprites_obstacle_status);
    if(index == -1) return -1; //não há registrador vazio. Nao é possivel criar o sprite
    else{
        reg = base_reg + index;
        obstacle_sprite.coord_x = coord_x;
        obstacle_sprite.coord_y = coord_y;
        obstacle_sprite.offset = obstacle.offset;
        obstacle_sprite.speed = obstacle.speed;
        obstacle_sprite.data_register = reg;
        obstacle_sprite.visibility = 1;
        set_sprite(obstacle_sprite);
        sprites_obstacle_status[index] = 1;

        //identificando obstaculo ativo na tela
        obstaculos_na_tela[index].coord_x = coord_x;
        obstaculos_na_tela[index].coord_y = coord_y;
        obstaculos_na_tela[index].offset = obstacle.offset;
        obstaculos_na_tela[index].speed = obstacle.speed;
        obstaculos_na_tela[index].reward = obstacle.reward;
        obstaculos_na_tela[index].size = obstacle.size; 
        obstaculos_na_tela[index].on_frame = 1;     
    }
    
    return 1; //criado com sucesso
 
}

void initialize_obstacle_vector(obstacle_t vetor_obstaculos[]){
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
}

obstacle_t select_random_obstacle_from_vector(obstacle_t vetor_obstaculos[]){
    obstacle_t selected_obstacle; 
    int i;

    i = random_number(0, 16);
    if(vetor_obstaculos[i].offset == 11) i += 1;
    selected_obstacle.coord_x = 0;
    selected_obstacle.coord_y = 0;
    selected_obstacle.offset = vetor_obstaculos[i].offset;
    selected_obstacle.reward = vetor_obstaculos[i].reward;
    selected_obstacle.size = vetor_obstaculos[i].size;
    selected_obstacle.speed = vetor_obstaculos[i].speed;
    selected_obstacle.on_frame = 0;
    return selected_obstacle;   
}


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

void move_obstacles(obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[], int unidade, int reg_base_obstacles){
    sprite_t new_sprite;
    int deslocamento, new_coord_y;

    for (int i = 0; i < 10; i++)
    {
        if(sprites_obstacle_status[i] == 1){
            deslocamento = obstaculos_na_tela[i].speed * unidade;
            new_coord_y = obstaculos_na_tela[i].coord_y + deslocamento;
            if(obstaculos_na_tela[i].coord_y >= 480 || new_coord_y >= 471){
                obstaculos_na_tela[i].coord_x = 0;
                obstaculos_na_tela[i].coord_y = 0;
                obstaculos_na_tela[i].on_frame = 0;

                /* Cria um sprite invisivel */
                new_sprite.coord_x = 0;
                new_sprite.coord_y = 0;
                //new_sprite.offset = 0;
                new_sprite.visibility = 0;
                
                // Libera o registrador para uso
                sprites_obstacle_status[i] = 0;

            }else{
                obstaculos_na_tela[i].coord_y = new_coord_y;
                obstaculos_na_tela[i].on_frame = 1;

                /* Atualiza o sprite */
                new_sprite.coord_x = obstaculos_na_tela[i].coord_x;
                new_sprite.coord_y = new_coord_y;
                new_sprite.offset = obstaculos_na_tela[i].offset;
                new_sprite.speed = obstaculos_na_tela[i].speed;
                new_sprite.data_register = reg_base_obstacles + i; //20 + i
                new_sprite.visibility = 1;
            } 
            set_sprite(new_sprite);
        }       
    }    
}

int all_obstacles_invisibles(obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[]){
    sprite_t invisible_sprite;
    int reg_base_obstacles = 20;

    for (int i = 0; i < 10; i++)
    {   
        invisible_sprite.coord_x = obstaculos_na_tela[i].coord_x;
        invisible_sprite.coord_y = obstaculos_na_tela[i].coord_y;
        invisible_sprite.offset = obstaculos_na_tela[i].offset;
        invisible_sprite.speed = obstaculos_na_tela[i].speed;
        invisible_sprite.data_register = reg_base_obstacles + i; //20 + i
        invisible_sprite.visibility = 0;

        //preserva o obstáculo, porém com invisibilidade
        obstaculos_na_tela[i].on_frame = 1;
        sprites_obstacle_status[i] = 1;

        set_sprite(invisible_sprite);
    }
}

int restart_all_invisible_obstacles(obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[]){
    sprite_t visible_sprite;
    int reg_base_obstacles = 20;

    for (int i = 0; i < 10; i++)
    {   
        visible_sprite.coord_x = obstaculos_na_tela[i].coord_x;
        visible_sprite.coord_y = obstaculos_na_tela[i].coord_y;
        visible_sprite.offset = obstaculos_na_tela[i].offset;
        visible_sprite.speed = obstaculos_na_tela[i].speed;
        visible_sprite.data_register = reg_base_obstacles + i; //20 + i
        visible_sprite.visibility = 1;

        //preserva o obstáculo
        obstaculos_na_tela[i].on_frame = 1;
        sprites_obstacle_status[i] = 1;

        set_sprite(visible_sprite);
    }
}

int clean_all_obstacles(obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[]){
    sprite_t new_sprite;
    int reg_base = 20;
    
    for (int i = 0; i < 10; i++)
    {   
        /*Restart sprite*/
        new_sprite.coord_x = 0;
        new_sprite.coord_y = 0;
        new_sprite.offset = 0;
        new_sprite.speed = 0;
        new_sprite.data_register = reg_base + i; //20 + i
        new_sprite.visibility = 0;

        obstaculos_na_tela[i].on_frame = 0;
        sprites_obstacle_status[i] = 0;

        set_sprite(new_sprite);
    }
}



int random_obstacle(int cord_x_player, int cord_y_player, int limite_min_pista, int limite_max_pista, obstacle_t obstaculos_na_tela[], int sprites_obstacle_status[], obstacle_t vetor_obstaculos[]){
    obstacle_t new_obstacle;
    sprite_t new_sprite;

    
    int coord_x_obstacle, coord_y_obstacle;

    int base_reg_obstacles = 20, unidade = 1;
    //gera coordenada aleatória entre 
    coord_x_obstacle = random_number(range_min_coord_x(cord_x_player, limite_min_pista), range_max_coord_x(cord_x_player, limite_max_pista));
    coord_y_obstacle = 10; // 0 + 10

    //Executar antes do random_obstacle
    //initialize_obstacle_vector(vetor_obstaculos);
    new_obstacle = select_random_obstacle_from_vector(vetor_obstaculos);
    create_sprite_obstacle(new_obstacle,coord_x_obstacle, coord_y_obstacle, base_reg_obstacles, sprites_obstacle_status, obstaculos_na_tela);
    move_obstacles(obstaculos_na_tela, sprites_obstacle_status, unidade, base_reg_obstacles);

    return 1; // sucess
}

