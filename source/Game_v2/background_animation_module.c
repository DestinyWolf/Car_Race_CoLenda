#include "background_animation_module.h"
#include <unistd.h>
#include <stdio.h>
#include "../Lib/Colenda_Lib/colenda.h"

//cores do fundo 
//estrutura da cor verde
color_t green = {
    .blue = 2,
    .green = 6,
    .red = 2,
};

//estrutura da cor cinza
color_t cinza = {
    .blue = 4,
    .green = 4,
    .red = 4,
};

//estrutura da cor branca
color_t white = {
    .blue = 6,
    .green = 6,
    .red = 6,
};

//estrutura da cor preta
color_t black = {
    .blue = 0,
    .green = 0,
    .red = 0,
};

//estrutura da cor invisivel
color_t invisible = {
    .blue = 7,
    .green = 7,
    .red = 6,
};

int frame; //controla o quadro que esta sendo exibido no momento

int bg_animation_module_init() {
    
    int i, j;
    frame = 0;

    set_background_color(cinza);
    //desenha as bordas verdes da tela
    for (i = 0; i < 12; i ++) {
        draw_vertical_block_line(60, i, 0, green);
        draw_vertical_block_line(60, i+68, 0, green);
    }


    
        //desenha a linha verde do meio da tela
        for(i = 38; i < 43; i++){
            draw_vertical_block_line(60, i, 0, green);
        }

        //desenhas as linhas pretas da tela
        draw_vertical_block_line(60, 12, 0, black);
        draw_vertical_block_line(60, 37, 0, black);
        draw_vertical_block_line(60, 43, 0, black);
        draw_vertical_block_line(60, 67, 0, black);


        //desenha as linhas brancas pela primeira vez
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 5; j++) {
                //linhas do espaço do primeiro jogador
                draw_vertical_block_line(6, ((i * 4) + i) + 17, (j * 2) * 6, white);

                //linhas do espaço do segundo jogador
                draw_vertical_block_line(6, ((i * 4) + i) + 47, (j * 2) * 6, white);
            }
        }

        return 1;
    
    
}

void bg_animation() {
    int i, j;

    /*são 12 quadros que vão de 0 a 11*/
    if(frame > 11) {
        frame = 0; /*quando chegar no 11 volta para o quadro 0*/

    }

    
        /*enquanto o valor de quadro é menor que 6 segue uma rotina*/
        if (frame < 6) {
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 5; j++) {

                    /*desaparece com o bloco do inicio da linha*/
                    draw_background_block(((i * 4) + i) + 17, (j * 2) * 6 + (frame), invisible);
                    draw_background_block(((i * 4) + i) + 47, (j * 2) * 6 + (frame), invisible);

                    /*acrescenta um bloco ao final da linha*/
                    draw_background_block(((i * 4) + i) + 17, ((j * 2) * 6) + (frame + 6), white);
                    draw_background_block(((i * 4) + i) + 47, ((j * 2) * 6) + (frame + 6), white);
                }
            }
        }
        /*quando o valor de quadro passar de 6 segue uma rotina diferente*/ 
        else {
            for (i = 0; i < 4; i++){
                for (j = 0; j < 5; j++) {
                    /*desaparece com o bloco do inicio da linha*/
                    draw_background_block(((i * 4) + i) + 17, (j * 2) * 6 + (frame), invisible);
                    draw_background_block(((i * 4) + i) + 47, (j * 2) * 6 + (frame), invisible);

                    /*insere aos poucos os blocos que foram removidos no laço de repetição anterior*/
                    draw_background_block(((i * 4) + i) + 17, ((j * 2) * 6) + (frame - 6), white);
                    draw_background_block(((i * 4) + i) + 47, ((j * 2) * 6) + (frame - 6), white);
                }
            }
        }
    
    frame++; /*incrementa o quadro*/
}