#include "mouse_module.h"
#include "colision_module.h"
#include "../Lib/colenda.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


#define PLAYER_SPEED_BASE 10

sprite_t obstaculos_sprite[10];
sprite_t cenario[10];
sprite_t player_sprite;

background_block_movel_t obstaculos_block[10];


pthread_mutex_t gpu_mutex;
pthread_mutex_t background_mutex;
pthread_mutex_t mouse_mutex;
pthread_mutex_t obstaculos_mutex;
pthread_cond_t mouse_cond;
pthread_cond_t obstacle_cond;
pthread_cond_t background_cond;


int pause_background, pause_obstacle, pause_mouse;

int obstaculos_gerados;
int score;
int bullets;
int car_sprite, car_speed, key_press;


void* change_background_routine(void* args) {
    while (1) {
        pthread_mutex_lock(&background_mutex);
        while (pause_background)
        {
            pthread_cond_wait(&background_cond, &background_mutex);
        }
        pthread_mutex_unlock(&background_mutex);

        pthread_mutex_lock(&gpu_mutex);
        //chamar a função que atualiza o fundo
        pthread_mutex_unlock(&gpu_mutex);
    }
}

void* mouse_polling_routine(void* args) {
    int value_x_mouse;
    while (1) {
        pthread_mutex_lock(&mouse_mutex);
        while (pause_mouse)
        {
            pthread_cond_wait(&mouse_cond, &mouse_mutex);
        }
        pthread_mutex_unlock(&mouse_mutex);
        read_mouse_event(&key_press, &value_x_mouse);

        car_speed = PLAYER_SPEED_BASE * value_x_mouse;
        car_sprite = player_sprite.offset + value_x_mouse;
        
    }
    
}

void* random_obstacle_generate_routine(void* args) {
    while (1)
    {
        pthread_mutex_lock(&obstaculos_mutex);
        while (pause_obstacle)
        {
            pthread_cond_wait(&obstacle_cond, &obstaculos_mutex);
        }
        pthread_mutex_unlock(&obstaculos_mutex);

        pthread_mutex_lock(&gpu_mutex);
        //chamar a geração de obstaculos

        pthread_mutex_unlock(&gpu_mutex);
    }
    
}


// loop principal do jogo
void game_main_loop(){

    int has_colision = 0, colision_check_enable = 1;

    while (1)
    {
        if (colision_check_enable) {
            for (size_t i = 0; i < 10; i++)
            {
                if (check_colision_player_sprite(player_sprite, obstaculos_sprite[i])) {
                    printf("colisão com sprite\n");
                    score -= ((obstaculos_sprite[i]).offset*10);
                    has_colision = 1;
                    colision_check_enable = 0;
                    break;
                } 

                if (check_colision_player_block(player_sprite, obstaculos_block[i])) {
                    printf("colisão com block\n");
                    score -= (score%10)*20;
                    has_colision = 1;
                    colision_check_enable = 0;
                    break;
                }
            }

            if(score < 0){
                printf("jogo finalizado\n");

                pthread_mutex_lock(&background_mutex);
                pause_background = 1;
                pthread_mutex_unlock(&background_mutex);
                pthread_mutex_lock(&mouse_mutex);
                pause_mouse = 1;
                pthread_mutex_unlock(&mouse_mutex);
                pthread_mutex_lock(&obstaculos_mutex);
                pause_obstaculos = 1;
                pthread_mutex_unlock(&obstaculos_mutex);
                score = 0;

                usleep(50000);
            }
        }

        //se houver uma colisão desabilitar a checagem de colisão por um tempo
        if(has_colision) {
            //fazer o carro piscar por um tempo
        } else {
            pthread_mutex_lock(&background_mutex);
            pause_background = 1;
            pthread_mutex_unlock(&background_mutex);
            pthread_mutex_lock(&mouse_mutex);
            pause_mouse = 1;
            pthread_mutex_unlock(&mouse_mutex);
            pthread_mutex_lock(&obstaculos_mutex);
            pause_obstaculos = 1;
            pthread_mutex_unlock(&obstaculos_mutex);

            
            player_sprite.speed = car_speed;
            player_sprite.coord_x += player_sprite.speed;

            //fazer deteccao de borda da pista

            player_sprite.offset = car_sprite;
            pthread_mutex_lock(&gpu_mutex);
            set_sprite(player_sprite);
            pthread_mutex_unlock(&gpu_mutex);

            pthread_mutex_lock(&background_mutex);
            pause_background = 0;
            pthread_cond_signal(&background_cond);
            pthread_mutex_unlock(&background_mutex);
            pthread_mutex_lock(&mouse_mutex);
            pause_mouse = 0;
            pthread_cond_signal(&mouse_cond);
            pthread_mutex_unlock(&mouse_mutex);
            pthread_mutex_lock(&obstaculos_mutex);
            pause_obstaculos = 0;
            pthread_cond_signal(&obstacle_cond);
            pthread_mutex_unlock(&obstaculos_mutex);
        }

    }
}

//personagem se tor invuneravel por algum tempo apos uma colisão para permitir que o jogo continue
//ao fim da atualização da tela trocar posição do player
int main() {

    //inicializacao dos inteiros
    obstaculos_gerados = 0;
    score = 0;
    bullets = 10;
    pause_background = 0;
    pause_mouse = 0;
    pause_obstacle = 0;

    pthread_t obstacle_thread, background_thread, mouse_thread; 
    GPU_open();

    player_sprite.coord_x = 320;
    player_sprite.coord_y = 340;
    player_sprite.data_register = 31;
    player_sprite.offset = 27;
    player_sprite.speed = 0;
    player_sprite.visibility = 1;

    //inicialização dos mutex
    pthread_mutex_init(&gpu_mutex, NULL);
    pthread_mutex_init(&background_mutex, NULL);
    pthread_mutex_init(&mouse_mutex, NULL);
    pthread_mutex_init(&obstaculos_mutex, NULL);

    //inicialização das condições
    pthread_cond_init(&mouse_cond, NULL);
    pthread_cond_init(&obstacle_cond, NULL);
    pthread_cond_init(&background_cond, NULL);

    //inicialização das threads
    pthread_create(&obstacle_thread, NULL, random_obstacle_generate_routine, NULL);
    pthread_create(&background_thread, NULL, change_background_routine, NULL);
    pthread_create(&mouse_thread, NULL, mouse_polling_routine, NULL);

    //loop principal do jogo
    game_main_loop();
    

    //finalizando as threads
    pthread_cancel(obstacle_thread);
    pthread_cancel(background_thread);
    pthread_cancel(mouse_thread);

    pthread_join(obstacle_thread, NULL);
    pthread_join(background_thread, NULL);
    pthread_join(mouse_thread, NULL);


    //encerrando os mutex
    pthread_mutex_destroy(&gpu_mutex);
    pthread_mutex_destroy(&background_mutex);
    pthread_mutex_destroy(&mouse_mutex);
    pthread_mutex_destroy(&obstaculos_mutex);

    //encerrando as condicionais
    pthread_cond_destroy(&mouse_cond);
    pthread_cond_destroy(&obstacle_cond);
    pthread_cond_destroy(&background_cond);


    GPU_close();
    return 0;
}