#include "obstacle.h"
#include "offset_sprite.h"
#include "create_cover.h"
#include "../Lib/colenda.h"
#include "../drivers/7seg_display/display_7seg.h"
#include "../drivers/pushbuttons/keys.h"
#include "mouse_module.h"
#include "colision_module.h"
#include "background_animation_module.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*estados do jogo*/
typedef enum {
    MENU = 1,
    RUNNING,
    PAUSED,
    GAME_OVER,
    WIN,
    FINISHED,
} game_state_t;

typedef struct {
    sprite_t sprite;
    int score;
    int bullets[5];
    int invunerability;
} player_t;

pthread_t polling_thread, screen_update_thread, colision_thread, ground_and_bullet_thread, timer_thread;

pthread_mutex_t polling_mutex;
pthread_mutex_t screen_mutex;
pthread_mutex_t update_data_mutex;
pthread_mutex_t ground_and_bullet_mutex;
pthread_mutex_t flags_mutex;
pthread_mutex_t timer_mutex;
pthread_mutex_t colision_mutex;

pthread_cond_t polling_cond;
pthread_cond_t screen_cond;
pthread_cond_t timer_cond;
pthread_cond_t ground_and_bullet_cond;
pthread_cond_t colision_cond;

player_t player;
game_state_t current_state;

sprite_t text[9];
sprite_t bullets[10];
obstacle_t obs_screen[10];
int obs_status[10];
int pause_polling, pause_update_screen, pause_ground_and_bullet, pause_colision;
int change_ground_flag, create_obstacle_flag, update_bullet_flag, invulnerability_flag, obs_destroy_flag;

void* invunerability_player_routine(void* args) {
    int i = 0;
    while (i < 10) {
        pthread_mutex_lock(&timer_mutex);
        while(current_state == PAUSED) {
            pthread_cond_wait(&timer_cond, &timer_mutex);
        }
        pthread_mutex_unlock(&timer_mutex);
        pthread_mutex_lock(&update_data_mutex);
        if (player.invunerability) {
            if (player.sprite.visibility) {
                player.sprite.visibility = 0;
            } else {
                player.sprite.visibility = 1;
            }
        }
        pthread_mutex_unlock(&update_data_mutex);
        i++;
        usleep(200000);
    }
    pthread_mutex_lock(&update_data_mutex);
    player.invunerability = 0;
    player.sprite.visibility = 1;
    pthread_mutex_unlock(&update_data_mutex);
    pthread_mutex_lock(&flags_mutex);
    invulnerability_flag = 0;
    pthread_mutex_unlock(&flags_mutex);

    return NULL;
}

void* change_ground_and_update_bullet_pos(void* args) {
    while (current_state != MENU && current_state != FINISHED) {
        pthread_mutex_lock(&ground_and_bullet_mutex);
        while(pause_ground_and_bullet || current_state == PAUSED) {
            pthread_cond_wait(&ground_and_bullet_cond, &ground_and_bullet_mutex);
        }
        pthread_mutex_unlock(&ground_and_bullet_mutex);

        for(int i = 0; i < 5; i++) {
            pthread_mutex_lock(&update_data_mutex);
            if (player.bullets[i]) { 
                bullets[i].coord_y -= bullets[i].speed;
                if (bullets[i].coord_y <= bullets[i].speed) { 
                    bullets[i].visibility = 0;
                    player.bullets[i] = 0;
                    update_bullet_flag = 1;
                }
            }
            pthread_mutex_unlock(&update_data_mutex);
        }
        pthread_mutex_lock(&flags_mutex);
        change_ground_flag = 1;
        create_obstacle_flag = 1;
        pthread_mutex_unlock(&flags_mutex);
        usleep(100000);
    }
    return NULL;
}

void* polling_routine(void* args) {
    int coord_x, offset, key_press, shot_flag;
    while (current_state != MENU && current_state != FINISHED)
    {
        pthread_mutex_lock(&polling_mutex);
        while(pause_polling || current_state == PAUSED) {
            pthread_cond_wait(&polling_cond, &polling_mutex);
        }
        pthread_mutex_unlock(&polling_mutex);

        read_mouse_1_event(&key_press, &coord_x);
        offset = coord_x * player.sprite.speed;

        if ( offset < 0 && player.sprite.coord_x - 10 >= 96 && (player.sprite.coord_x - 10) + offset <= 96) {
            //pega o espaço restante que o carro ainda pode se mover antes de chegar na borda
            player.sprite.coord_x -= ((player.sprite.coord_x - 10 + offset) - 96); 
        } else if (offset > 0 && player.sprite.coord_x + 10 <= 289 && (player.sprite.coord_x + 10) + offset >= 289) {
            //pega o espaço restante que o carro ainda pode se mover antes de chegar na borda
            player.sprite.coord_x += (289 - (player.sprite.coord_x + 10 + offset));
        } else { 
            player.sprite.coord_x += offset;
        }

        if (key_press && !shot_flag) {
            pthread_mutex_lock(&screen_mutex);
            pause_update_screen = 1;
            pthread_mutex_unlock(&screen_mutex);
            for(int i = 0; i < 5; i ++) {
                if (player.bullets[i] == 0) {
                    player.bullets[i] = 1;
                    bullets[i].coord_x = player.sprite.coord_x;
                    bullets[i].coord_y = player.sprite.coord_y - 20;
                    bullets[i].speed = 15;
                    bullets[i].offset = 11;
                    bullets[i].data_register = i + 1;
                    bullets[i].visibility = 1;
                    shot_flag = 1;
                    pthread_mutex_lock(&flags_mutex);
                    update_bullet_flag = 1;
                    pthread_mutex_lock(&flags_mutex);
                    break;
                }
            }
            pthread_mutex_lock(&screen_mutex);
            pause_update_screen = 0;
            pthread_cond_signal(&screen_cond);
            pthread_mutex_unlock(&screen_mutex);
        } else if (key_press){
            shot_flag = 1;
        } else {
            shot_flag = 0;
        }
    }
    
    return NULL;
}

void* update_screen_routine(void* args) {
    sprite_t invisible = {.coord_x = 1, .coord_y = 1, .visibility = 0, .offset = 0};
    while(current_state != MENU && current_state != FINISHED){
        pthread_mutex_lock(&screen_mutex);
        while (pause_update_screen || current_state == PAUSED) {
            pthread_cond_wait(&screen_cond, &screen_mutex);
        }
        pthread_mutex_unlock(&screen_mutex);
        pthread_mutex_lock(&update_data_mutex);
        pthread_mutex_lock(&polling_mutex);
        pause_polling = 1;
        pthread_mutex_unlock(&polling_mutex);
        set_sprite(player.sprite);
        pthread_mutex_lock(&polling_mutex);
        pause_polling = 0;
        pthread_cond_signal(&polling_cond);
        pthread_mutex_unlock(&polling_mutex);

        pthread_mutex_lock(&flags_mutex);
        if (update_bullet_flag) {
            for(int i = 0; i <5; i++) {
                if(player.bullets[i]) {
                    set_sprite(bullets[i]);
                }
            }
            pthread_mutex_lock(&flags_mutex);
            update_bullet_flag = 0;
        }
        pthread_mutex_unlock(&flags_mutex);
        pthread_mutex_lock(&flags_mutex);
        if (create_obstacle_flag) {
            random_obstacle(player.sprite.coord_x, player.sprite.coord_y, 96, 289, obs_screen, obs_status);
            create_obstacle_flag = 0;
        }
        pthread_mutex_unlock(&flags_mutex);
        pthread_mutex_unlock(&update_data_mutex);
        pthread_mutex_lock(&flags_mutex);
        if (change_ground_flag) {
            bg_animation();
            change_ground_flag = 0;
            
        }
        pthread_mutex_unlock(&flags_mutex);
        pthread_mutex_lock(&flags_mutex);
        if(obs_destroy_flag) {
            for(int i = 0; i < 10; i++) {
                if(obs_screen[i].on_frame) {
                    obs_screen[i].on_frame = 0;
                    invisible.data_register = 20 + i;
                    set_sprite(invisible);
                }
            }
            obs_destroy_flag = 0;
        }
        pthread_mutex_unlock(&flags_mutex);
        
        
    }
    return NULL;
}

void* verify_colision_routine(void* args) {
    while(current_state != MENU && current_state != FINISHED){
        pthread_mutex_lock(&colision_mutex);
        while (pause_colision || current_state == PAUSED) {
            pthread_cond_wait(&colision_cond, &colision_mutex);
        }
        pthread_mutex_unlock(&colision_mutex);

        for(int i = 0; i < 10; i++){
            if (obs_status[i]) {
                for (int j = 0; j < 5; j++) {
                    if(player.bullets[j]) {
                        if (check_collision(bullets[j], obs_screen[i])) {
                            pthread_mutex_lock(&update_data_mutex);
                            player.score += obs_screen[i].reward;
                            display_write_score(player.score, 0);
                            player.bullets[j] = 0;
                            obs_status[i] = 0;
                            bullets[j].visibility = 0;
                            pthread_mutex_unlock(&update_data_mutex);

                            pthread_mutex_lock(&flags_mutex);
                            update_bullet_flag = 1;
                            obs_destroy_flag = 1;
                            pthread_mutex_unlock(&flags_mutex);
                            break;
                        }
                    }
                }
                if (!player.invunerability && obs_status[i]) {
                    if (check_collision(player.sprite, obs_screen[i])) {
                        pthread_mutex_lock(&update_data_mutex);
                        player.score -= obs_screen[i].reward;
                        display_write_score(player.score, 0);
                        obs_status[i] = 0;
                        player.invunerability = 1;
                        pthread_mutex_unlock(&update_data_mutex);
                        pthread_mutex_lock(&flags_mutex);
                        invulnerability_flag = 1;
                        obs_destroy_flag = 1;
                        pthread_mutex_unlock(&flags_mutex);

                        pthread_create(&timer_thread, NULL, invunerability_player_routine, NULL);
                    }
                }
            }
        }

        if (player.score >= 1000) {
            printf("player 1 ganhou\n");
            current_state = PAUSED;
            win();
        } else if (player.score < 0) {
            printf("player 1 perdeu");
            current_state = PAUSED;
            lose();
        }
    }

    return NULL;
}

void init_game() {
    player.sprite.coord_x = 192;
    player.sprite.coord_y = 340;
    player.sprite.speed = 2;
    player.sprite.visibility = 1;
    player.sprite.offset = RED_CAR;
    player.score = 0;
    player.invunerability = 0;
    for (int i = 0; i < 5; i++) {
        player.bullets[i] = 0;
        obs_status[i] = 0;
        obs_status[i+5] = 0;
        obs_screen[i].on_frame = 0;
        obs_screen[i+5].on_frame = 0;
    }
    pthread_create(colision_thread, NULL, verify_colision_routine, NULL);
    pthread_create(polling_thread, NULL, polling_routine, NULL);
    pthread_create(ground_and_bullet_thread, NULL, change_ground_and_update_bullet_pos, NULL);
    pthread_create(screen_update_thread, NULL, update_screen_routine, NULL);
    clear();
    bg_animation_module_init();
}

void paused_game() {
    sprite_t invisible_sprite = {
    .coord_x = 1, 
    .coord_y = 1, 
    .offset = 0, 
    .speed = 0, 
    .visibility = 0};
    int msg_letters[5] = {P, A, U, S, E};

    pthread_mutex_lock(&gpu_mutex);
    for (int i = 0; i < 10; i++)
    {
        if(obs_status[i]){
            invisible_sprite.data_register = 20 + i;
            set_sprite(invisible_sprite);
        }
        if ( i < 5 && player.bullets[i]) {
            invisible_sprite.data_register = 1 + i;
            set_sprite(invisible_sprite);
        }
        
    }
    player.sprite.visibility = 0;
    set_sprite(player.sprite);
    
    for (int i = 0; i < 5; i++)
    {
        text[i].coord_x = 282 + (20 * i);
        text[i].coord_y = 240;
        text[i].data_register = 11 + i;
        text[i].visibility = 1;
        text[i].offset = msg_letters[i];
        text[i].speed = 0;
        set_sprite(text[i]);
    }

    return;
}

void return_to_paused() {

    for(int i = 0; i <5; i++) {
        text[i].visibility = 0;
        set_sprite(text[i]);
    }
    change_ground_flag = 1;
    update_bullet_flag = 1;
    create_obstacle_flag = 1;

    current_state = GAME;
    if(player.invunerability) {
        pthread_cond_signal(&timer_cond);
    }
    pthread_cond_signal(&polling_cond);
    pthread_cond_signal(&screen_cond);
    pthread_cond_signal(&colision_cond);
    pthread_cond_signal(&ground_and_bullet_cond);
    return;

}
void win() {
    sprite_t invisible_sprite = {
    .coord_x = 1, 
    .coord_y = 1, 
    .offset = 0, 
    .speed = 0, 
    .visibility = 0};
    int msg_letters[6] = {V, E, N, C, E, U};
    for (int i = 0; i < 10; i++)
    {
        if(obs_status[i]){
            invisible_sprite.data_register = 20 + i;
            set_sprite(invisible_sprite);
        }
        if ( i < 5 && player.bullets[i]) {
            invisible_sprite.data_register = 1 + i;
            set_sprite(invisible_sprite);
        }
    }
    for(int i = player.sprite.coord_y; i >= 1; --i) {
        player.sprite.coord_y = i;
        set_sprite(player.sprite);
        usleep(100000);
    }
    player.sprite.visibility = 0;
    set_sprite(player.sprite);

    for (int i = 0; i < 6; i++)
    {
        text[i].coord_x = 282 + (20 * i);
        text[i].coord_y = 240;
        text[i].data_register = 11 + i;
        text[i].visibility = 1;
        text[i].offset = msg_letters[i];
        text[i].speed = 0;
        set_sprite(text[i]);
    }
    sleep(1);
    pthread_join(&colision_thread, NULL);
    pthread_join(&polling_thread, NULL);
    pthread_join(&ground_and_bullet_thread, NULL);
    pthread_join(&screen_update_thread, NULL);
    pthread_join(&timer_thread, NULL);
    clear();
    current_state = MENU;
    draw_cover_art();
    set_menu();
    return;
}

void lose() {

    sprite_t invisible_sprite = {
    .coord_x = 1, 
    .coord_y = 1, 
    .offset = 0, 
    .speed = 0, 
    .visibility = 0};
    int msg_letters[6] = {P, E, R, D, E, U};
    while(player.invunerability) {
        pthread_cond_signal(&timer_cond);
    }
    
    for (int i = 0; i < 10; i++)
    {
        
        if(obs_status[i]){
            invisible_sprite.data_register = 20 + i;
            set_sprite(invisible_sprite);
        }
        if ( i < 5 && player.bullets[i]) {
            invisible_sprite.data_register = 1 + i;
            set_sprite(invisible_sprite);
        }
        
    }
    for (int i = 0; i < 6; i++)
    {
        text[i].coord_x = 280 + (20 * i);
        text[i].coord_y = 240;
        text[i].data_register = 11 + i;
        text[i].visibility = 1;
        text[i].offset = msg_letters[i];
        text[i].speed = 0;
        set_sprite(text[i]);
    }
    sleep(1);
    pthread_join(&colision_thread, NULL);
    pthread_join(&polling_thread, NULL);
    pthread_join(&ground_and_bullet_thread, NULL);
    pthread_join(&screen_update_thread, NULL);
    pthread_join(&timer_thread, NULL);
    clear();
    current_state = MENU;
    draw_cover_art();
    set_menu();
    return;
}   

void menu() {
    char btn_pressed;

    while (current_state != FINISHED) {
        KEYS_read(&btn_pressed);
        if(btn_pressed == BUTTON0 && current_state == MENU) {
            current_state = GAME;
            init_game();
        } else if(btn_pressed == BUTTON1 && current_state == GAME) {
            current_state == PAUSED;
        } else if (btn_pressed == BUTTON1 && current_state == PAUSED) {
            return_to_paused();
        } else if (btn_pressed == BUTTON2) {
            current_state = MENU;
            pthread_join(&colision_thread, NULL);
            pthread_join(&polling_thread, NULL);
            pthread_join(&ground_and_bullet_thread, NULL);
            pthread_join(&screen_update_thread, NULL);
            pthread_join(&timer_thread, NULL);
            clear();
            draw_cover_art();
            set_menu();
        } else if (btn_pressed == BUTTON3) {
            current_state = FINISHED;
            return;
        }
    }
    
}

int main() {
    pthread_t polling_thread, screen_update_thread;

    pause_polling = 0;
    pause_update_screen = 0;
    pause_colision = 0;
    player.sprite.coord_x = 192;
    player.sprite.coord_y = 340;
    player.sprite.speed = 2;
    player.sprite.visibility = 1;
    player.sprite.offset = RED_CAR;
    player.score = 0;
    player.invunerability = 0;
    for (int i = 0; i < 5; i++) {
        player.bullets[i] = 0;
    }

    GPU_open();
    KEYS_open();
    display_open();
    module_init_mouse_1();
    initialize_obstacle_vector();
    system("./criar_sprite");
    clear();
    draw_cover_art();
    set_menu();

    pthread_mutex_init(&polling_mutex, NULL);
    pthread_mutex_init(&screen_mutex, NULL);
    pthread_mutex_init(&flags_mutex, NULL);
    pthread_mutex_init(&colision_mutex, NULL);
    pthread_mutex_init(&timer_mutex, NULL);
    pthread_mutex_init(&update_data_mutex, NULL);
    pthread_mutex_init(&ground_and_bullet_mutex, NULL);
    pthread_cond_init(&polling_cond, NULL);
    pthread_cond_init(&screen_cond, NULL);
    pthread_cond_init(&timer_cond, NULL);
    pthread_cond_init(&ground_and_bullet_cond, NULL);
    pthread_cond_init(&colision_cond, NULL);

    menu();

    pthread_join(&colision_thread, NULL);
    pthread_join(&polling_thread, NULL);
    pthread_join(&ground_and_bullet_thread, NULL);
    pthread_join(&screen_update_thread, NULL);
    pthread_join(&timer_thread, NULL);

    pthread_mutex_destroy(&ground_and_bullet_mutex);
    pthread_mutex_destroy(&colision_mutex);
    pthread_mutex_destroy(&update_data_mutex);
    pthread_mutex_destroy(&screen_mutex);
    pthread_mutex_destroy(&flags_mutex);
    pthread_mutex_destroy(&timer_mutex);
    pthread_mutex_destroy(&polling_mutex);

    pthread_cond_destroy(&polling_cond);
    pthread_cond_destroy(&screen_cond);
    pthread_cond_destroy(&timer_cond);
    pthread_cond_destroy(&ground_and_bullet_cond);
    pthread_cond_destroy(&colision_cond);

    module_exit_mouse_1();
    KEYS_close();
    display_close();
    GPU_close();
    return 0;
}
