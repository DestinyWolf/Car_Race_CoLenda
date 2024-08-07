/**
 * \file            display_7seg_driver.h
 * \brief           Arquivo de cabeçalho da biblioteca dos displays de 7 segmentos
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
 * This file is part of Super Auto.
 *
 * Authors:          Brenda Barbosa, Camila Queiroz, Maike e Oliveira
 */

#ifndef DISPLAY_7SEG_DRIVER

#include<linux/ioctl.h>

#define DISPLAY_7SEG_DRIVER

/*Definição da struct da dados a ser recebida do espaço de usuário*/
typedef struct {
  uint8_t hex_id;                              /*!< Identificação do display */
  uint8_t data;                                /*!< Código dos segmentos do dadoo a ser exibido */
} ioctl_args_t;

/* Definição dos comandos ioctl*/
#define WR_VALUE _IOW('a', 'a', ioctl_args_t *)

/*Nome do driver */
#define DRIVER_NAME "display_7seg_driver"

#endif