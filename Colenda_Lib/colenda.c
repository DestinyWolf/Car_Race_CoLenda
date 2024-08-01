/**
 * \file            colenda.c
 * \brief           arquivo souce da biblioteca colenda
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
 * This file is part of Colenda.
 *
 * Authors:          Brenda Barbosa, Camila Queiroz, Maike de Oliveira
 */

#include "colenda.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <wchar.h>
#include <stdint.h>

int dev, inst_count;

int 
GPU_open() {
    inst_count = 0;

    /* abrir o arquivo com permissão de escrita e caso exista, sobreescreve o arquivo */
    dev = open("/dev/colenda", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dev == -1) {
        printf("Fail to open file");
        return 0;
    }
    return 1;
}

int 
set_background_color(color_t color) {
    char instruction_to_driver[8] = {0};            /* string que guarda a instrução a ser escrita no arquivo de comunicação com a GPU */
    wchar_t data_a, data_b;

    /* validação dos valores inseridos pelo usuario  */
    if ((color.red > 7 || color.green > 7 || color.blue > 7) || (color.red < 0 || color.green < 0 || color.blue < 0)) {
        printf("valor fora do limite de representação\n");
        return 0;
    }

    data_a = (0b00000 << 4) | WBR;
    data_b = (color.blue << 6) | (color.green << 3) | color.red;


    /* converte os wchar_t em um buffer de 8 char */
    wchar_to_string(data_a, data_b, instruction_to_driver);

    /* coloca o ponteiro de escrita no arquivo no começo do documento  */ 
    lseek(dev, 0, SEEK_SET);

    /* escrita da instrução no arquivo de comunicação com a gpu */ 
    write_in_gpu(instruction_to_driver);

    return 1;
}

int 
set_background_block(background_block_t bg_block) {

    char instruction_to_driver[8] = {0};            /* string que guarda a instrução a ser escrita no arquivo de comunicação com a GPU  */ 
    uint64_t mem_adress;
    wchar_t data_a, data_b;

    /* validação dos valores inseridos pelo usuario  */
    if ((bg_block.color.blue > 7 || bg_block.color.green > 7 || bg_block.color.red > 7 || bg_block.coord_x > 79 || bg_block.coord_y > 59)) {
        printf("valor fora do limite de representação\n");
        return 0;
    }
    
    /* calcula o endereço do bloco */
    mem_adress = (bg_block.coord_y * 80) + bg_block.coord_x;

    data_a = (mem_adress << 4) | WBM;
    data_b = (bg_block.color.blue << 6) | (bg_block.color.green << 3) | bg_block.color.red;

    /* converte os wchar_t em um buffer de 8 char */
    wchar_to_string(data_a, data_b, instruction_to_driver);

    /* coloca o ponteiro de escrita no arquivo no começo do documento */ 
    lseek(dev, 0, SEEK_SET);

    /* escrita da instrução no arquivo de comunicação com a gpu */ 
    write_in_gpu(instruction_to_driver);
    return 1;
}

int set_background_block_movel(background_block_movel_t bg_block)
{
    char instruction_to_driver[8] = {0};            /* string que guarda a instrução a ser escrita no arquivo de comunicação com a GPU  */ 
    uint64_t mem_adress;
    wchar_t data_a, data_b;

    /* validação dos valores inseridos pelo usuario  */
    if ((bg_block.color.blue > 7 || bg_block.color.green > 7 || bg_block.color.red > 7 || bg_block.coord_x > 79 || bg_block.coord_y > 59)) {
        printf("valor fora do limite de representação\n");
        return 0;
    }
    
    /* calcula o endereço do bloco */
    mem_adress = (bg_block.coord_y * 80) + bg_block.coord_x;

    data_a = (mem_adress << 4) | WBM;
    data_b = (bg_block.color.blue << 6) | (bg_block.color.green << 3) | bg_block.color.red;

    /* converte os wchar_t em um buffer de 8 char */
    wchar_to_string(data_a, data_b, instruction_to_driver);

    /* coloca o ponteiro de escrita no arquivo no começo do documento */ 
    lseek(dev, 0, SEEK_SET);

    /* escrita da instrução no arquivo de comunicação com a gpu */ 
    write_in_gpu(instruction_to_driver);

    return 1;
}

int 
set_sprite(sprite_t sprite) {
    char instruction_to_driver[8] = {0};            /* string que guarda a instrução a ser escrita no arquivo de comunicação com a GPU  */
    wchar_t data_a, data_b;

    /* validação dos valores inseridos pelo usuario  */
    if ((sprite.visibility > 1 || sprite.coord_x > 639 || sprite.coord_y > 479 || sprite.offset > 40 || sprite.data_register > 32) || sprite.data_register < 1) {
        // printf("valor fora do limite de representação\n");
        return 0;
    }

    data_a = (sprite.data_register << 4) | WBR;
    data_b = (sprite.visibility << 29) | (sprite.coord_x << 19) | (sprite.coord_y << 9) | sprite.offset;

    /* converte os wchar_t em um buffer de 8 char */
    wchar_to_string(data_a, data_b, instruction_to_driver);

    /* coloca o ponteiro de escrita no arquivo no começo do documento  */
    lseek(dev, 0, SEEK_SET);

    /* escrita da instrução no arquivo de comunicação com a gpu */ 
    write_in_gpu(instruction_to_driver);

    return 1;
}

int 
set_polygon(polygon_t polygon) {
    char instruction_to_driver[8] = {0};            /* string que guarda a instrução a ser escrita no arquivo de comunicação com a GPU  */
    wchar_t data_a, data_b;

    /* validação dos valores inseridos pelo usuario  */
    if ((polygon.shape > 1 || polygon.color.blue > 7 || polygon.color.green > 7 || polygon.color.red > 7 || polygon.size > 15 || polygon.coord_y > 479 || polygon.coord_x > 511 || polygon.mem_address > 15)) {
        printf("valor fora do alcance de representação\n");
        return 0;
    }

    if (((polygon.shape + 1) * 10) / 2 > polygon.coord_x || ((polygon.shape + 1) * 10) / 2 > polygon.coord_y){
        printf("posição invalida para triangulos, por favor informar um numero maior\n");
        return 0;
    }

    data_a = (polygon.mem_address << 4) | DP;
    data_b = (polygon.shape << 31) | (polygon.color.blue << 28) | (polygon.color.green << 25) | (polygon.color.red << 22) | (polygon.size << 18) | (polygon.coord_y << 9) | polygon.coord_x;

    /* converte os wchar_t em um buffer de 8 char */
    wchar_to_string(data_a, data_b, instruction_to_driver);

    /* coloca o ponteiro de escrita no arquivo no começo do documento  */
    lseek(dev, 0, SEEK_SET);

    /* escrita da instrução no arquivo de comunicação com a gpu */ 
    write_in_gpu(instruction_to_driver);

    return 1;
}

int 
set_pixel(pixel_t pixel) {
    char instruction_to_driver[8] = {0};            /* string que guarda a instrução a ser escrita no arquivo de comunicação com a GPU  */
    wchar_t data_a, data_b;

    /* validação dos valores inseridos pelo usuario  */
    if ((pixel.color.blue > 7 || pixel.color.green > 7 || pixel.color.red > 7 || pixel.mem_address > 16383))
    {
        printf("valor fora do limite de representação\n");
        return 0;
    }

    data_a = (pixel.mem_address << 4) | WSM;
    data_b = (pixel.color.blue << 6) | (pixel.color.green << 3) | (pixel.color.red);

    /* converte os wchar_t em um buffer de 8 char */
    wchar_to_string(data_a, data_b, instruction_to_driver);

    /* coloca o ponteiro de escrita no arquivo no começo do documento  */
    lseek(dev, 0, SEEK_SET);

    /* escrita da instrução no arquivo de comunicação com a gpu */ 
    write_in_gpu(instruction_to_driver);
    return 1;
}

int 
clear() {
    size_t i;

    /* Cria um sprite invisivel */
    sprite_t disable_sprite = {
        .coord_x = 0,
        .coord_y = 0,
        .offset = 0,
        .visibility = 0,
    };
    /* Cria um polígono desabilitado*/
    polygon_t disable_polygon = {
    .coord_x = 320,
    .coord_y = 240,
    .shape = 0,
    .size = 0,
    .color = { 
        .blue = 0,
        .green = 0,
        .red = 0
        }
    };

    /* Cria um bloco com a cor invisivel */
    background_block_t bg_disble_block;
    bg_disble_block.color.blue = 7;
    bg_disble_block.color.green = 7;
    bg_disble_block.color.red = 6;


    /* Cor preta para o background */
    color_t bg_black_color = {
        .blue = 0,
        .green = 0,
        .red = 0
    };

    /* Passa por todos os registradores de sprite */
    for (i = 1; i < 32; ++i) {
        disable_sprite.data_register = i;
        /* insere um sprite desabilitado */
        set_sprite(disable_sprite);
    }

    /* passa por todos os endereços */
    for (i = 0; i < 16; ++i) {
        disable_polygon.mem_address = i;

        /* escreve um poligno desabilitado */ 
        set_polygon(disable_polygon);
    }

    for (i = 0; i < 60; ++i) {
        bg_disble_block.coord_y = i;
        for (size_t j = 0; j < 80; ++j) {
            bg_disble_block.coord_x = j;
            set_background_block(bg_disble_block);
        }
    }

    /* muda a cor do fundo para preto */ 
    set_background_color(bg_black_color);

    return 1;
}

int 
draw_horizontal_block_line(uint64_t size, uint64_t coord_x, uint64_t coord_y, color_t color) {
    background_block_t bg_block = {
        .coord_y = coord_y,
        .color = color,
    };
    bg_block.color = color;
    bg_block.coord_y = coord_y;

    /* validação dos valores inseridos pelo usuario  */
    if (coord_x > 79 || coord_y > 59 || coord_x + size > 80) {
        printf("valor fora do limite de representação");
        return 0;
    }

    for (size_t i = coord_x; i < (coord_x + size); ++i) {
        bg_block.coord_x = i;
        set_background_block(bg_block);
    }
    return 1;
}

int 
draw_vertical_block_line(uint64_t size, uint64_t coord_x, uint64_t coord_y, color_t color) {
    background_block_t bg_block = {
        .coord_x = coord_x,
        .color = color,
    };

    /* validação dos valores inseridos pelo usuario  */
    if (coord_x > 79 || coord_y > 59 || coord_y + size > 60) {
        printf("valor fora do limite de representação");
        return 0;
    }

    for (size_t i = coord_y; i < (coord_y + size); ++i) {
        bg_block.coord_y = i;
        set_background_block(bg_block);
    }
    return 1;
}

int 
create_sprite_by_matrix(color_t matrix[], uint64_t offset) {
    size_t index_pixel = (offset*400);

    pixel_t pixel;

    for (size_t i = 0; i < 400; i++, index_pixel++)
    {
        pixel.color = matrix[i];
        pixel.mem_address = index_pixel;
        set_pixel(pixel);
    }
    
    return 1;
}

int 
draw_background_block(uint64_t coord_x, uint64_t coord_y, color_t color) {
    background_block_t bg_block = {
        .coord_x = coord_x, 
        .coord_y = coord_y, 
        .color = color,
        };
    set_background_block(bg_block);
    return 1;
}

void 
wchar_to_string(wchar_t data_a, wchar_t data_b, char* retorno) {
    for (int i = 0; i < 4; i++) {
        retorno[i] = (data_b >> (8 * (3 - i))) & 0xFF;
        retorno[i + 4] = (data_a >> (8 * (3 - i))) & 0xFF;
    }
}

void 
write_in_gpu(char* instruction_binary_string) {
    ssize_t bytes_written;
    bytes_written = write(dev, instruction_binary_string, 8);
    while (bytes_written == -1) {
        bytes_written = write(dev, instruction_binary_string, 8);
    }
}

int 
GPU_close() {

    /* caso haja algum erro ao encerrar a comunicação retorna 0 */ 
    if (close(dev) == -1) {
        printf("não foi possivel encerrar o arquivo\n");
        return 0;
    }
    return 1;
}