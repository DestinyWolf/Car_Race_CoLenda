#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include "mouse_module.h"


struct input_event ev1; /*struct do tipo input_event para armazenar os dados do arquivo do 1º mouse*/
struct input_event ev2; /*struct do tipo input_event para armazenar os dados do arquivo do 2º mouse*/

int x_mouse_1, x_mouse_2, key_press_1, key_press_2; /* variaveis dos campos lidos   */
int fd_mouse_1, fd_mouse_2;                         /* apontadores para os arquivos */

int read_mouse_1_event(int* key, int* coord_x) {

    ssize_t n = read(fd_mouse_1, &ev1, sizeof(ev1)); /*realiza a leitura do arquivo do mouse 1*/

        if (n == (ssize_t)-1) { /*caso o retorno tenha sido -1*/
            printf("erro ao ler /dev/input/event0\n"); /*imprime que houve um erro ao ler o arquivo*/
            return 0; /*retorna 0, indicando falha*/
        }

        /*caso seja um evento de movimentação*/
        if(ev1.type == EV_REL){
            /*no eixo x*/
            if (ev1.code == REL_X) {
                
                /*mascara para traduzir a aceleração do mouse em valores validos para o jogo*/
                if(ev1.value > 5) {
                    x_mouse_1 = 2;
                    key_press_1 = 0;
                } else if(ev1.value > 0) {
                    x_mouse_1 = 1;
                    key_press_1 = 0;
                } else if (ev1.value < -5){
                    x_mouse_1 = -2;
                    key_press_1 = 0;
                } else if (ev1.value < 0) {
                    x_mouse_1 = -1;
                    key_press_1 = 0;
                } else {
                    x_mouse_1 = 0;
                    key_press_1 = 0;
                }

            }
        } 
        /*caso seja o pressionamento de uma tecla*/
        else if (ev1.type == EV_KEY) {
            /*verifica se é a tecla esquerda*/
            if (ev1.code == BTN_LEFT) {
                if (ev1.value) {
                    key_press_1 = 1;
                    x_mouse_1 = 0;
                } else {
                    key_press_1 = 0;
                    x_mouse_1 = 0;
                }
            } 
        }

        /*atribui aos parametros de retorno, os valores lidos*/
        *coord_x = x_mouse_1;
        *key = key_press_1;

        /*retorna 1, indicando que houve sucesso na leitura*/
        return 1;
    
}

int read_mouse_2_event(int* key, int* coord_x) {
    ssize_t n = read(fd_mouse_2, &ev2, sizeof(ev2));

        if (n == (ssize_t)-1){
            printf("erro ao ler /dev/input/event1\n"); /*informa que houve um erro ao ler o arquivo do mouse 2*/
            return 0;
        }

        if(ev2.type == EV_REL){
            if (ev2.code == REL_X) {
                if(ev2.value > 5) {
                    x_mouse_2 = 2;
                    key_press_2 = 0;
                } else if(ev2.value > 0) {
                    x_mouse_2 = 1;
                    key_press_2 = 0;
                } else if (ev2.value < -5){
                    x_mouse_2 = -2;
                    key_press_2 = 0;
                } else if (ev2.value < 0) {
                    x_mouse_2 = -1;
                    key_press_2 = 0;
                } else {
                    x_mouse_2 = 0;
                    key_press_2 = 0;
                }

            } 
        } else if (ev2.type == EV_KEY) {
            if (ev2.code == BTN_LEFT) {
                if (ev2.value) {
                    key_press_2 = 1;
                    x_mouse_2 = 0;
                } else {
                    key_press_2 = 0;
                    x_mouse_2 = 0;
                }
            } 
        }

        /*altera o valor dos parametros de retorno*/
        *coord_x = x_mouse_2;
        *key = key_press_2;
        return 1;
    
}

int module_init_mouse_1() {
    x_mouse_1 = 0;
    key_press_1 = 0;

    /*realiza a abertura do arquivo no modo leitura apenas*/
    fd_mouse_1 = open("/dev/input/event0", O_RDONLY);
    if (fd_mouse_1 == -1) { /*caso retorne -1, indica que houve problema na abertura do arquivo do mouse 1*/
        printf("erro ao abrir /dev/input/event0\n");
        return 0; 
    }
    return 1;
}

int module_exit_mouse_1() {
    close(fd_mouse_1); /* finaliza a comunicação com o arquivo do mouse 1*/
    return 1;
}

int module_init_mouse_2() {
    x_mouse_2 = 0;
    key_press_2 = 0;

    fd_mouse_2 = open("/dev/input/event1", O_RDONLY); /*abre o arquivo do mouse 2 no modo apenas leitura*/
    if (fd_mouse_2 == -1) { /*caso retorne -1, indica que houve um erro ao tentar abrir o arquivo do mouse 2*/
        printf("erro ao abrir /dev/input/event1\n");
        return 0;
    }
    return 1;
}

int module_exit_mouse_2() {
    close(fd_mouse_2); /*encerra a comunicação com o arquivo do segundo mouse*/
    return 1;
}