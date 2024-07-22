#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include "display_7seg_driver.h"

#define DRIVER_PATH "/dev/display_7seg"

#define OFF 127 //1111111
#define ZERO 64 //1000000
#define ONE 121 //1111001
#define TWO 36 //0100100
#define THREE 48 //0110000
#define FOUR 25 //0011001
#define FIVE 18 // 0010010
#define SIX  2 //0000010
#define SEVEN 120 //1111000
#define EIGHT 0 //0000000
#define NINE 16 //0010000

/* Identificadores dos displays*/
#define HEX0 0
#define HEX1 1
#define HEX2 2
#define HEX3 3
#define HEX4 4
#define HEX5 5

/* Variáveis globais*/
static int dev = -1;

/*Protótipos das funções*/
int display_open();
int display_write(uint8_t, uint8_t);
int display_write_all(uint8_t*);
int display_close();
void display_clear();

int display_open(){

    /* abrir o arquivo com permissão de escrita e caso exista, sobreescreve o arquivo */
    dev = open(DRIVER_PATH, O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dev == -1) {
        printf("Failed to open file!\n");
        return -1;
    }
    return 0;
}

int display_write(uint8_t hex, uint8_t data){
    /*Verificando limites de representação dos displays*/
    if(hex > 5 || hex < 0 || data > 127) return -1;
    
    struct ioctl_args args = {hex, data};

    ioctl(dev, WR_VALUE, &args);
    return 0;
}

int display_write_all(uint8_t *data){
    /* erificando limites de representação dos displays*/
    if(data[0]+data[1]+data[2]+data[3]+data[4]+data[5] > 762) return -1;
    
    /*Escrevendo dados nos displays*/
    struct ioctl_args args;

    for (size_t i = 0; i < 6; i++)
    {
        args.hex_id = i;
        args.data = data[i];
        ioctl(dev, WR_VALUE, &args);
    }

    return 0;
}

void display_clear(){
    struct ioctl_args args;
    args.data = OFF;

    for (size_t i = 0; i < 6; i++)
    {
        args.hex_id = i;
        ioctl(dev, WR_VALUE, &args);
    }
}
int display_close(){
    /* caso haja algum erro ao encerrar a comunicação retorna 0 */ 
    if (close(dev) == -1) {
        printf("Failed to close file!\n");
        return -1;
    }
    return 0;
}

static int int2segcode(uint8_t value){
    int seg_code = OFF;

    switch (value)
    {
    case 0:
       value = ZERO;
        break;
    case 1:
       value = ONE;
        break;
    case 2:
       value = TWO;
        break;
    case 3:
       value = THREE;
        break;
    case 4:
       value = FOUR;
        break;
    case 5:
       value = FIVE;
        break.
    case 6:
       value = SIX;
        break;
    case 7:
       value = SEVEN;
        break;
    case 8:
       value = EIGHT;
        break;
    case 9:
       value = NINE;
        break;
    }

    return seg_code;
}

int display_write_int(int score, int player){

    if(score > 999) return -1;

    uint8_t data[3] = {int2segcode(score / 100), int2segcode((score % 100) / 10), int2segcode(score % 10)};

    if(player == 1){
        display_write(HEX2, data[0]);
        display_write(HEX1, data[1]);
        display_write(HEX0, data[2]);
    }else{
        display_write(HEX5, data[0]);
        display_write(HEX4, data[1]);
        display_write(HEX3, data[2]);
    }
    return 0;
}