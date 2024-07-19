#include "mouse_module.h"
#include "colision_module.h"
#include "../Lib/colenda.h"
#include "background_animation_module.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define PLAYER_SPEED_BASE 2
#define BULLET_SPRITE 2
#define BULLET_SPEED_BASE 15

typedef enum {
    running = 1,
    in_pause,
    in_menu,
    finish,
    win,
    lose,
} states;
typedef enum {
    single_player = 1,
    dual_player,
} game_modes;
//do registrador 1 ao 10 são as balas, do 11 ao 20 são cenarios, do 21 ao 30 são obstaculos e o 31
obstacle_t obs[13];
obstacle_t screen_obs[10];
sprite_t cenario[9];
sprite_t sprite_bullets[10];
sprite_t player_1_sprite;
sprite_t player_2_sprite;
//mutex utilizados
pthread_mutex_t gpu_mutex;
pthread_mutex_t background_mutex;
pthread_mutex_t mouse_1_mutex;
pthread_mutex_t mouse_2_mutex;
pthread_mutex_t obstacle_mutex;
pthread_mutex_t bullets_mutex;
pthread_mutex_t player_1_invunerability_mutex;
pthread_mutex_t player_2_invunerability_mutex;
pthread_mutex_t colision_mutex;
//condicionais utilizadas
pthread_cond_t mouse_1_cond;
pthread_cond_t mouse_2_cond;
pthread_cond_t obstacle_cond;
pthread_cond_t background_cond;
pthread_cond_t bullets_cond;
pthread_cond_t player_1_invulnerability_cond;
pthread_cond_t player_2_invulnerability_cond;
pthread_cond_t colision_cond;
//threads
pthread_t obstacle_thread, background_thread, mouse_1_thread, mouse_2_thread, player_1_timer_thread, player_2_timer_thread, bullets_thread, colision_thread; 

states state;
game_modes game_mode;
int pause_background, pause_obstacle, pause_mouse_1, pause_mouse_2, pause_bullets, pause_colision;
int player_1_invunerability;
int player_2_invunerability;
int obstaculos_gerados[10] = {0,0,0,0,0,0,0,0,0,0};
int score_1;
int score_2;
int bullets[10] = {0,0,0,0,0,0,0,0,0,0};
int key_press_1;
int key_press_2;

void pause_threads() {
    
    pthread_mutex_lock(&mouse_2_mutex);
    pause_mouse_2 = 1;
    pthread_mutex_unlock(&mouse_2_mutex);
    pthread_mutex_lock(&mouse_1_mutex);
    pause_mouse_1 = 1;
    pthread_mutex_unlock(&mouse_1_mutex);
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
    
    if(game_mode == dual_player) {
        pthread_mutex_lock(&mouse_2_mutex);
        pause_mouse_2 = 0;
        pthread_cond_signal(&mouse_2_cond);
        pthread_mutex_unlock(&mouse_2_mutex);
    }
    pthread_mutex_lock(&mouse_1_mutex);
    pause_mouse_1 = 0;
    pthread_cond_signal(&mouse_1_cond);
    pthread_mutex_unlock(&mouse_1_mutex);
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

void* player_1_invulnerability_timer(void* args) {
    int i = 0;
    while(1){
        pthread_mutex_lock(&player_1_invunerability_mutex);
        while(!player_1_invunerability || state == in_pause || state == in_menu){
            pthread_cond_wait(&player_1_invulnerability_cond, &player_1_invunerability_mutex);
        }
        pthread_mutex_unlock(&player_1_invunerability_mutex);

        pthread_mutex_lock(&mouse_1_mutex);
        pause_mouse_1 = 1;
        pthread_mutex_unlock(&mouse_1_mutex);
        
        if(player_1_sprite.visibility) {
            player_1_sprite.visibility = 0;
        } else {
            player_1_sprite.visibility = 1;
        }
        pthread_mutex_lock(&gpu_mutex);
        set_sprite(player_1_sprite);
        pthread_mutex_unlock(&gpu_mutex);
        pthread_mutex_lock(&mouse_1_mutex);
        pause_mouse_1 = 0;
        pthread_cond_signal(&mouse_1_cond);
        pthread_mutex_unlock(&mouse_1_mutex);
        
        if (i == 10) {
            pthread_mutex_lock(&player_1_invunerability_mutex);
            player_1_invunerability = 0;
            pthread_mutex_unlock(&player_1_invunerability_mutex);
            i=0;
            player_1_sprite.visibility = 1;

            pthread_mutex_lock(&gpu_mutex);
            set_sprite(player_1_sprite);
            pthread_mutex_unlock(&gpu_mutex);
        }else {
            i++;
        }
        usleep(200000);
    }
}

void* player_2_invulnerability_timer(void* args) {
    int i = 0;
    while(1){
        pthread_mutex_lock(&player_2_invunerability_mutex);
        while(!player_2_invunerability || state == in_pause || state == in_menu){
            pthread_cond_wait(&player_2_invulnerability_cond, &player_2_invunerability_mutex);
        }
        pthread_mutex_unlock(&player_2_invunerability_mutex);
        pthread_mutex_lock(&mouse_2_mutex);
        pause_mouse_2 = 1;
        pthread_mutex_unlock(&mouse_2_mutex);
        if (player_2_sprite.visibility) {
            player_2_sprite.visibility = 0;
        }
        else {
            player_2_sprite.visibility = 1;
        }

        pthread_mutex_lock(&gpu_mutex);
        set_sprite(player_2_sprite);
        pthread_mutex_unlock(&gpu_mutex);

        pthread_mutex_lock(&mouse_2_mutex);
        pause_mouse_2 = 0;
        pthread_cond_signal(&mouse_2_cond);
        pthread_mutex_unlock(&mouse_2_mutex);

        if(i == 10) {
            pthread_mutex_lock(&player_2_invunerability_mutex);
            player_2_invunerability = 0;
            pthread_mutex_unlock(&player_2_invunerability_mutex);
            i = 0;
            player_2_sprite.visibility = 1;
            pthread_mutex_lock(&gpu_mutex);
            set_sprite(player_2_sprite);
            pthread_mutex_unlock(&gpu_mutex);
        } else {
            i++;
        }
        usleep(200000);
    }
}

void* bullet_routine(void* args) {
    while (1) {
        pthread_mutex_lock(&bullets_mutex);
        while (pause_bullets || state == in_pause || state == in_menu)
        {
            pthread_cond_wait(&bullets_cond, &bullets_mutex);
        }
        pthread_mutex_unlock(&bullets_mutex);
        if (game_mode == single_player) {

            for(int i = 0; i < 10; i++) {
                if(bullets[i] == 1) {
                    pthread_mutex_lock(&mouse_1_mutex);
                    pause_mouse_1 = 1;
                    pthread_mutex_unlock(&mouse_1_mutex);
                    
                    if (sprite_bullets[i].coord_y <= 15) {
                        bullets[i] = 0;
                        sprite_bullets[i].visibility = 0;
                    } else {
                        sprite_bullets[i].coord_y -= BULLET_SPEED_BASE;
                    }
                    pthread_mutex_lock(&mouse_1_mutex);
                    pause_mouse_1 = 0;
                    pthread_cond_signal(&mouse_1_cond);
                    pthread_mutex_unlock(&mouse_1_mutex);
                    pthread_mutex_lock(&gpu_mutex);
                    set_sprite(sprite_bullets[i]);
                    pthread_mutex_unlock(&gpu_mutex);
                }
            }

        } else {
            for (int i = 0; i < 10; i++)
            {
                if (i < 5) {
                    if(bullets[i] == 1) {
                        pthread_mutex_lock(&mouse_1_mutex);
                        pause_mouse_1 = 1;
                        pthread_mutex_unlock(&mouse_1_mutex);
                        
                        if (sprite_bullets[i].coord_y <= 15) {
                            bullets[i] = 0;
                            sprite_bullets[i].visibility = 0;
                        } else {
                            sprite_bullets[i].coord_y -= BULLET_SPEED_BASE;
                        }
                        pthread_mutex_lock(&mouse_1_mutex);
                        pause_mouse_1 = 0;
                        pthread_cond_signal(&mouse_1_cond);
                        pthread_mutex_unlock(&mouse_1_mutex);
                        pthread_mutex_lock(&gpu_mutex);
                        set_sprite(sprite_bullets[i]);
                        pthread_mutex_unlock(&gpu_mutex);
                    }
                } else {
                    if(bullets[i] == 1) {
                        pthread_mutex_lock(&mouse_2_mutex);
                        pause_mouse_2 = 1;
                        pthread_mutex_unlock(&mouse_2_mutex);
                        
                        if (sprite_bullets[i].coord_y <= 15) {
                            bullets[i] = 0;
                            sprite_bullets[i].visibility = 0;
                        } else {
                            sprite_bullets[i].coord_y -= BULLET_SPEED_BASE;
                        }
                        pthread_mutex_lock(&mouse_2_mutex);
                        pause_mouse_2 = 0;
                        pthread_cond_signal(&mouse_2_cond);
                        pthread_mutex_unlock(&mouse_2_mutex);
                        pthread_mutex_lock(&gpu_mutex);
                        set_sprite(sprite_bullets[i]);
                        pthread_mutex_unlock(&gpu_mutex);
                    }
                }
            }
        }
        usleep(100000);
    }
}

//esta rotina já esta completa, apenas basta ver o tempo de delay de alteração do fundo 
void* change_background_routine(void* args) {
    while (1) {
        pthread_mutex_lock(&background_mutex);
        while (pause_background || state == in_pause || state == in_menu)
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

void* mouse_1_polling_routine(void* args) {
    
    int value_x_mouse = 0, i, has_shot = 0;
    int car_speed, car_sprite;

    while (1) {
        pthread_mutex_lock(&mouse_1_mutex);
        while (pause_mouse_1 || state == in_pause || state == in_menu)
        {
            pthread_cond_wait(&mouse_1_cond, &mouse_1_mutex);
        }
        pthread_mutex_unlock(&mouse_1_mutex);
        read_mouse_1_event(&key_press_1, &value_x_mouse);

        car_speed = PLAYER_SPEED_BASE * value_x_mouse;
        car_sprite = player_1_sprite.offset + value_x_mouse;
        //detecção de borda
        if ( car_speed < 0 && player_1_sprite.coord_x - 10 >= 96 && (player_1_sprite.coord_x - 10) + car_speed <= 96) {
            //pega o espaço restante que o carro ainda pode se mover antes de chegar na borda
            player_1_sprite.coord_x -= ((player_1_sprite.coord_x - 10 + car_speed) - 96); 
        } else if (car_speed > 0 && player_1_sprite.coord_x + 10 <= 289 && (player_1_sprite.coord_x + 10) + car_speed >= 289) {
            //pega o espaço restante que o carro ainda pode se mover antes de chegar na borda
            player_1_sprite.coord_x += (289 - (player_1_sprite.coord_x + 10 + car_speed));
        } else { 
            player_1_sprite.coord_x += car_speed;
        }

        if (key_press_1 == 1 && !has_shot) {
            i = 0;
            pthread_mutex_lock(&bullets_mutex);
            pause_bullets = 1;
            pthread_mutex_unlock(&bullets_mutex);
            if (game_mode == single_player) {
                while (i < 10) {
                    if (bullets[i] == 0) {
                        bullets[i] = 1;
                        sprite_bullets[i].offset = LASER_VERTICAL;
                        sprite_bullets[i].coord_x = player_1_sprite.coord_x;
                        sprite_bullets[i].coord_y = player_1_sprite.coord_y - 20;
                        sprite_bullets[i].visibility = 1;
                        sprite_bullets[i].data_register = (i + 1);
                        sprite_bullets[i].speed = BULLET_SPEED_BASE;
                        has_shot = 1;
                        break;
                    }
                    i++;
                }
            } else {
                while(i < 5){
                    if (bullets[i] == 0) {
                        bullets[i] = 1;
                        sprite_bullets[i].offset = LASER_VERTICAL;
                        sprite_bullets[i].coord_x = player_1_sprite.coord_x;
                        sprite_bullets[i].coord_y = player_1_sprite.coord_y - 20;
                        sprite_bullets[i].visibility = 1;
                        sprite_bullets[i].data_register = (i + 1);
                        sprite_bullets[i].speed = BULLET_SPEED_BASE;
                        has_shot = 1;
                        break;
                    }
                    i++;
                }
            }
            
            pthread_mutex_lock(&bullets_mutex);
            pause_bullets = 0;
            pthread_cond_signal(&bullets_cond);
            pthread_mutex_unlock(&bullets_mutex);
        } else if (key_press_1){
            has_shot = 1;
        } else {
            has_shot = 0;
        }

        pthread_mutex_lock(&gpu_mutex);
        set_sprite(player_1_sprite);
        if((i < 5 && game_mode == dual_player) || (i < 10 && game_mode == single_player)) {
            set_sprite(sprite_bullets[i]);
        }
        pthread_mutex_unlock(&gpu_mutex);
    }
}

void* mouse_2_polling_routine(void* args) {
    
    int value_x_mouse = 0, i, has_shot = 0;
    int car_speed, car_sprite;

    while (1) {
        pthread_mutex_lock(&mouse_2_mutex);
        while (pause_mouse_2 || state == in_pause || state == in_menu)
        {
            pthread_cond_wait(&mouse_2_cond, &mouse_2_mutex);
        }
        pthread_mutex_unlock(&mouse_2_mutex);
        read_mouse_2_event(&key_press_2, &value_x_mouse);
        car_speed = PLAYER_SPEED_BASE * value_x_mouse;
        car_sprite = player_2_sprite.offset + value_x_mouse;

        //detecção de borda
        if ( car_speed < 0 && player_2_sprite.coord_x - 10 >= 336 && (player_2_sprite.coord_x - 10) + car_speed <= 336) {
            //pega o espaço restante que o carro ainda pode se mover antes de chegar na borda
            player_2_sprite.coord_x -= ((player_2_sprite.coord_x - 10 + car_speed) - 336); 
        } else if (car_speed > 0 && player_2_sprite.coord_x + 10 <= 528 && (player_2_sprite.coord_x + 10) + car_speed >= 528) {
            //pega o espaço restante que o carro ainda pode se mover antes de chegar na borda
            player_2_sprite.coord_x += (528 - (player_2_sprite.coord_x + 10 + car_speed));
        } else { 
            player_2_sprite.coord_x += car_speed;
        }

        if (key_press_2 == 1 && !has_shot) {
            i = 5;
            pthread_mutex_lock(&bullets_mutex);
            pause_bullets = 1;
            pthread_mutex_unlock(&bullets_mutex);
            while(i < 10){
        
                if (bullets[i] == 0) {
                    bullets[i] = 1;
                    sprite_bullets[i].offset = LASER_VERTICAL;
                    sprite_bullets[i].coord_x = player_2_sprite.coord_x;
                    sprite_bullets[i].coord_y = player_2_sprite.coord_y - 20;
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
        } else if (key_press_2){
            has_shot = 1;
        } else {
            has_shot = 0;
        }

        pthread_mutex_lock(&gpu_mutex);
        set_sprite(player_2_sprite);
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
        while (pause_obstacle || state == in_pause || state == in_menu)
        {
            printf("ta pausado obstacle1\n");
            pthread_cond_wait(&obstacle_cond, &obstacle_mutex);
        }
        pthread_mutex_unlock(&obstacle_mutex);

        pthread_mutex_lock(&gpu_mutex);
        if(game_mode == dual_player) {
            random_obstacle(player_2_sprite.coord_x, player_2_sprite.coord_y, 336, 528, screen_obs, obs, obstaculos_gerados);
        }
        random_obstacle(player_1_sprite.coord_x, player_1_sprite.coord_y, 96, 289, screen_obs, obs, obstaculos_gerados);
        pthread_mutex_unlock(&gpu_mutex);
        usleep(100000);
    }
}

// loop principal do jogo
void* colision_routine(void* args){

    int points = 0;
    sprite_t invisible_sprite = {.coord_x = 1, .coord_y = 1, .offset = 1, .speed = 0, .visibility = 0};
    while (1)
    {
        pthread_mutex_lock(&colision_mutex);
        while(pause_colision || state == in_pause || state == in_menu){
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
                        if (check_colision_bullet(sprite_bullets[j], screen_obs[i])) {
                            points = screen_obs[i].reward;
                            sprite_bullets[j].visibility = 0;
                            bullets[j] = 0;
                            obstaculos_gerados[i] = 0;
                            screen_obs[i].on_frame = 0;
                            invisible_sprite.data_register = 21 + i;
                            pthread_mutex_lock(&gpu_mutex);
                            set_sprite(invisible_sprite);
                            pthread_mutex_unlock(&gpu_mutex);
                        } else {
                            points = 0;
                        }
                        pthread_mutex_lock(&bullets_mutex);
                        pause_bullets = 0;
                        pthread_cond_signal(&bullets_cond);
                        pthread_mutex_unlock(&bullets_mutex);
                        if (game_mode == single_player || (game_mode == dual_player && j < 5)){
                            score_1 += points;
                        } else {
                            score_2 += points;
                        }
                    }
                }
                pthread_mutex_lock(&obstacle_mutex);
                pause_obstacle = 0;
                pthread_cond_signal(&obstacle_cond);
                pthread_mutex_unlock(&obstacle_mutex);
            }
        }

        pthread_mutex_lock(&player_1_invunerability_mutex);
        if (!player_1_invunerability){
            for (int i = 0; i < 10; i++) {
                if(obstaculos_gerados[i]) {
                    pthread_mutex_lock(&obstacle_mutex);
                    pause_obstacle = 1;
                    pthread_mutex_unlock(&obstacle_mutex);

                    pthread_mutex_lock(&mouse_1_mutex);
                    pause_mouse_1 = 1;
                    pthread_mutex_unlock(&mouse_1_mutex);

                    if(check_colision_player(player_1_sprite, screen_obs[i])){
                        score_1 -= screen_obs[i].reward;
                        player_1_invunerability = 1;
                        obstaculos_gerados[i] = 0;
                        screen_obs[i].on_frame = 0;
                        invisible_sprite.data_register = 21 + i;
                        pthread_mutex_lock(&gpu_mutex);
                        set_sprite(invisible_sprite);
                        pthread_mutex_unlock(&gpu_mutex);
                        pthread_cond_signal(&player_1_invulnerability_cond);
                        
                    }
                    pthread_mutex_lock(&mouse_1_mutex);
                    pause_mouse_1 = 0;
                    pthread_cond_signal(&mouse_1_cond);
                    pthread_mutex_unlock(&mouse_1_mutex);
                    pthread_mutex_lock(&obstacle_mutex);
                    pause_obstacle = 0;
                    pthread_cond_signal(&obstacle_cond);
                    pthread_mutex_unlock(&obstacle_mutex);
                    if(player_1_invunerability) {
                        break;
                    }
                }
            }
        }
        pthread_mutex_unlock(&player_1_invunerability_mutex);
        
        pthread_mutex_lock(&player_2_invunerability_mutex);
        if (!player_2_invunerability && game_mode == dual_player){
            for (int i = 0; i < 10; i++) {
                if(obstaculos_gerados[i]) {
                    pthread_mutex_lock(&obstacle_mutex);
                    pause_obstacle = 1;
                    pthread_mutex_unlock(&obstacle_mutex);
                    pthread_mutex_lock(&mouse_2_mutex);
                    pause_mouse_2 = 1;
                    pthread_mutex_unlock(&mouse_2_mutex);

                    if(check_colision_player(player_2_sprite, screen_obs[i])) {
                        score_2 -= screen_obs[i].reward;
                        player_2_invunerability = 1;
                        obstaculos_gerados[i] = 0;
                        screen_obs[i].on_frame = 0;
                        invisible_sprite.data_register = 21 + i;
                        pthread_mutex_lock(&gpu_mutex);
                        set_sprite(invisible_sprite);
                        pthread_mutex_unlock(&gpu_mutex);
                        pthread_cond_signal(&player_2_invulnerability_cond);
                    }
                    pthread_mutex_lock(&mouse_2_mutex);
                    pause_mouse_2 = 0;
                    pthread_cond_signal(&mouse_2_cond);
                    pthread_mutex_unlock(&mouse_2_mutex);
                    pthread_mutex_lock(&obstacle_mutex);
                    pause_obstacle = 0;
                    pthread_cond_signal(&obstacle_cond);
                    pthread_mutex_unlock(&obstacle_mutex);
                    if (player_2_invunerability) {
                        break;
                    }
                }
            }
        }
        pthread_mutex_unlock(&player_2_invunerability_mutex);

        if(game_mode == single_player) {
            printf("score: %d\n", score_1);
        } else  {
            printf("score 1: %d\tscore 2: %d\n", score_1, score_2);
        }
        
        if(score_1 < 0) {
            printf("jogador 1 perdeu\n");
            lose_screen(); // colocar o jogar que perdeu
        } else if (score_2 < 0) {
            printf("jogador 2 perdeu\n");
            lose_screen(); // colocar o jogador que perdeu
        } else if (score_1 >= 1000 && score_2 >= 1000) {
            printf("empate\n");
            win_screen(); //mudar pra tela de empate
        } else if (score_1 >= 1000) {
            printf("jogador 1 venceu\n");
            win_screen(); //adicionar a opção do jogador que venceu
        } else if (score_2 >= 1000) {
            printf("jogador 2 venceu\n");
            win_screen(); //adicionar a opção do jogador que venceu
        }
    }
}

void pause_screen() {
    pause_threads();
    module_exit_mouse_1();
    module_exit_mouse_2();
    //TODO: tela de pausa
}

void return_screen() {
    module_init_mouse_1();
    module_init_mouse_2();
    reestart_threads();
    //TODO: tela falando que foi despausado
    state = running;
}

void win_screen() {
    pause_threads();
    //TODO: tela de vitoria
    state = in_menu;
    module_exit_mouse_1();
    module_exit_mouse_2();
    menu();
}


void lose_screen() {
    pause_threads();
    while(player_1_invunerability || player_2_invunerability){
    }
    clear();

    player_1_invunerability = 0;
    player_2_invunerability = 0;
    //TODO: tela de perder o jogo
    state = in_menu;
    module_exit_mouse_1();
    module_exit_mouse_2();
    menu();
}

void init_game() {
    sprite_t invisible_obstacle = {.coord_x = 1, .coord_y = 1, .visibility = 0, .offset = 0, .speed = 0};
    clear();
    score_1 = 0;
    score_2 = 0;
    for(int i = 0; i < 10; i++){
        if (bullets[i]) {
            sprite_bullets[i].visibility = 0;
            bullets[i] = 0;
            pthread_mutex_lock(&gpu_mutex);
            set_sprite(sprite_bullets[i]);
            pthread_mutex_unlock(&gpu_mutex);
        }
        if(obstaculos_gerados[i]){
            screen_obs[i].on_frame = 0;
            obstaculos_gerados[i] = 0;
            invisible_obstacle.data_register = 21 + i;
            pthread_mutex_lock(&gpu_mutex);
            set_sprite(invisible_obstacle);
            pthread_mutex_unlock(&gpu_mutex);
        }
    }
    if(game_mode == dual_player) {
        module_init_mouse_1();
        module_init_mouse_2();
        bg_animation_module_init();
        set_sprite(player_1_sprite);
        set_sprite(player_2_sprite);
        reestart_threads();
        //TODO: tela de iniciar o jogo
    } else {
        module_init_mouse_1();
        bg_animation_module_init();
        set_sprite(player_1_sprite);
        reestart_threads();
        //TODO: tela inicial do jogo
    }
    
}

int end_game() {
    pause_threads();
    //TODO: tela de finalizar o jogo
    module_exit_mouse_1();
    module_exit_mouse_2();
    //finalizando as threads
    pthread_cancel(obstacle_thread);
    pthread_cancel(background_thread);
    pthread_cancel(mouse_1_thread);
    pthread_cancel(mouse_2_thread);
    pthread_cancel(player_1_timer_thread);
    pthread_cancel(player_2_timer_thread);
    pthread_cancel(bullets_thread);
    pthread_cancel(colision_thread);

    pthread_join(obstacle_thread, NULL);
    pthread_join(background_thread, NULL);
    pthread_join(mouse_1_thread, NULL);
    pthread_join(mouse_2_thread, NULL);
    pthread_join(player_1_timer_thread, NULL);
    pthread_join(player_2_timer_thread, NULL);
    pthread_join(bullets_thread, NULL);
    pthread_join(colision_thread, NULL);

    //encerrando os mutex
    pthread_mutex_destroy(&gpu_mutex);
    pthread_mutex_destroy(&background_mutex);
    pthread_mutex_destroy(&mouse_1_mutex);
    pthread_mutex_destroy(&mouse_2_mutex);
    pthread_mutex_destroy(&obstacle_mutex);
    pthread_mutex_destroy(&player_1_invunerability_mutex);
    pthread_mutex_destroy(&player_2_invunerability_mutex);
    pthread_mutex_destroy(&bullets_mutex);
    pthread_mutex_destroy(&colision_mutex);

    //encerrando as condicionais
    pthread_cond_destroy(&mouse_1_cond);
    pthread_cond_destroy(&mouse_2_cond);
    pthread_cond_destroy(&obstacle_cond);
    pthread_cond_destroy(&background_cond);
    pthread_cond_destroy(&player_1_invulnerability_cond);
    pthread_cond_destroy(&player_2_invulnerability_cond);
    pthread_cond_destroy(&bullets_cond);
    pthread_cond_destroy(&colision_cond);


    GPU_close();
    return 0;
}

void menu() {
    state = in_menu;
    int btn_val;
    if (pause_background || pause_bullets || pause_colision || pause_mouse_1 || pause_obstacle || (pause_mouse_2 && game_mode == dual_player) || (pause_obstacle_2 && game_mode == dual_player)) {
        pause_threads();
    }
    while(1) {
        scanf("%d", &btn_val);

        if(btn_val == 1 && state == in_menu) {
            state = running;
            game_mode = dual_player;
            init_game();
        } else if (btn_val == 2 && state == in_menu){
            state = running;
            game_mode = single_player;
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
            break;
        } 

    }
    return NULL;
}

int main() {

    //inicializacao dos inteiros
    score_1 = 0;
    score_2 = 0;
    pause_background = 1;
    pause_mouse_1 = 1;
    pause_mouse_2 = 1;
    pause_obstacle = 1;
    player_1_invunerability = 0;
    player_2_invunerability = 0;
    pause_bullets = 1;
    pause_colision = 1;
    key_press_1 = 0;
    key_press_2 = 0;
    game_mode = dual_player; /*o modo de jogo padão é o de dois jogadores*/
    state = in_menu;

    GPU_open();
    initialize_obstacle_vector(obs);
    clear();

    player_1_sprite.coord_x = 200;
    player_1_sprite.coord_y = 340;
    player_1_sprite.data_register = 31;
    player_1_sprite.offset = BLUE_CAR;
    player_1_sprite.speed = 0;
    player_1_sprite.visibility = 1;

    player_2_sprite.coord_x = 460;
    player_2_sprite.coord_y = 340;
    player_2_sprite.data_register = 20;
    player_2_sprite.offset = GREEN_CAR;
    player_2_sprite.speed = 0;
    player_2_sprite.visibility = 1;

    //inicialização dos mutex
    pthread_mutex_init(&gpu_mutex, NULL);
    pthread_mutex_init(&background_mutex, NULL);
    pthread_mutex_init(&mouse_1_mutex, NULL);
    pthread_mutex_init(&mouse_2_mutex, NULL);
    pthread_mutex_init(&obstacle_mutex, NULL);
    pthread_mutex_init(&player_1_invunerability_mutex, NULL);
    pthread_mutex_init(&player_2_invunerability_mutex, NULL);
    pthread_mutex_init(&bullets_mutex, NULL);
    pthread_mutex_init(&colision_mutex, NULL);

    //inicialização das condições
    pthread_cond_init(&mouse_1_cond, NULL);
    pthread_cond_init(&mouse_2_cond, NULL);
    pthread_cond_init(&obstacle_cond, NULL);
    pthread_cond_init(&background_cond, NULL);
    pthread_cond_init(&player_1_invulnerability_cond, NULL);
    pthread_cond_init(&player_2_invulnerability_cond, NULL);
    pthread_cond_init(&bullets_cond, NULL);
    pthread_cond_init(&colision_cond, NULL);

    //inicialização das threads
    
    pthread_create(&background_thread, NULL, change_background_routine, NULL);
    pthread_create(&mouse_1_thread, NULL, mouse_1_polling_routine, NULL);
    pthread_create(&mouse_2_thread, NULL, mouse_2_polling_routine, NULL);
    pthread_create(&bullets_thread, NULL, bullet_routine, NULL);
    pthread_create(&obstacle_thread, NULL, random_obstacle_generate_routine, NULL);
    pthread_create(&colision_thread, NULL, colision_routine, NULL);
    pthread_create(&player_1_timer_thread, NULL, player_1_invulnerability_timer, NULL);
    pthread_create(&player_2_timer_thread, NULL, player_2_invulnerability_timer, NULL);

    //loop principal do jogo
    menu();
    
    return 0;
}