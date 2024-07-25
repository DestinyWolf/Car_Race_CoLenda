#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include "display_7seg_driver.h"

#define DRIVER_PATH_7SEG "/dev/display_7seg"

#define OFF 10 //index at the segment code array

/* Letras*/
#define P 12 //0001100
#define A 8  //0001000
#define U 65 //1000001
#define S 18 //0010010
#define E 6  //0000110

/*
ZERO 64 //1000000
ONE 121 //1111001
TWO 36 //0100100
THREE 48 //0110000
FOUR 25 //0011001
FIVE 18 // 0010010
SIX  2 //0000010
SEVEN 120 //1111000
EIGHT 0 //0000000
NINE 16 //0010000
OFF 127 //1111111
*/
static const uint8_t segment_codes[11] = {64, 121, 36, 48, 25, 18, 2, 120, 0, 16, 127};


/* Identificadores dos displays*/
#define HEX0 0
#define HEX1 1
#define HEX2 2
#define HEX3 3
#define HEX4 4
#define HEX5 5

/* Variáveis globais*/
static int dev_7seg = -1;

/*Protótipos das funções*/
int display_open();
int display_write_digit(uint8_t, uint8_t);
int display_write_int(uint32_t);
int display_write_score(uint16_t, uint8_t);
int display_close();
void display_clear();
int display_write_letter(uint8_t *);



int display_open(){

    /* abrir o arquivo com permissão de escrita e caso exista, sobreescreve o arquivo */
    dev_7seg = open(DRIVER_PATH_7SEG, O_WRONLY);
    if (dev_7seg == -1) {
        printf("Failed to open file!\n");
        return -1;
    }
    return 0;
}

int display_write_digit(uint8_t hex, uint8_t data){
    /*Verificando limites de representação dos displays*/
    if(hex > 5 || hex < 0 || data > 10) return -1;
    
    struct ioctl_args args = {hex, segment_codes[data]};

    ioctl(dev_7seg, WR_VALUE, &args);
    return 0;
}

int display_write_int(uint32_t data){
    /* verificando limites de representação dos displays*/
    if(data > 999999) return -1;

    /*Escrevendo dados nos displays*/
    struct ioctl_args args;

    for (size_t i = 0; i < 6; i++){
        args.hex_id = i;
        args.data = segment_codes[data % 10];
        ioctl(dev_7seg, WR_VALUE, &args);
        data /= 10;
    }
    return 0;
}

void display_clear(){
    struct ioctl_args args;
    args.data = segment_codes[OFF];

    for (size_t i = 0; i < 6; i++){
        args.hex_id = i;
        ioctl(dev_7seg, WR_VALUE, &args);
    }
}

int display_write_score(uint16_t score, uint8_t player){ 
    if(score > 999) return -1;

    int display = player ? 3 : 0; //player -> 0 (player 1)

    for (size_t i = 0; i < 3; i++){
        display_write_digit(display++, (score % 10));
        score /= 10;
    }
         
    return 0;
}
int display_close(){
    /* caso haja algum erro ao encerrar a comunicação retorna -1 */ 
    if (close(dev_7seg) == -1) {
        printf("Failed to close file!\n");
        return -1;
    }
    return 0;
}

int display_write_word(uint8_t data[]){
    struct ioctl_args args;

    for(size_t i = 0; i<6; i++){
        args.hex_id = 5-i;
        args.data = data[i];
        ioctl(dev_7seg, WR_VALUE, &args);
    }
	return 0;
}
