
#include "background_animation_module.h"
#include <unistd.h>
#include <stdio.h>
#include "../Lib/colenda.h"

//96 e terminar em 536



//cores do fundo 
color_t verde = {
    .blue = 2,
    .green = 6,
    .red = 2,
};

color_t cinza = {
    .blue = 4,
    .green = 4,
    .red = 4,
};

color_t branco = {
    .blue = 6,
    .green = 6,
    .red = 6,
};

color_t preto = {
    .blue = 0,
    .green = 0,
    .red = 0,
};

color_t invisivel = {
    .blue = 7,
    .green = 7,
    .red = 6,
};

int frame;

int bg_animation_module_init() {
    
    int i, j;
    frame = 0;

    set_background_color(cinza);
    //desenha as bordas da tela
    for (i = 0; i < 11; i ++) {
        draw_vertical_block_line(60, i, 0, verde);
        draw_vertical_block_line(60, i+69, 0, verde);
    }

    draw_vertical_block_line(60, 11, 0, preto);
    draw_vertical_block_line(60, 68, 0, preto);


    //desenha as linhas brancas pela primeira vez
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            draw_vertical_block_line(6, ((i * 6) + 1) + 18, (j*2) * 6, branco);
            draw_vertical_block_line(6, ((i * 6) + 1) + 42, (j*2) * 6, branco);
        }
    }

    return 1;
}

void bg_animation() {
    int i, j;
    frame++;

    if( frame > 12) {
        frame = 1;

    }
    if (frame < 6) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 5; j++) {
                draw_background_block(((i * 6) + 1) + 18, (j * 2) * 6 + (frame - 1), invisivel);
                draw_background_block(((i * 6) + 1) + 42, (j * 2) * 6 + (frame - 1), invisivel);
                draw_background_block(((i * 6) + 1) + 18, ((j * 2) * 6) + (frame - 1) + 6, branco);
                draw_background_block(((i * 6) + 1) + 42, ((j * 2) * 6) + (frame - 1) + 6, branco);
            }
        }
    } else {
        for (i = 0; i < 4; i++){
            for (j = 0; j < 5; j++) {
                draw_background_block(((i * 6) + 1) + 18, (j * 2) * 6 + (frame - 1), invisivel);
                draw_background_block(((i * 6) + 1) + 42, (j * 2) * 6 + (frame - 1), invisivel);
                draw_background_block(((i * 6) + 1) + 18, ((j * 2) * 6) + (frame - 7), branco);
                draw_background_block(((i * 6) + 1) + 42, ((j * 2) * 6) + (frame - 7), branco);
            }
        }
    }
    
}