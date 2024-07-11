#include "mouse_module.h"
#include "colision_module.h"
#include "obstacle.h"
#include "../Lib/colenda.h"
#include "background_animation_module.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


#define PLAYER_SPEED_BASE 10
#define BULLET_SPRITE 1
#define BULLET_SPEED_BASE 15

//do registrador 1 ao 10 são as balas, do 11 ao 20 são cenarios, do 21 ao 30 são obstaculos e o 31
obstacle_t obstacle[20];
sprite_t cenario[10];
sprite_t sprite_bullets[10];
sprite_t player_sprite;

//mutex utilizados
pthread_mutex_t gpu_mutex;
pthread_mutex_t background_mutex;
pthread_mutex_t mouse_mutex;
pthread_mutex_t obstaculos_mutex;
pthread_mutex_t bullets_routine_mutex;
pthread_mutex_t bullets_mutex;
pthread_mutex_t player_invunerability_mutex;

//condicionais utilizadas
pthread_cond_t mouse_cond;
pthread_cond_t obstacle_cond;
pthread_cond_t background_cond;
pthread_cond_t bullets_routine_cond;
pthread_cond_t player_invulnerability_cond;

int pause_background, pause_obstacle, pause_mouse, pause_bullets;
int player_invunerability;
int obstaculos_gerados[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int score;
int bullets[10] = {0,0,0,0,0,0,0,0,0,0};
int key_press;


//fazer a parte de piscar
void* player_invulnerability_timer(void* args) {
    while(1){
        pthread_mutex_lock(&player_invunerability_mutex);
        while(!player_invunerability){
            pthread_cond_wait(&player_invulnerability_cond, &player_invunerability_mutex);
        }
        pthread_mutex_unlock(&player_invunerability_mutex);

        for(int i = 0; i < 10; i++) {
            pthread_mutex_lock(&mouse_mutex);
            pause_mouse = 1;
            pthread_mutex_unlock(&mouse_mutex);
            player_sprite.visibility = (player_sprite.visibility) ? 0:1;

            usleep(200000000);

            thread_mutex_lock(&mouse_mutex);
            pause_mouse = 0;
            pthread_cond_signal(&mouse_cond);
            pthread_mutex_unlock(&mouse_mutex);
        }
        

        pthread_mutex_lock(&player_invunerability_mutex);
        player_invunerability = 0;
        pthread_mutex_unlock(&player_invunerability_mutex);
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

//rotina de movimentação das balas/disparos
void* bullets_update_routine(void* args) {
    while (1) {
        pthread_mutex_lock(&bullets_routine_mutex);

        while (pause_bullets)
        {
            pthread_cond_wait(&bullets_routine_cond, &bullets_routine_mutex);
        }
        pthread_mutex_unlock(&bullets_routine_mutex);

        for (int i = 0; i < 10; i++) {
            if (bullets[i]) {
                pthread_mutex_lock(&gpu_mutex);

                sprite_bullets[i].coord_y -= sprite_bullets[i].speed;
                if (sprite_bullets[i].coord_y < 0) {
                    sprite_bullets[i].visibility = 0;
                    bullets[i] = 0;
                }
                set_sprite(sprite_bullets[i]);

                pthread_mutex_unlock(&gpu_mutex);
            }
        }
    }

}

void* mouse_polling_routine(void* args) {
    int value_x_mouse, i;
    int car_speed, car_sprite;
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

        //detecção de borda
        if (player_sprite.coord_x - 10 >= 95 && player_sprite.coord_x - 10 + car_speed <= 95) {
            //pega o espaço restante que o carro ainda pode se mover
            player_sprite.coord_x += (95 - player_sprite.coord_x - 10 + car_speed); 
        } else if (player_sprite.coord_x + 10 <= 550 && player_sprite.coord_x + 10 + car_speed >= 550) {
            //pega o espaço restante que o carro ainda pode se mover
            player_sprite.coord_x += (550 - player_sprite.coord_x + 10 + car_speed);
        } else { 
            player_sprite.coord_x += car_speed;
        }

        if (key_press) {
            while(i < 10){
                if(bullets[i] == 0){
                    bullets[i] = 1;
                    break;
                }
                i++;
            }
        }

        sprite_bullets[i].offset = BULLET_SPRITE;
        sprite_bullets[i].coord_x = player_sprite.coord_x;
        sprite_bullets[i].coord_y = player_sprite.coord_y + 20;
        sprite_bullets[i].visibility = 1;
        sprite_bullets[i].data_register = (i + 1);
        sprite_bullets[i].speed = BULLET_SPEED_BASE;

        pthread_mutex_lock(&gpu_mutex);
        set_sprite(player_sprite);
        if (i < 10) {
            set_sprite(sprite_bullets[i]);
        }
        pthread_mutex_unlock(&gpu_mutex);
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

    bg_animation_module_init();
    while (1)
    {
        for (int i = 0; i < 20; i++)
        {
            if(obstaculos_gerados[i]) {
                pthread_mutex_lock(&obstaculos_mutex);
                pause_obstacle = 1;
                pthread_mutex_unlock(&obstaculos_mutex);
                for (int j = 0; i < 10; j++)
                {
                    if (bullets[j]) {

                        pthread_mutex_lock(&bullets_mutex);
                        pause_bullets = 1;
                        pthread_mutex_unlock(&bullets_mutex);

                        if (check_colision_bullet(sprite_bullets[j], obstacle[i])) {
                            score += obstacle[i].reward;
                        }
                        pthread_mutex_lock(&bullets_mutex);
                        pause_bullets = 0;
                        pthread_cond_signal(&bullets_routine_cond);
                        pthread_mutex_unlock(&bullets_mutex);
                    }
                }
                pthread_mutex_lock(&obstaculos_mutex);
                pause_obstacle = 0;
                pthread_cond_signal(&obstacle_cond);
                pthread_mutex_unlock(&obstaculos_mutex);
            }
        }

        pthread_mutex_lock(&player_invunerability_mutex);
        if (!player_invunerability){
            for (int i = 0; i < 20; i++) {
                if(obstaculos_gerados[i]) {
                    pthread_mutex_lock(&obstaculos_mutex);
                    pause_obstacle = 1;
                    pthread_mutex_unlock(&obstaculos_mutex);

                    pthread_mutex_lock(&mouse_mutex);
                    pause_mouse = 1;
                    pthread_mutex_unlock(&mouse_mutex);

                    if(check_colision_player(player_sprite, obstacle[i])){
                        score -= obstacle[i].reward;
                        player_invunerability = 1;
                        pthread_cond_signal(&player_invulnerability_cond);
                        pthread_mutex_lock(&mouse_mutex);
                        pause_mouse = 0;
                        pthread_cond_signal(&mouse_cond);
                        pthread_mutex_unlock(&mouse_mutex);
                        break;
                    }
                    pthread_mutex_lock(&mouse_mutex);
                    pause_mouse = 0;
                    pthread_cond_signal(&mouse_cond);
                    pthread_mutex_unlock(&mouse_mutex);
                }
            }
            pthread_mutex_lock(&obstaculos_mutex);
            pause_obstacle = 0;
            pthread_cond_signal(&obstacle_cond);
            pthread_mutex_unlock(&obstaculos_mutex);
        }
        pthread_mutex_unlock(&player_invunerability_mutex);
        

        if(score < 0) {
            printf("jogador perdeu");
            break;
        }
        

    }
}

//personagem se tor invuneravel por algum tempo apos uma colisão para permitir que o jogo continue
//ao fim da atualização da tela trocar posição do player
int main() {

    //inicializacao dos inteiros
    score = 0;
    pause_background = 0;
    pause_mouse = 0;
    pause_obstacle = 0;
    player_invunerability = 0;

    pthread_t obstacle_thread, background_thread, mouse_thread, bullets_thread, player_timer_thread; 
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
    pthread_mutex_init(&player_invunerability_mutex, NULL);

    //inicialização das condições
    pthread_cond_init(&mouse_cond, NULL);
    pthread_cond_init(&obstacle_cond, NULL);
    pthread_cond_init(&background_cond, NULL);
    pthread_cond_init(&player_invulnerability_cond, NULL);

    //inicialização das threads
    pthread_create(&obstacle_thread, NULL, random_obstacle_generate_routine, NULL);
    pthread_create(&background_thread, NULL, change_background_routine, NULL);
    pthread_create(&mouse_thread, NULL, mouse_polling_routine, NULL);
    pthread_create(&bullets_thread, NULL, bullets_update_routine, NULL);
    pthread_create(&player_timer_thread, NULL, player_invulnerability_timer, NULL);

    //loop principal do jogo
    game_main_loop();
    

    //finalizando as threads
    pthread_cancel(obstacle_thread);
    pthread_cancel(background_thread);
    pthread_cancel(mouse_thread);
    pthread_cancel(bullets_thread);
    pthread_cancel(player_timer_thread);

    pthread_join(obstacle_thread, NULL);
    pthread_join(background_thread, NULL);
    pthread_join(mouse_thread, NULL);
    pthread_join(bullets_thread, NULL);
    pthread_join(player_timer_thread, NULL);


    //encerrando os mutex
    pthread_mutex_destroy(&gpu_mutex);
    pthread_mutex_destroy(&background_mutex);
    pthread_mutex_destroy(&mouse_mutex);
    pthread_mutex_destroy(&obstaculos_mutex);
    pthread_mutex_destroy(&bullets_mutex);
    pthread_mutex_destroy(&player_invunerability_mutex);

    //encerrando as condicionais
    pthread_cond_destroy(&mouse_cond);
    pthread_cond_destroy(&obstacle_cond);
    pthread_cond_destroy(&background_cond);
    pthread_cond_destroy(&bullets_routine_cond);
    pthread_cond_destroy(&player_invulnerability_cond);


    GPU_close();
    return 0;
}