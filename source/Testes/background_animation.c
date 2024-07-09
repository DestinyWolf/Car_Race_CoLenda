
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
        draw_vertical_block_line(59, i, 0, verde);
        draw_vertical_block_line(59, i+69, 0, verde);
    }

    draw_vertical_block_line(59, 11, 0, preto);
    draw_vertical_block_line(59, 68, 0, preto);


    //desenha as linhas brancas pela primeira vez
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            draw_vertical_block_line(6, ((i * 6) + 1) + 18, (j*2) * 6, branco);
            draw_vertical_block_line(6, ((i * 6) + 1) + 40, (j*2) * 6, branco);
        }
    }

    frame = 1;

    return 1;
}

void bg_animation() {


    if( frame > 12) {
        frame = 1;

    }
    else if (frame < 6) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 5; j++) {
                draw_background_block(((i * 6) + 1) + 18, (j*2) * 6 + (frame - 1), invisivel);
                draw_background_block(((i * 6) + 1) + 40, (j*2) * 6 + (frame - 1), invisivel);
                draw_background_block(((i * 6) + 1) + 18, ((j*2) * 6) + 6 + (frame - 1), branco);
                draw_background_block(((i * 6) + 1) + 40, ((j*2) * 6) + 6 + (frame - 1), branco);
            }
        }
    } else {
        for (j = 0; j < 5; j++) {
                draw_background_block(((i * 6) + 1) + 18, (j*2) * 6 + (frame - 6), invisivel);
                draw_background_block(((i * 6) + 1) + 40, (j*2) * 6 + ( frame - 6), invisivel);
                draw_background_block(((i * 6) + 1) + 18, ((j*2) * 6) + (frame - 6), branco);
                draw_background_block(((i * 6) + 1) + 40, ((j*2) * 6) + (frame - 6), branco);
            }
    }
    frame++;
}

int bg_animation_module_exit() {
    return 1;
}