#include "mouse_module.h"
#include "colision_module.h"
#include "../Lib/colenda.h"
#include "background_animation_module.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


#define PLAYER_SPEED_BASE 1     /*definição da velocidade base do jogador*/
#define BULLET_SPEED_BASE 15    /*definição da velocidade base do tiro*/

/*enumeração com os estados de jogo começando pelo 1*/
typedef enum {
    running = 1,
    in_pause,
    in_menu,
    finish,
    win,
    lose,
} states;

obstacle_t obstacles_model[13];
obstacle_t obstacle[10];    /*array de obstaculos*/
sprite_t scene[10];         /*array de objetos de cena*/
sprite_t sprite_bullets[10];    /*array com sprites de disparo*/
sprite_t player_sprite;         /*sprite do player*/

//mutex utilizados
pthread_mutex_t gpu_mutex;
pthread_mutex_t background_mutex;
pthread_mutex_t mouse_mutex;
pthread_mutex_t obstacle_mutex;
pthread_mutex_t bullets_mutex;
pthread_mutex_t player_invunerability_mutex;
pthread_mutex_t colision_mutex;

//condicionais utilizadas
pthread_cond_t mouse_cond;
pthread_cond_t obstacle_cond;
pthread_cond_t background_cond;
pthread_cond_t bullets_cond;
pthread_cond_t player_invulnerability_cond;
pthread_cond_t colision_cond;


states state; /*variavel do tipo states responsavel por guardar o estado atual do jogo*/

int pause_background, pause_obstacle, pause_mouse, pause_bullets, pause_colision;
int player_invunerability;
int obstaculos_gerados[10] = {0,0,0,0,0,0,0,0,0,0};
int score;
int bullets[10] = {0,0,0,0,0,0,0,0,0,0};
int key_press;


void pause_threads() {
    pthread_mutex_lock(&mouse_mutex);
    pause_mouse = 1;
    pthread_mutex_unlock(&mouse_mutex);
    pthread_mutex_lock(&bullets_mutex);
    pause_bullets = 1;
    pthread_mutex_unlock(&bullets_mutex);
    pthread_mutex_lock(&obstacle_mutex);
    pause_obstacle = 1;
    pthread_mutex_unlock(&obstacle_mutex);
    pthread_mutex_lock(&background_mutex);
    pause_background = 1;
    pthread_mutex_unlock(&background_mutex);
    pthread_mutex_lock(&colision_mutex);
    pause_colision = 1;
    pthread_mutex_unlock(&colision_mutex);
}

void reestart_threads() {
    pthread_mutex_lock(&mouse_mutex);
    pause_mouse = 0;
    pthread_cond_signal(&mouse_cond);
    pthread_mutex_unlock(&mouse_mutex);
    pthread_mutex_lock(&bullets_mutex);
    pause_bullets = 0;
    pthread_cond_signal(&bullets_cond);
    pthread_mutex_unlock(&bullets_mutex);
    pthread_mutex_lock(&obstacle_mutex);
    pause_obstacle = 0;
    pthread_cond_signal(&obstacle_cond);
    pthread_mutex_unlock(&obstacle_mutex);
    pthread_mutex_lock(&background_mutex);
    pause_background = 0;
    pthread_cond_signal(&background_cond);
    pthread_mutex_unlock(&background_mutex);
    pthread_mutex_lock(&colision_mutex);
    pause_colision = 0;
    pthread_cond_signal(&colision_cond);
    pthread_mutex_unlock(&colision_mutex);
}

void* player_invulnerability_timer(void* args) {
    int i = 0;
    while(1){
        pthread_mutex_lock(&player_invunerability_mutex);
        while(!player_invunerability){
            pthread_cond_wait(&player_invulnerability_cond, &player_invunerability_mutex);
        }
        pthread_mutex_unlock(&player_invunerability_mutex);

        
        pthread_mutex_lock(&mouse_mutex);
        pause_mouse = 1;
        pthread_mutex_unlock(&mouse_mutex);
        if (player_sprite.visibility) {
            player_sprite.visibility = 0;
        } else {
            player_sprite.visibility = 1;
        }

        pthread_mutex_lock(&gpu_mutex);
        set_sprite(player_sprite);
        pthread_mutex_unlock(&gpu_mutex);

        pthread_mutex_lock(&mouse_mutex);
        pause_mouse = 0;
        pthread_cond_signal(&mouse_cond);
        pthread_mutex_unlock(&mouse_mutex);
        if(i == 10) {
            pthread_mutex_lock(&player_invunerability_mutex);
            player_invunerability = 0;
            pthread_mutex_unlock(&player_invunerability_mutex);
            player_sprite.visibility = 1;
            pthread_mutex_lock(&gpu_mutex);
            set_sprite(player_sprite);
            pthread_mutex_unlock(&gpu_mutex);
            i = 0;
        }else {
            i++;
        }
        usleep(200000);

        

        
        
    }
    
}

void* bullet_routine(void* args) {
    while (1) {
        pthread_mutex_lock(&bullets_mutex);
        while (pause_bullets)
        {
            pthread_cond_wait(&bullets_cond, &bullets_mutex);
        }
        pthread_mutex_unlock(&bullets_mutex);

        for (int i = 0; i < 10; i++)
        {
            if(bullets[i] == 1) {
                pthread_mutex_lock(&mouse_mutex);
                pause_mouse = 1;
                pthread_mutex_unlock(&mouse_mutex);
                
                if (sprite_bullets[i].coord_y <= 15) {
                    bullets[i] = 0;
                    sprite_bullets[i].visibility = 0;

                } else {
                    sprite_bullets[i].coord_y -= BULLET_SPEED_BASE;
                }
                pthread_mutex_lock(&gpu_mutex);
                set_sprite(sprite_bullets[i]);
                pthread_mutex_unlock(&gpu_mutex);
                pthread_mutex_lock(&mouse_mutex);
                pause_mouse = 0;
                pthread_cond_signal(&mouse_cond);
                pthread_mutex_unlock(&mouse_mutex);
            } else {
                sprite_bullets[i].visibility = 0;
                pthread_mutex_lock(&gpu_mutex);
                set_sprite(sprite_bullets[i]);
                pthread_mutex_unlock(&gpu_mutex);
            }
        }
        usleep(100000);
    }
}

//esta rotina já esta completa, apenas basta ver o tempo de delay de alteração do fundo 
void* change_background_routine(void* args) {
    while (1) {
        pthread_mutex_lock(&background_mutex);
        while (pause_background)
        {
            pthread_cond_wait(&background_cond, &background_mutex);
        }
        pthread_mutex_unlock(&background_mutex);

        pthread_mutex_lock(&gpu_mutex);
        bg_animation(); //função do modulo que faz a atualização do fundo e da animação com o passar dos quadros
        pthread_mutex_unlock(&gpu_mutex);
        usleep(100000);
    }
}

void* mouse_polling_routine(void* args) {
    
    int value_x_mouse = 0, i, has_shot = 0;
    int car_speed, car_sprite;

    while (1) {
        pthread_mutex_lock(&mouse_mutex);
        while (pause_mouse)
        {
            pthread_cond_wait(&mouse_cond, &mouse_mutex);
        }
        pthread_mutex_unlock(&mouse_mutex);
        read_mouse_2_event(&key_press, &value_x_mouse);

        
        car_speed = PLAYER_SPEED_BASE * value_x_mouse;
        car_sprite = player_sprite.offset + value_x_mouse;

        //detecção de borda
        if ( car_speed < 0 && player_sprite.coord_x - 10 >= 103 && (player_sprite.coord_x - 10) + car_speed <= 103) {
            //pega o espaço restante que o carro ainda pode se mover antes de chegar na borda
            player_sprite.coord_x -= ((player_sprite.coord_x - 10 + car_speed) - 103); 
        } else if (car_speed > 0 && player_sprite.coord_x + 10 <= 296 && (player_sprite.coord_x + 10) + car_speed >= 296) {
            //pega o espaço restante que o carro ainda pode se mover antes de chegar na borda
            player_sprite.coord_x += (296 - (player_sprite.coord_x + 10 + car_speed));
        } else { 
            player_sprite.coord_x += car_speed;
        }

        

        if (key_press == 1 && !has_shot) {
            i = 0;
            pthread_mutex_lock(&bullets_mutex);
            pause_bullets = 1;
            pthread_mutex_unlock(&bullets_mutex);
            while(i < 10){
        
                if (bullets[i] == 0) {
                    bullets[i] = 1;
                    sprite_bullets[i].offset = LASER_VERTICAL;
                    sprite_bullets[i].coord_x = player_sprite.coord_x;
                    sprite_bullets[i].coord_y = player_sprite.coord_y - 20;
                    sprite_bullets[i].visibility = 1;
                    sprite_bullets[i].data_register = (i + 1);
                    sprite_bullets[i].speed = BULLET_SPEED_BASE;
                    has_shot = 1;
                    break;
                }
                
                i++;
            }
            pthread_mutex_lock(&bullets_mutex);
            pause_bullets = 0;
            pthread_cond_signal(&bullets_cond);
            pthread_mutex_unlock(&bullets_mutex);
        } else if (key_press){
            has_shot = 1;
        } else {
            has_shot = 0;
        }

        pthread_mutex_lock(&gpu_mutex);
        set_sprite(player_sprite);
        if(i<10) {
            set_sprite(sprite_bullets[i]);
        }
        pthread_mutex_unlock(&gpu_mutex);
        
    }
}

void* random_obstacle_generate_routine(void* args) {
    while (1)
    {
        pthread_mutex_lock(&obstacle_mutex);
        while (pause_obstacle)
        {
            pthread_cond_wait(&obstacle_cond, &obstacle_mutex);
        }
        pthread_mutex_unlock(&obstacle_mutex);

        pthread_mutex_lock(&gpu_mutex);
        random_obstacle(player_sprite.coord_x, player_sprite.coord_y, 103, 296, obstacle, obstacles_model, obstaculos_gerados);
        pthread_mutex_unlock(&gpu_mutex);
        usleep(100000);
    }
    
}


// loop principal do jogo
void* colision_routine(void* args){
    sprite_t invisible_obstacle = {.coord_x = 1, .coord_y = 1, .offset = 1, .speed = 0, .visibility = 0};
    
    while (1)
    {
        pthread_mutex_lock(&colision_mutex);
        while(pause_colision){
            pthread_cond_wait(&colision_cond, &colision_mutex);
        }
        pthread_mutex_unlock(&colision_mutex);

        for (int i = 0; i < 10; i++)
        {
            if(obstaculos_gerados[i]) {
                pthread_mutex_lock(&obstacle_mutex);
                pause_obstacle = 1;
                pthread_mutex_unlock(&obstacle_mutex);
                for (int j = 0; j < 10; j++)
                {
                    if (bullets[j]) {
                        pthread_mutex_lock(&bullets_mutex);
                        pause_bullets = 1;
                        pthread_mutex_unlock(&bullets_mutex);
                        if (check_colision_bullet(sprite_bullets[j], obstacle[i])) {
                            score += obstacle[i].reward;
                            sprite_bullets[j].visibility = 0;
                            bullets[j] = 0;
                            invisible_obstacle.data_register = (21 + i);
                            obstacle[i].on_frame = 0;
                            obstaculos_gerados[i] = 0;
                            pthread_mutex_lock(&gpu_mutex);
                            set_sprite(invisible_obstacle);
                            pthread_mutex_unlock(&gpu_mutex);
                        }
                        pthread_mutex_lock(&bullets_mutex);
                        pause_bullets = 0;
                        pthread_cond_signal(&bullets_cond);
                        pthread_mutex_unlock(&bullets_mutex);
                    }
                }
                pthread_mutex_lock(&obstacle_mutex);
                pause_obstacle = 0;
                pthread_cond_signal(&obstacle_cond);
                pthread_mutex_unlock(&obstacle_mutex);
            }
        }

        pthread_mutex_lock(&player_invunerability_mutex);
        if (!player_invunerability){
            for (int i = 0; i < 10; i++) {
                if(obstaculos_gerados[i]) {
                    pthread_mutex_lock(&obstacle_mutex);
                    pause_obstacle = 1;
                    pthread_mutex_unlock(&obstacle_mutex);

                    pthread_mutex_lock(&mouse_mutex);
                    pause_mouse = 1;
                    pthread_mutex_unlock(&mouse_mutex);

                    if(check_colision_player(player_sprite, obstacle[i])){
                        score -= obstacle[i].reward;
                        player_invunerability = 1;
                        obstacle[i].on_frame = 0;
                        invisible_obstacle.data_register = (21 + i);
                        obstaculos_gerados[i] = 0;
                        pthread_mutex_lock(&gpu_mutex);
                        set_sprite(invisible_obstacle);
                        pthread_mutex_unlock(&gpu_mutex);
                        
                        pthread_mutex_lock(&mouse_mutex);
                        pause_mouse = 0;
                        pthread_cond_signal(&mouse_cond);
                        pthread_mutex_unlock(&mouse_mutex);
                        pthread_cond_signal(&player_invulnerability_cond);
                        break;
                    }
                    pthread_mutex_lock(&mouse_mutex);
                    pause_mouse = 0;
                    pthread_cond_signal(&mouse_cond);
                    pthread_mutex_unlock(&mouse_mutex);
                }
            }
            pthread_mutex_lock(&obstacle_mutex);
            pause_obstacle = 0;
            pthread_cond_signal(&obstacle_cond);
            pthread_mutex_unlock(&obstacle_mutex);
        }
        pthread_mutex_unlock(&player_invunerability_mutex);
        
        printf("score: %d\n", score);
        if(score < 0) {
            printf("jogador perdeu\n");
            lose_screen();
        } else if (score >= 1000) {
            printf("jogador ganhou\n");
            win_screen();
        }
        

    }
}



void menu() {
    state = in_menu;
    int btn_val;
    if(pause_background || pause_bullets || pause_colision || pause_mouse || pause_obstacle) {
        pause_threads();
    }
    while(1) {
        scanf("%d", &btn_val);

        if(btn_val == 1 && state == in_menu) {
            state = running;
            init_game();
        } else if (btn_val == 2 && state == running) {
            state = in_pause;
            pause_screen();
        } else if (btn_val == 2 && state == in_pause) {
            state = running;
            return_screen();
        } else if (btn_val == 3) {
            state = in_menu;
            menu();
        } else if (btn_val == 4) {
            state = finish;
            end_game();
        } 

    }
}


void pause_screen() {
    pause_threads();
    //TODO: tela de pausa
}

void return_screen() {
    reestart_threads();
    //TODO: tela falando que foi despausado
    state = running;
}

void win_screen() {
    pause_threads();
    //TODO: tela de vitoria
    state = in_menu;
    menu();
}

void lose_screen() {
    pause_threads();
    //TODO: tela de perder o jogo
    state = in_menu;
    menu();
}

void init_game() {
    // bullets = {0,0,0,0,0,0,0,0,0,0};
    // obstacle = {0,0,0,0,0,0,0,0,0,0};
    clear();
    module_init_mouse_2();
    bg_animation_module_init();
    initialize_obstacle_vector(obstacles_model);
    reestart_threads();
    //TODO: tela de iniciar o jogo
    set_sprite(player_sprite);
}

void end_game() {
    pause_threads();

    //TODO: tela de finalizar o jogo
    exit(0);
}

int main() {


    //inicializacao dos inteiros
    score = 0;
    pause_background = 1;
    pause_mouse = 1;
    pause_obstacle =1;
    player_invunerability = 0;
    pause_bullets = 0;
    pause_colision = 1;
    key_press = 0;

    pthread_t obstacle_thread, background_thread, mouse_thread, player_timer_thread, bullets_thread, colision_thread; 
    GPU_open();
    clear();

    player_sprite.coord_x = 120;
    player_sprite.coord_y = 340;
    player_sprite.data_register = 31;
    player_sprite.offset = BLUE_CAR;
    player_sprite.speed = 0;
    player_sprite.visibility = 1;

    //inicialização dos mutex
    pthread_mutex_init(&gpu_mutex, NULL);
    pthread_mutex_init(&background_mutex, NULL);
    pthread_mutex_init(&mouse_mutex, NULL);
    pthread_mutex_init(&obstacle_mutex, NULL);
    pthread_mutex_init(&player_invunerability_mutex, NULL);
    pthread_mutex_init(&bullets_mutex, NULL);
    pthread_mutex_init(&colision_mutex, NULL);

    //inicialização das condições
    pthread_cond_init(&mouse_cond, NULL);
    pthread_cond_init(&obstacle_cond, NULL);
    pthread_cond_init(&background_cond, NULL);
    pthread_cond_init(&player_invulnerability_cond, NULL);
    pthread_cond_init(&bullets_cond, NULL);
    pthread_cond_init(&colision_cond, NULL);

    //inicialização das threads
    pthread_create(&obstacle_thread, NULL, random_obstacle_generate_routine, NULL);
    pthread_create(&background_thread, NULL, change_background_routine, NULL);
    pthread_create(&mouse_thread, NULL, mouse_polling_routine, NULL);
    pthread_create(&player_timer_thread, NULL, player_invulnerability_timer, NULL);
    pthread_create(&bullets_thread, NULL, bullet_routine, NULL);
    pthread_create(&colision_thread, NULL, colision_routine, NULL);

    //loop principal do jogo
    menu();
    

    //finalizando as threads
    pthread_cancel(obstacle_thread);
    pthread_cancel(background_thread);
    pthread_cancel(mouse_thread);
    pthread_cancel(player_timer_thread);
    pthread_cancel(bullets_thread);
    pthread_cancel(colision_thread);

    pthread_join(obstacle_thread, NULL);
    pthread_join(background_thread, NULL);
    pthread_join(mouse_thread, NULL);
    pthread_join(player_timer_thread, NULL);
    pthread_join(bullets_thread, NULL);
    pthread_join(colision_thread, NULL);


    //encerrando os mutex
    pthread_mutex_destroy(&gpu_mutex);
    pthread_mutex_destroy(&background_mutex);
    pthread_mutex_destroy(&mouse_mutex);
    pthread_mutex_destroy(&obstacle_mutex);
    pthread_mutex_destroy(&player_invunerability_mutex);
    pthread_mutex_destroy(&bullets_mutex);
    pthread_mutex_destroy(&colision_mutex);

    //encerrando as condicionais
    pthread_cond_destroy(&mouse_cond);
    pthread_cond_destroy(&obstacle_cond);
    pthread_cond_destroy(&background_cond);
    pthread_cond_destroy(&player_invulnerability_cond);
    pthread_cond_destroy(&bullets_cond);
    pthread_cond_destroy(&colision_cond);


    GPU_close();
    return 0;
}