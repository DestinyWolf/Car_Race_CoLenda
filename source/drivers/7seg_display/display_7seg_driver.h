/**
 * \file            colenda.h
 * \brief           arquivo de cabeçalho da biblioteca colenda
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
 * Authors:          Brenda Barbosa, Camila Queiroz, Maike e Oliveira
 */

#ifndef DISPLAY_7SEG_DRIVER

#include<linux/ioctl.h>

#define DISPLAY_7SEG_DRIVER

/*Definição da struct da dados a ser recebida do espaço de usuário*/
struct ioctl_args{
  uint8_t hex_id;
  uint8_t data;
};

/* Definição dos comandos ioctl*/
#define WR_VALUE _IOW('a', 'a', ioctl_args *)

/* Endereços de memória para mapeamento dos displays de 7 segmentos*/
#define HEX5_BASE 0x10
#define HEX4_BASE 0x20
#define HEX3_BASE 0x30
#define HEX2_BASE 0x40
#define HEX1_BASE 0x50
#define HEX0_BASE 0x60

/*Nome do driver */
#define DRIVER_NAME "display_7seg_driver"

#endif