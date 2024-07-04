#include <pthreads.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include "../Lib/colenda.h"

pthread_mutex_t mutex;
pthread_cond_t cond;
int pause_thread = 0;


void* send_animation(void* arg) {

    color_t verde = {
        .blue = 2,
        .green = 6,
        .red = 2,
    };

    color_t cinza = {
        .blue = 4,
        .green = 4,
        .red = 4,
    };

    color_t branco = {
        .blue = 6,
        .green = 6,
        .red = 6,
    };

    color_t preto = {
        .blue = 0,
        .green = 0,
        .red = 0,
    };

    color_t invisivel = {
        .blue = 7,
        .green = 7,
        .red = 6,
    };

    int x, change;

    clear();
    set_background_color(cinza);

    for (int i = 0; i < 9; i++)
    {
        draw_vertical_block_line(60, i, 0, verde);
        draw_vertical_block_line(60, (79-i), 0, verde);
    }

    draw_vertical_block_line(60, 9, 0, preto);
    draw_vertical_block_line(60, 70, 0, preto);

    x = 9;
    for (int i = 0; i < 5; i++)
    {
        x +=6;

        for (int j = 0; j < 6; j++)
        {
            draw_vertical_block_line(5, x-3, (j*10), branco);
            

            draw_vertical_block_line(5, 79-x+3,(j*10), branco);
        }
        

        draw_vertical_block_line(60, x, 0, preto);
        draw_vertical_block_line(60, 79-x, 0, preto);

    }

    change = 0;

    GPU_open();
    while(1) {
        pthread_mutex_lock(&mutex);
        while (pause_thread) {
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);



    }
    GPU_close();
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;

    //o ultimo parametro é que será passado para a thread;
    if(pthread_create(&thread, NULL, send_animation, NULL) != 0){
        printf("falha ao criar thread\n");
        return 1;
    } else {
        printf("thread criada com sucesso\n");
    }

    while(1){
        if(!render_screen_signal()){
            pthread_mutex_lock(&mutex);
            pause_thread = 1;
            while(render_screen_signal()){
                //fazer os calculos necessarios
            }
            pthread_mutex_unlock(&mutex);
        }
        pthread_mutex_lock(&mutex);
        pause_thread = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);


        //verificar se houve encerramento do jogo
        
    }

    pthread__cancel(thread);
    if (pthread_join(thread, NULL) != 0) {
        printf("falha ao esperar thread\n");
        return 1;
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Thread terminou\n");
    return 0;

}