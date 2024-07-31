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

#ifndef COLENDA
#define COLENDA

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

/* offset de cada sprite */
#define ORANGE_BLOCK        0
#define BLUE_BLOCK          1
#define MEDAL               2
#define BOMB                3
#define TREE                4
#define SHIP_UP             5
#define SHIP_RIGHT          6
#define SHIP_DOWN           7
#define SHIP_LEFT           8
#define COIN                9
#define DIAMOND             10
#define LASER_VERTICAL      11
#define LASER_DIAGONAL1     12
#define LASER_HORIZONTAL    13
#define LASER_DIAGONAL2     14
#define HEART               15
#define LOG                 16
#define BLUE_CAR_F1            17
#define GREEN_CAR_F1           18
#define PURPLE_CAR_F1          19
#define YELLOW_CAR_F1          20
#define ALIEN1              21
#define ALIEN2              22
#define DRAGON              23
#define CUP                 24

/* Codigo das operações */
#define WBR 0 /* 0000 */
#define WSM 1 /* 0001 */
#define WBM 2 /* 0010 */
#define DP  3 /* 0011 */

/**
 * \brief           dados de uma cor
 * \note            essa estrutura é utilizada para separar os campos de uma cor
 */
typedef struct {
    uint64_t red;           /*!< Intensidade cor vermelha*/
    uint64_t green;         /*!< Intensidade cor verde*/
    uint64_t blue;          /*!< Intensidade cor azul*/ 
} color_t;

/**
 * \brief           Estrutura de um sprite
 * \note            Essa struct é utilizada para separar os campos que um sprite deve conter
*/
typedef struct {
    uint64_t coord_x;               /*!< Coordenada do eixo x do centro do sprite:        0 a 639*/
    uint64_t coord_y;               /*!< Coordenada do eixo y do centro do sprite:        0 a 479*/
    uint64_t offset;                /*!< Identificação do sprite:                         0 a 24 */ 
    uint64_t data_register;         /*!< Localização do sprite na memoria:                1 a 31 */
    uint64_t visibility;            /*!< Visibilidade do sprite: 0. invisivel         1. visivel */ 
    uint64_t speed;                 /*!< Velocidade que o sprite se move, 0 se for um sprite fixo*/
} sprite_t;

/**
 * \brief           Estrutura de um polígono
 * \note            Essa estrutura é utilizada para definir os campos de um polígono
 */
typedef struct {
    uint64_t coord_x;               /*!< Eixo X:                    0 a 511 */ 
    uint64_t coord_y;               /*!< Eixo Y:                    0 a 479 */ 
    uint64_t mem_address;           /*!< Número referente a camada: 0 a 15 */ 
    uint64_t shape;                 /*!< Forma do poligno: 0 . quadrado  1 . triangulo */
    uint64_t size;                  /*!< Tamanho do poligno:        0 a 15 */ 
    color_t color;                  /*!< Campo de cor do polígono */
} polygon_t;

/**
 * \brief           Estrutura de Bloco de BackGround 
 * \note            Estrutura utilizada para definir os campos de um bloco do background 
 */
typedef struct {
    uint64_t coord_x;           /*!< Endereço do bloco: 0 a 79 */ 
    uint64_t coord_y;           /*!< Endereço do bloco: 0 a 59 */ 
    color_t color;              /*!< Cor referente ao bloco */ 
} background_block_t;


typedef struct{
    uint64_t coord_x;           /*!< Endereço do bloco: 0 a 79      */ 
    uint64_t coord_y;           /*!< Endereço do bloco: 0 a 59      */ 
    color_t color;              /*!< Cor referente ao bloco         */ 
    uint64_t speed;             /*!< Velocidade que o bloco se move */
} background_block_movel_t;

/**
 * \brief           Estrutura de um pixel 
 * \note            Essa Struct é utilizada para definir os campos que um pixel deve conter
 */
typedef struct {
    uint64_t mem_address;           /*!< Endereço do pixel: 0 a 16383 */ 
    color_t color;                  /*!< Cor do pixel */
} pixel_t;

/**
 * \brief           Inicializa a comunicação com a GPU
 * \note            É necessario estar no começo do codigo antes da utilzação que qualquer outra função da GPU
 * \return          se a operação ocorreu ou não           
 */
int 
GPU_open();

/**
 * \brief           Alterar a cor do fundo da tela
 * \param[in]       color: cor que será aplicada
 * \return          se a operação ocorreu ou não
*/
int 
set_background_color(color_t color);

/**
 * \brief           Altera a cor ou desativa um bloco do fundo
 * \param[in]       bg_block: bloco que sera alterado
 * \return          se a operação ocorreu ou não
*/
int 
set_background_block(background_block_t bg_block);

/**
 * \brief           Altera a cor ou desativa um bloco do fundo do tipo movel
 * \param[in]       bg_block: bloco que sera alterado
 * \return          se a operação ocorreu ou não
*/
int 
set_background_block_movel(background_block_movel_t bg_block);

/**
 * \brief           Insere ou desabilita um sprite da tela
 * \param[in]       sprite: Sprite que será inserio/alterado
 * \return          se a operação foi bem sucedida ou não
*/
int 
set_sprite(sprite_t sprite);

/**
 * \brief           Insere ou altera um polígono da tela
 * \param[in]       polygon: polígono a ser inserido/alterado
 * \return          se a operação foi bem sucedida ou não
*/
int 
set_polygon(polygon_t polygon);

/**
 * \brief           Insere ou altera um pixel da memoria de sprites
 * \param[in]       pixel: pixel a ser alterado/inserido
 * \return          Se a operação foi bem sucedida ou não
*/
int 
set_pixel(pixel_t pixel);

/**
 * \brief            Remove/Desabilita os elementos que estão na tela e troca a cor do fundo para preto
 * \return           Se a operação foi bem sucedida ou não
*/
int 
clear();

/**
 * \brief           desenha uma linha horizontal usando blocos de background
 * \param[in]       size: tamanho da linha
 * \param[in]       coord_x: ponto inicial X
 * \param[in]       coord_y: ponto inicial Y
 * \param[in]       color: Cor da linha
 * \return          se a operação ocorreu com sucesso ou não
 */
int 
draw_horizontal_block_line(uint64_t size, uint64_t coord_x, uint64_t coord_y, color_t color);

/**
 * \brief           desenha uma linha vertical usando blocos de background
 * \param[in]       size: tamanho da linha
 * \param[in]       coord_x: ponto inicial X
 * \param[in]       coord_y: ponto inicial Y
 * \param[in]       color: Cor da linha
 * \return          se a operação ocorreu com sucesso ou não
 */
int 
draw_vertical_block_line(uint64_t size, uint64_t coord_x, uint64_t coord_y, color_t color);


int
create_sprite_by_matrix(color_t matrix[], uint64_t offset);

/**
 * \brief           desenha um bloco de background
 * \param[in]       coord_x: ponto inicial X
 * \param[in]       coord_y: ponto inicial Y 
 * \param[in]       color: Cor da linha  
 * \return          se a operação ocorreu com sucesso ou não    
*/
int 
draw_background_block(uint64_t coord_x, uint64_t coord_y, color_t color);

/**
 * \brief           converte 2 wchar_t em um vetor de 8 char
 * \param[in]       data_a: dados que vão para o barrramento A
 * \param[in]       data_b: dados que vão para o barramento B
 * \param[out]      retorno: vetor de 8 char
*/
void 
wchar_to_string(wchar_t data_a, wchar_t data_b, char* retorno);


/**
 * \brief           escreve no arquivo de comunicação entre a a biblioteca e a gpu
 * \param[in]       instruction_binary_string: bits a ser escritos no barramento
*/
void 
write_in_gpu(char* instruction_binary_string);

/**
 * \brief           encessa a comunicação entre a biblioteca e a gpu
 * \return          retorna erro ou sucesso
*/
int 
GPU_close();

#endif