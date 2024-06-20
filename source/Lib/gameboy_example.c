#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "colenda.h"

int main() {
    GPU_open();

    //limpa a tela e remove todos os elementos armazenados anteriormente
    clear();

    //cores
    color_t black = {0, 0, 0};
    color_t blue = {0, 0, 3};
    color_t white = {7,7,7};
    color_t red = {7, 0, 0};

    //muda a cor do fundo para brando
    set_background_color(white);


    //desenha a borda superior externa
    for (int i = 6; i < 9; i++)
    {
        draw_horizontal_block_line(60, 10, i, red);
    }

    //desenha a borda superior interna
    for (int i = 9; i < 12; i++)
    {
        draw_horizontal_block_line(60, 10, i, black);
    }

    //lateral esquerda exterior
    for (int i = 7; i < 10; i++)
    {
        draw_vertical_block_line(51, i, 9, red);
    }

    //borda interior esquerda
    for (int i = 10; i < 13; i++)
    {
        draw_vertical_block_line(33, i, 12, black);
    }

    //muda a cor da tela interna para azul
    for(int i = 12; i<42; i++){
        draw_horizontal_block_line(54, 13, i, blue);
    }

    //borda interior direita
    for (int i = 67; i < 70; i++)
    {
        draw_vertical_block_line(33, i, 12, black);
    }

    //borda inferior interna
    for (int i = 42; i < 45; i++)
    {
        draw_horizontal_block_line(60, 10, i, black);
    }
    
    //parte inferior do gameboy
    for(int i = 45; i<60; i++){
        draw_horizontal_block_line(60, 10, i, red);
    }

    //lateral direita exterior
    for (int i = 70; i < 73; i++)
    {
        draw_vertical_block_line(51, i, 9, red);
    }
    

    //desenho dos botões
    polygon_t btn_quadrado;
    polygon_t btn_triangulo;
    btn_quadrado.color = black;
    btn_triangulo.color = black;
    btn_quadrado.mem_address = 0;
    btn_triangulo.mem_address = 1;
    btn_quadrado.coord_x = 405;
    btn_quadrado.coord_y = 480;
    btn_triangulo.coord_x = 475;
    btn_triangulo.coord_y = 418;
    btn_triangulo.shape = 1;
    btn_quadrado.shape = 0;
    btn_quadrado.size = 5;
    btn_triangulo.size = 5;
    
    //desenha os botões internos
    set_polygon(btn_quadrado);
    set_polygon(btn_triangulo);


    //desenha os direcionais
    for (int i = 18; i < 24; i++)
    {
        draw_vertical_block_line(4, i, 53, black);
    }
    
    for(int i = 57; i < 60; i++){
        draw_horizontal_block_line(16, 13, i, black);
    }

    //ciração dos sprites exibidos na tela interna
    sprite_t alien1 = {218, 126, ALIEN1, 1, 1};
    sprite_t alien2 = {238, 146, ALIEN2, 2, 1};
    sprite_t alien3 = {258, 166, ALIEN1, 3, 1};
    sprite_t alien4 = {278, 186, ALIEN2, 4, 1};
    sprite_t alien5 = {298, 186, ALIEN1, 5, 1};
    sprite_t alien6 = {318, 166, ALIEN2, 6, 1};
    sprite_t alien7 = {338, 146, ALIEN1, 7, 1};
    sprite_t alien8 = {358, 126, ALIEN2, 8, 1};
    sprite_t nave = {318, 300, SHIP_UP, 9, 1};
    sprite_t vida1 = {440, 106, HEART, 10, 1};
    sprite_t vida2 = {470, 106, HEART, 11, 1};
    sprite_t vida3 = {500, 106, HEART, 12, 1};
    sprite_t laser1 = {258, 256, LASER_VERTICAL, 13, 1};
    sprite_t laser2 = {358, 244, LASER_VERTICAL, 14, 1};
    sprite_t laser3 = {318, 280, LASER_VERTICAL, 15, 1};
    sprite_t laser4 = {218, 186, LASER_VERTICAL, 16, 1};

    //desenha os sprites fixos 
    set_sprite(nave);
    set_sprite(vida1);
    set_sprite(vida2);
    set_sprite(vida3);
    set_sprite(laser1);
    set_sprite(laser2);
    set_sprite(laser3);
    set_sprite(laser4);

    //animação de troca de sprite dos aliens
    while (1)
    {
        //desenha os aliens
        set_sprite(alien1);
        set_sprite(alien2);
        set_sprite(alien3);
        set_sprite(alien4);
        set_sprite(alien5);
        set_sprite(alien6);
        set_sprite(alien7);
        set_sprite(alien8);

        //troca o sprite de cada um
        alien1.offset = ALIEN2;
        alien2.offset = ALIEN1;
        alien3.offset = ALIEN2;
        alien4.offset = ALIEN1;
        alien5.offset = ALIEN2;
        alien6.offset = ALIEN1;
        alien7.offset = ALIEN2;
        alien8.offset = ALIEN1;
        
        //aguarda 500 ms
        usleep(500000);

        //desenha os prites novamente
        set_sprite(alien1);
        set_sprite(alien2);
        set_sprite(alien3);
        set_sprite(alien4);
        set_sprite(alien5);
        set_sprite(alien6);
        set_sprite(alien7);
        set_sprite(alien8);

        //troca os sprites novamente
        alien1.offset = ALIEN1;
        alien2.offset = ALIEN2;
        alien3.offset = ALIEN1;
        alien4.offset = ALIEN2;
        alien5.offset = ALIEN1;
        alien6.offset = ALIEN2;
        alien7.offset = ALIEN1;
        alien8.offset = ALIEN2;

        //aguarda 500 ms
        usleep(500000);
    }

    GPU_close();
    return 0;
}
