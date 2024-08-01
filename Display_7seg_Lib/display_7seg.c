/**
 * \file            display_7seg.c
 * \brief           Arquivo source da biblioteca do display de 7 segmentos
 */

/*
 * Copyright (c) 2024 Brenda Barbosa, Camila Queiroz, Maike Nascimento
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of Super Auto.
 *
 * Authors:          Brenda Barbosa, Camila Queiroz, Maike de Oliveira
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include "display_7seg_driver.h"
#include "display_7seg.h"

static const uint8_t letters_segments_codes[8] = {12, 8, 65, 18, 6, 78, 7, 64};
/*
 * ZERO 64 //1000000
 * ONE 121 //1111001
 * TWO 36 //0100100
 * THREE 48 //0110000
 * FOUR 25 //0011001
 * FIVE 18 // 0010010
 * SIX  2 //0000010
 * SEVEN 120 //1111000
 * EIGHT 0 //0000000
 * NINE 16 //0010000
 * OFF 127 //1111111
*/
static const uint8_t numbers_segments_codes[11] = {64, 121, 36, 48, 25, 18, 2, 120, 0, 16, 127};


/* Variáveis globais*/
static int dev_7seg;

/*Protótipos das funções*/
int  display_open();
int  display_write_digit(const uint8_t, const uint8_t);
int  display_write_int(const uint32_t);
int  display_clear();
int  display_write_score(const uint16_t, const uint8_t);
int  display_write_word(const uint8_t []);
int  display_close();

int
display_open(){
    dev_7seg = -1;
    
    /* Abre o arquivo com permissão de escrita */
    dev_7seg = open(DRIVER_PATH_7SEG, O_WRONLY);
    
    if (dev_7seg == -1) {
        printf("Failed to open file!\n");
        return -1;
    }
    return 0;
}

int
display_write_digit(const uint8_t hex, const uint8_t data){
    ioctl_args_t args;
    int8_t ret;
    /*Verificando limites de representação dos displays*/
    if(hex > 5 || hex < 0 || data > 10){
        return -1;
    }
    
    /*Escrevendo dados nos displays*/
    args = (ioctl_args_t) {.hex_id = hex, .data = numbers_segments_codes[data]};

    ret = ioctl(dev_7seg, WR_VALUE, &args);
    
    return ret ? ret : 0;
}

int
display_write_int(const uint32_t data){
    ioctl_args_t args;
    uint32_t value;

    /* Verificando limites de representação dos displays*/
    if(data > 999999){
        return -1;
    }

    value = data;
    /*Escrevendo dados nos displays*/
    for (size_t i = 0; i < 6; ++i){
        args = (ioctl_args_t) {.hex_id = i, .data = numbers_segments_codes[value % 10]};
        if(ioctl(dev_7seg, WR_VALUE, &args) == -1){
            return -1;
        }
        value /= 10;
    }
    return 0;
}

int
display_clear(){
    ioctl_args_t args;
    args.data = numbers_segments_codes[OFF_DISPLAY];
    
    /*Apagando todos os segmentos de cada um dos displays*/
    for (size_t i = 0; i < 6; ++i){
        args.hex_id = i;
        if(ioctl(dev_7seg, WR_VALUE, &args) == -1){
            return -1;
        }
    }
    return 0;
}

int
display_write_score(const uint16_t score, const uint8_t player){ 
    uint16_t value;
    uint8_t display;

    /* Verificando limite de representaão dos displays */
    if(score > 999){
        return -1;
    }

    value = score;
    display = player ? 3 : 0;              /* player = 0 -> exibe nos displays 0 a 2 */

    for (size_t i = 0; i < 3; ++i){
        if(display_write_digit(display++, (value % 10)) == -1){
            return -1;
        }
        value /= 10;
    }
    return 0;
}

int
display_write_word(const uint8_t data[]){
    ioctl_args_t args;

    for(size_t i = 0; i < 6; ++i){
        args = (ioctl_args_t) {
            .hex_id = (5-i),
            .data = (data[i] == OFF_DISPLAY) ? numbers_segments_codes[data[i]] : letters_segments_codes[data[i]]
        };
        if(ioctl(dev_7seg, WR_VALUE, &args)){
            return -1;
        }
    }
	return 0;
}

int 
display_animation(const uint8_t data[]){

    
    int letters[12] = {OFF_DISPLAY, OFF_DISPLAY, OFF_DISPLAY, OFF_DISPLAY, OFF_DISPLAY, OFF_DISPLAY,
    data[0], data[1], data[2], data[3], data[4], data[5]};

    int *ptr[12];
    int *temp;
    int rotate_index = 0;
    ioctl_args_t args;

    for(size_t i = 0; i < 12; ++i){
        ptr[i] = &letters[i];
    }
    for(size_t a = 0; a<12; ++a){

    for(size_t i = 0; i < 11; ++i){
        temp = ptr[rotate_index];
        ptr[rotate_index] =ptr[(rotate_index + 1) % 12];
        ptr[(rotate_index + 1) % 12] = temp;
        rotate_index = (rotate_index + 1) % 12;
    }

    for(size_t k = 0; k < 6; k++){
        args.hex_id = 5-k;
	args.data = (*ptr[k] == OFF_DISPLAY) ? numbers_segments_codes[*ptr[k]] : letters_segments_codes[*ptr[k]];
        ioctl(dev_7seg, WR_VALUE, &args);
    }
	usleep(250000);
}

    return 0;
}

int 
display_title_animation(){

    
    int letters[16] = {OFF_DISPLAY, OFF_DISPLAY, OFF_DISPLAY, OFF_DISPLAY, OFF_DISPLAY, OFF_DISPLAY,
    S_DISPLAY, U_DISPLAY, P_DISPLAY, E_DISPLAY, R_DISPLAY, OFF_DISPLAY,A_DISPLAY, U_DISPLAY, T_DISPLAY, O_DISPLAY};

    int *ptr[16];
    int *temp;
    int rotate_index = 0;
    ioctl_args_t args;

    for(size_t i = 0; i < 16; ++i){
        ptr[i] = &letters[i];
    }
    for(size_t a = 0; a<16; ++a){

    for(size_t i = 0; i < 15; ++i){
        temp = ptr[rotate_index];
        ptr[rotate_index] =ptr[(rotate_index + 1) % 16];
        ptr[(rotate_index + 1) % 16] = temp;
        rotate_index = (rotate_index + 1) % 16;
    }

    for(size_t k = 0; k < 6; k++){
        args.hex_id = 5-k;
	args.data = (*ptr[k] == OFF_DISPLAY) ? numbers_segments_codes[*ptr[k]] : letters_segments_codes[*ptr[k]];
        ioctl(dev_7seg, WR_VALUE, &args);
    }
	usleep(250000);
}

    return 0;
}

int
display_close(){
    /* caso haja algum erro ao encerrar a comunicação, retorna -1 */ 
    if (close(dev_7seg) == -1) {
        printf("Failed to close file!\n");
        return -1;
    }
    return 0;
}
