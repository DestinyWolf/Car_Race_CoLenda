#include "mouse_module.h"
#include "colision_module.h"
#include "../Lib/colenda.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


sprite_t obstaculos[10];
sprite_t cenario[10];
sprite_t jogador;

pthread_mutex_t change_background_mutex;
pthread_mutex_t mouse_mutex;
pthread_mutex_t player_mutex;
pthread_mutex_t obstaculos_mutex;
pthread_cond_t player_cond;
pthread_cond_t obstacle_cond;
pthread_cond_t background_cond;


int pause_background, pause_obstacle, pause_player;

int obstaculos_gerados;
int score;
int bullets;


void* change_background_routine(void* args) {
    //todo
}

void* mouse_polling_routine(void* args) {
    //todo
}

void* player_update_routine(void* args) {
    //todo
}

void* random_obstacle_generate_routine(void* args) {
    //todo
}

int main() {
    obstaculos_gerados = 0;
    score = 0;
    bullets = 10;



    return 0;
}