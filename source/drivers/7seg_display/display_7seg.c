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

#define OFF 0 //0000000
#define ZERO 126 //1111110
#define ONE 48 //0110000
#define TWO 109 //1101101
#define THREE 121 //1111001
#define FOUR 51 //0110011
#define FIVE 91 //1011011
#define SIX  95 //1011111
#define SEVEN 112 //1110000
#define EIGHT 127 //1111111
#define NINE 123 //1111011

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
int display_write_all(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
int display_close();
int display_clear();

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

