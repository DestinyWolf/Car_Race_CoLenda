#include <stdio.h>
#include "../Lib/colenda.h"
#include "obstacle.h"
#include "mouse_read.c"
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_mutex_t obs_mutex;
pthread_cond_t cond;
pthread_cond_t obs_cond;
int key, coord_x, coord_y;
int pause_thread = 0;
int pause_obstacle = 0;
sprite_t sp;

void* rotina(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (pause_thread)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);

        printf("continuou\n");
        read_mouse_event(&key, &coord_y, &coord_x);
        
    }
    
    return NULL;

}

void* obstaculo(void* arg) {

    while (1) {
        pthread_mutex_lock(&obs_mutex);
        while (pause_obstacle)
        {
            pthread_cond_wait(&obs_cond, &obs_mutex);
        }
        pthread_mutex_unlock(&obs_mutex);

        random_obstacle(sp.coord_x, sp.coord_y);
        
    }
    
    return NULL;
}

int main() {
    pthread_t thread, obs_thread;
    key = 0; coord_x = 0; coord_y = 0;

    
    sp.coord_x = 0;
    sp.coord_y = 0;
    sp.offset = BLUE_CAR;
    sp.data_register = 31;
    sp.visibility = 1;

    color_t white = {7,7,7};

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&obs_mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&obs_cond, NULL);
    GPU_open();
    clear();
    set_background_color(white);
    set_sprite(sp);
    
    if (!module_init()) {
        printf("erro ao iniciar modulo do mouse\n");
        return 0;
    }
    printf("thread criada\n");

    if(pthread_create(&thread, NULL, rotina, NULL) != 0) {
        printf("erro ao criar thread\n");
        return 0;
    }   
    if(pthread_create(&obs_thread, NULL, obstaculo, NULL) != 0) {
        printf("erro ao criar thread\n");
        return 0;
    }   
    printf("thread criada\n");

    while(1) {
        pthread_mutex_lock(&mutex);
        pause_thread = 1;
        pthread_mutex_unlock(&mutex);
        pthread_mutex_lock(&obs_mutex);
        pause_obstacle = 1;
        pthread_mutex_unlock(&obs_mutex);
        sp.coord_x = coord_x;
        sp.coord_y = y;
        set_sprite(sp);
        pthread_mutex_lock(&mutex);
        pause_thread = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        pthread_mutex_lock(&obs_mutex);
        pause_obstacle = 0;
        pthread_cond_signal(&obs_cond);
        pthread_mutex_unlock(&obs_mutex);
        
    }

    pthread_cancel(thread);
    pthread_cancel(obs_thread);
    pthread_join(thread, NULL);
    pthread_join(obs_thread, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&obs_mutex);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&obs_cond);
    GPU_close();
    module_exit();
    return 0;
}