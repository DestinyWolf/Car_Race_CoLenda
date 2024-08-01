#include "mouse_module.h"
#include "colision_module.h"
#include "../Lib/Colenda_Lib/colenda.h"
#include "../Lib/Key_Lib/keys.h"
#include "../Lib/Display_7seg_Lib/display_7seg.h"
#include "background_animation_module.h"
#include "create_cover.h"
#include "offset_sprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "create_sprite.h"


#define PLAYER_SPEED_BASE 2
#define OFFSET_PLAYER_1 18
#define BULLET_SPEED_BASE 15  /*definição da velocidade base do tiro*/

/*enumeração com os estados de jogo começando pelo 1*/
typedef enum {
    running = 1,
    in_pause,
    in_menu,
    finish,
    win,
    lose,
} states;

obstacle_t obstacle_model[16];
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

pthread_t obstacle_thread, mouse_thread, player_timer_thread, colision_thread; 

states state; /*variavel do tipo states responsavel por guardar o estado atual do jogo*/

int pause_background, pause_obstacle, pause_mouse, pause_bullets, pause_colision;
int player_invunerability;
int obstaculos_gerados[10] = {0,0,0,0,0,0,0,0,0,0};
int score;
int bullets[10] = {0,0,0,0,0,0,0,0,0,0};
int key_press;


void pause_threads() {
    pthread_mutex_lock(&colision_mutex);
    pause_colision = 1;
    pthread_mutex_unlock(&colision_mutex);
    pthread_mutex_lock(&mouse_mutex);
    pause_mouse = 1;
    pthread_mutex_unlock(&mouse_mutex);
    pthread_mutex_lock(&obstacle_mutex);
    pause_obstacle = 1;
    pthread_mutex_unlock(&obstacle_mutex);
    return;
}

void reestart_threads() {
    pthread_mutex_lock(&colision_mutex);
    pause_colision = 0;
    pthread_cond_signal(&colision_cond);
    pthread_mutex_unlock(&colision_mutex);
    pthread_mutex_lock(&mouse_mutex);
    pause_mouse = 0;
    pthread_cond_signal(&mouse_cond);
    pthread_mutex_unlock(&mouse_mutex);
    pthread_mutex_lock(&obstacle_mutex);
    pause_obstacle = 0;
    pthread_cond_signal(&obstacle_cond);
    pthread_mutex_unlock(&obstacle_mutex);
    return;
}

void* player_invulnerability_timer(void* args) {
    int i = 0;
    while(state != finish){
        pthread_mutex_lock(&player_invunerability_mutex);
        while(!player_invunerability || state == in_menu || state == in_pause){
            pthread_cond_wait(&player_invulnerability_cond, &player_invunerability_mutex);
        }
        pthread_mutex_unlock(&player_invunerability_mutex);
        if (player_sprite.visibility) {
            player_sprite.visibility = 0;
        } else {
            player_sprite.visibility = 1;
        }
        set_sprite(player_sprite);
        if(i == 10) {
            pthread_mutex_lock(&player_invunerability_mutex);
            player_invunerability = 0;
            pthread_mutex_unlock(&player_invunerability_mutex);
            player_sprite.visibility = 1;
            
            set_sprite(player_sprite);
            
            i = 0;
        }else {
            i++;
        }
        if(state == finish) {
            return NULL;
        }
        usleep(200000);
        if(state == finish) {
            return NULL;
        }
    }
    return NULL;
}

void* mouse_polling_routine(void* args) {
    
    int value_x_mouse = 0, i, has_shot = 0;
    int car_speed, car_sprite;

    while (state != finish) {
        pthread_mutex_lock(&mouse_mutex);
        while (pause_mouse || state == in_menu || state == in_pause || state == lose || state == win)
        {
            pthread_cond_wait(&mouse_cond, &mouse_mutex);
        }
        pthread_mutex_unlock(&mouse_mutex);
        read_mouse_1_event(&key_press, &value_x_mouse);

        
        car_speed = PLAYER_SPEED_BASE * value_x_mouse;
        car_sprite = player_sprite.offset + value_x_mouse;

        //detecção de borda
        if ( car_speed < 0 && player_sprite.coord_x - 10 >= 96 && (player_sprite.coord_x - 10) + car_speed <= 96) {
            //pega o espaço restante que o carro ainda pode se mover antes de chegar na borda
            player_sprite.coord_x -= ((player_sprite.coord_x - 10 + car_speed) - 96); 
        } else if (car_speed > 0 && player_sprite.coord_x + 10 <= 289 && (player_sprite.coord_x + 10) + car_speed >= 289) {
            //pega o espaço restante que o carro ainda pode se mover antes de chegar na borda
            player_sprite.coord_x += (289 - (player_sprite.coord_x + 10 + car_speed));
        } else { 
            player_sprite.coord_x += car_speed;
        }

        

        if (key_press == 1 && !has_shot) {
            i = 0;
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
        } else if (key_press){
            has_shot = 1;
        } else {
            has_shot = 0;
        }

        
        set_sprite(player_sprite);
        if(i<10) {
            set_sprite(sprite_bullets[i]);
        }
        
        if(state == finish) {
            return NULL;
        }
        
    }
    return NULL;
}

void* random_obstacle_generate_routine(void* args) {
    while (state != finish)
    {
        pthread_mutex_lock(&obstacle_mutex);
        while (pause_obstacle || state == in_menu || state == in_pause || state == lose || state == win)
        {
            pthread_cond_wait(&obstacle_cond, &obstacle_mutex);
        }
        pthread_mutex_unlock(&obstacle_mutex);

        
        random_obstacle(player_sprite.coord_x, player_sprite.coord_y, 96, 289, obstacle, obstaculos_gerados, obstacle_model);
        
        for (int i = 0; i < 10; i++)
        {
            if(bullets[i] == 1) {
                if (sprite_bullets[i].coord_y <= 15) {
                    bullets[i] = 0;
                    sprite_bullets[i].visibility = 0;
                } else {
                    sprite_bullets[i].coord_y -= BULLET_SPEED_BASE;
                }
                set_sprite(sprite_bullets[i]);
            } else {
                sprite_bullets[i].visibility = 0;
                set_sprite(sprite_bullets[i]);
                
            }
        }
        bg_animation(); //função do modulo que faz a atualização do fundo e da animação com o passar dos quadros

        if(state == finish) {
            return NULL;
        }
        usleep(100000);
        if(state == finish) {
            return NULL;
        }
    }
    return NULL;
}


// loop principal do jogo
void* colision_routine(void* args){
    sprite_t invisible_obstacle = {.coord_x = 1, .coord_y = 1, .offset = 1, .speed = 0, .visibility = 0};
    
    while (state != finish)
    {
        pthread_mutex_lock(&colision_mutex);
        while(pause_colision || state == in_menu || state == in_pause || state == lose || state == win){
            pthread_cond_wait(&colision_cond, &colision_mutex);
        }
        pthread_mutex_unlock(&colision_mutex);
        if(state == finish) {
            return NULL;
        }

        for (int i = 0; i < 10; i++)
        {
            if(obstaculos_gerados[i]) {
                pthread_mutex_lock(&obstacle_mutex);
                pause_obstacle = 1;
                pthread_mutex_unlock(&obstacle_mutex);
                for (int j = 0; j < 10; j++)
                {
                    if (bullets[j]) {
                        if (check_colision_bullet(sprite_bullets[j], obstacle[i])) {
                            score += obstacle[i].reward;
                            display_write_score(score, 0);
                            sprite_bullets[j].visibility = 0;
                            bullets[j] = 0;
                            invisible_obstacle.data_register = (20 + i);
                            obstacle[i].on_frame = 0;
                            obstaculos_gerados[i] = 0;
                            set_sprite(invisible_obstacle);
                        }
                    }
                }
                pthread_mutex_lock(&obstacle_mutex);
                pause_obstacle = 0;
                pthread_cond_signal(&obstacle_cond);
                pthread_mutex_unlock(&obstacle_mutex);
                if(obstacle[i].coord_y + 11 >= player_sprite.coord_y - 10) {
                    pthread_mutex_lock(&obstacle_mutex);
                    pause_obstacle = 1;
                    pthread_mutex_unlock(&obstacle_mutex);
                    pthread_mutex_lock(&player_invunerability_mutex);
                    if(!player_invunerability) {
                        if(check_colision_player(player_sprite, obstacle[i])){
                            score -= obstacle[i].reward;
                            display_write_score(score, 0);
                            player_invunerability = 1;
                            obstacle[i].on_frame = 0;
                            invisible_obstacle.data_register = (20 + i);
                            obstaculos_gerados[i] = 0;
                            set_sprite(invisible_obstacle);
                            
                            pthread_cond_signal(&player_invulnerability_cond);
                        }
                    }
                    pthread_mutex_unlock(&player_invunerability_mutex);
                    pthread_mutex_lock(&obstacle_mutex);
                    pause_obstacle = 0;
                    pthread_cond_signal(&obstacle_cond);
                    pthread_mutex_unlock(&obstacle_mutex);
                }
            }
        }  
    
        printf("score: %d\n", score);
        if (score < 0) {
            state = lose;
            lose_screen();
        } else if (score >= 1000) {
            display_write_score(999, 1);
            state = win;
            win_screen();
        }

    }
    return NULL;
}



void menu() {
    state = in_menu;
    char btn_val;
    // printf("chegou aqui\n");
    while(state != finish) {
        // printf("chegou aqui\n");
        // scanf("%c", &btn_val);
        KEYS_read(&btn_val);
        printf("chegou aqui\n");

        if(btn_val == BUTTON0 && state == in_menu) {
            state = running;
            init_game();
        } else if (btn_val == BUTTON1 && state == running) {
            state = in_pause;
            pause_screen();
        } else if (btn_val == BUTTON1 && state == in_pause) {
            state = running;
            return_screen();
        } else if (btn_val == BUTTON2) {
            state = in_menu;
            pause_threads();
            clear();
            clean_all_obstacles(obstacle, obstaculos_gerados);
            display_clear();
            draw_cover_art();
            set_menu();
            
        } else if (btn_val == BUTTON3) {
            state = finish;
            break;
        } 
    }
    return;
}


void pause_screen() {
    sprite_t invisible_sprite = {
    .coord_x = 1, 
    .coord_y = 1, 
    .offset = 0, 
    .speed = 0, 
    .visibility = 0};
    int msg_letters[5] = {P, A, U, S, E};
    pause_threads();

    
    all_obstacles_invisibles(obstacle, obstaculos_gerados);
    for (int i = 0; i < 10; i++)
    {
        if (bullets[i]) {
            invisible_sprite.data_register = 1 + i;
            set_sprite(invisible_sprite);
        }
        
    }
    player_sprite.visibility = 0;
    set_sprite(player_sprite);
    

    for (int i = 0; i < 5; i++)
    {
        scene[i].coord_x = 280 + (20 * i);
        scene[i].coord_y = 240;
        scene[i].data_register = 11 + i;
        scene[i].visibility = 1;
        scene[i].offset = msg_letters[i];
        scene[i].speed = 0;
        set_sprite(scene[i]);
    }
    return;
}

void return_screen() {
    sprite_t invisible_sprite = {
    .coord_x = 1, 
    .coord_y = 1, 
    .offset = 0, 
    .speed = 0, 
    .visibility = 0};
    
    player_sprite.visibility = 1;
    

    for (int i = 0; i < 5; i++) {
        scene[i].visibility = 0;
        set_sprite(scene[i]);
    }
    set_sprite(player_sprite);
    
    reestart_threads();
    state = running;
    return;
}

void win_screen() {
    sprite_t invisible_sprite = {
    .coord_x = 1, 
    .coord_y = 1, 
    .offset = 0, 
    .speed = 0, 
    .visibility = 0};
    int coord_x = 0;
    int msg_letters[6] = {V, E, N, C, E, U};
    pause_threads();
    
    all_obstacles_invisibles(obstacle, obstaculos_gerados);
    for (int i = 0; i < 10; i++)
    {
        if (bullets[i]) {
            invisible_sprite.data_register = 1 + i;
            set_sprite(invisible_sprite);
        }
    }
    
    
    
    for(int i = player_sprite.coord_y; i >=0; --i) {
        player_sprite.coord_y = i;
        
        set_sprite(player_sprite);
        
        coord_x = 130;
        usleep(10000);
    }
    

    for(int i = 0; i < 6; i++) {
        scene[i].coord_x = coord_x + (20 * i);
        scene[i].coord_y = 240;
        scene[i].data_register = 11 + i;
        scene[i].visibility = 1;
        scene[i].offset = msg_letters[i];
        scene[i].speed = 0;
        set_sprite(scene[i]);
    }

    sleep(1);

    state = in_menu;
    clear();
    draw_cover_art();
    set_menu();
    return;
}

void lose_screen() {
    sprite_t invisible_sprite = {
    .coord_x = 1, 
    .coord_y = 1, 
    .offset = 0, 
    .speed = 0, 
    .visibility = 0};

    int coord_x;
    int msg_letters[6] = {P, E, R, D, E, U};
    pause_threads();
    while(player_invunerability){
    }
    
    coord_x = 130;
    
    all_obstacles_invisibles(obstacle, obstaculos_gerados);
    for (int i = 0; i < 10; i++)
    {
        if (bullets[i]) {
            invisible_sprite.data_register = 1 + i;
            
            set_sprite(invisible_sprite);
            
        }
    }

    player_sprite.visibility = 0;

    set_sprite(player_sprite);
    
    player_invunerability = 0;

    for (int i = 0; i < 6; i++) {
        scene[i].coord_x = coord_x + (20 * i);
        scene[i].coord_y = 240;
        scene[i].data_register = 11 + i;
        scene[i].visibility = 1;
        scene[i].offset = msg_letters[i];
        scene[i].speed = 0;
        set_sprite(scene[i]);
    }

    sleep(1);
    clear();
    state = in_menu;
    draw_cover_art();
    set_menu();
    return;
}

void init_game() {
    sprite_t invisible_sprite = {.coord_x = 1, .coord_y = 1, .offset = 0, .speed = 0, .visibility = 0};
    score = 950;
    display_clear();
    clean_all_obstacles(obstacle, obstaculos_gerados);
    for(int i = 0; i< 10; i++) {
        if (bullets[i]) {
            bullets[i] = 0;
        }
    }
    clear();
    
    bg_animation_module_init();
    
    reestart_threads();
    player_sprite.visibility = 1;
    player_sprite.coord_x = 200;
    player_sprite.coord_y = 340;
    set_sprite(player_sprite);
    return;
}

void main() {

    module_init_mouse_1();
    
    //inicializacao dos inteiros
    score = 0;
    pause_background = 1;
    pause_mouse = 1;
    pause_obstacle =1;
    player_invunerability = 0;
    pause_bullets = 0;
    pause_colision = 1;
    key_press = 0;
    state = in_menu;

    
    GPU_open();
    KEYS_open();
    display_open();
    set_new_sprites();
    initialize_obstacle_vector(obstacle_model);
    clear();
    draw_cover_art();
    set_menu();
    player_sprite.coord_x = 200;
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
    pthread_create(&mouse_thread, NULL, mouse_polling_routine, NULL);
    pthread_create(&player_timer_thread, NULL, player_invulnerability_timer, NULL);
    pthread_create(&colision_thread, NULL, colision_routine, NULL);

    //loop principal do jogo
    
    menu();
    

    //finalizando as threads
    pthread_cancel(obstacle_thread);
    //pthread_cancel(background_thread);
    pthread_cancel(mouse_thread);
    pthread_cancel(player_timer_thread);
    //pthread_cancel(bullets_thread);


    pause_colision = 0;
    pause_obstacle = 0;
    pause_mouse = 0;
    pthread_cond_broadcast(&colision_cond);
    pthread_cond_broadcast(&obstacle_cond);
    pthread_cond_broadcast(&mouse_cond);
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
    clear();
    display_clear();

    KEYS_close();
    display_close();
    GPU_close();
    return 0;
}
