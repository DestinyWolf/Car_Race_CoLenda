/**
 * \file            display_7seg.h
 * \brief           Arquivo de cabeçalho da biblioteca do display de 7 segmentos
 */

/*
 * Copyright (c) 2024 Brenda Babosa, Camila queiroz, Maike Nascimento
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
 * This file is part of Colenda.
 *
 * Authors:          Brenda Barbosa, Camila Boa Morte, Maike e Oliveira
 */


#ifndef DISPLAY_7SEG
#define DISPLAY_7SEG

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

/* Código dos segmentos de cada letra*/
#define P 12 //0001100
#define A 8  //0001000
#define U 65 //1000001
#define S 18 //0010010
#define E 6  //0000110

/*Códigos dos segmentos de cada dígito
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

#define OFF 10 //index do off no array segment_codes

/* Identificadores dos displays*/
#define HEX0 0
#define HEX1 1
#define HEX2 2
#define HEX3 3
#define HEX4 4
#define HEX5 5


/**
 * \brief           Inicia a comunicação com o driver dos botões
 * \return          Retorna 0 caso a operação seja bem sucedida. Caso contrário, retorna -1
*/
int
display_open();

/**
 * \brief           Exibe um dígito em um dos displays
 * \param[in]       hex: display a ser escrito
 * \param[in]       data: dígito a ser exibido. De 0 a 9 ou OFF
 * \return          Retorna 0 caso a operação seja bem sucedida. Caso contrário, retorna EINVAL
*/
int
display_write_digit(uint8_t hex, uint8_t data);

/**
 * \brief           Exibe um inteiro nos displays
 * \param[in]       data: inteiro a ser exibido. De 0 a 999999
 * \return          Retorna 0 caso a operação seja bem sucedida. Caso contrário, retorna EINVAL
*/
int display_write_int(uint32_t data);

/**
 * \brief           Apaga todos os display
*/
void
display_clear();

/**
 * \brief          Exibe o score do jogador.
 * \param[in]      score: inteiro a ser exibido. De 0 a 999
 * \param[in]      player: indica em quais displays exibir. Caso o valor for 0, exibe a pontuação
 * nos displays 0 a 2. Caso contrário, exibe nos displays 3 a 5.
 * \return          Retorna 0 caso a operação seja bem sucedida. Caso contrário, retorna EINVAL
*/
int
display_write_score(uint16_t score, uint8_t player);

/**
 * \brief          Exibe uma palavra nos displays.
 * \param[in]      data: array com as constantes das letras a serem exibidas
 * \return         Retorna 0 caso a operação seja bem sucedida. Caso contrário, retorna -1
*/

int
display_write_word(uint8_t data[]);


/**
 * \brief           Encerra a comunicação com o driver dos botões.
 * \return          Retorna 0 caso a operação seja bem sucedida. Caso contrário, retorna -1
*/
int display_close();

#endif