#include "../Lib/colenda.h"
#include "../Lib/test.c"
#include <stdio.h>
#include <unistd.h>

int main(){

    GPU_open();
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

    int x, change;

    clear();

    set_background_color(cinza);


    for (int i = 0; i < 9; i++)
    {
        draw_vertical_block_line(60, i, 0, verde);
        draw_vertical_block_line(60, (79-i), 0, verde);
    }

    draw_vertical_block_line(60, 9, 0, preto);
    draw_vertical_block_line(60, 70, 0, preto);

    x = 9;
    for (int i = 0; i < 5; i++)
    {
        x +=6;

        for (int j = 0; j < 6; j++)
        {
            draw_vertical_block_line(5, x-3, (j*10), branco);
            draw_vertical_block_line(5, 79-x+3,(j*10), branco);
        }
        

        draw_vertical_block_line(60, x, 0, preto);
        draw_vertical_block_line(60, 79-x, 0, preto);

    }

    usleep(500);
    
    change = 0;
    
    while (1)
    {
        x = 9;

        for (int i = 0; i < 5; i++)
        {
            x +=6;

            for (int j = 0; j < 6; j++)
            {
                draw_background_block(x-3, (j*10)+change, invisivel);
                draw_background_block(79-x+3,(j*10)+change, invisivel);

                draw_background_block(x-3, (j*10)+change+5, branco);
                draw_background_block(79-x+3, (j*10)+change+5, branco);
            }

        }
        usleep(500);
        if(change == 59){
            change = 0;
        } else{
            change++;
        }
    }
    
    GPU_close();

    return 0;
}