#include "../Lib/colenda.h"
#include "background_animation_module.c"
#include <stdio.h>
#include <unistd.h>

int main() {

    GPU_open();
    clear();
    bg_animation_module_init();
    char c;

    int i = 0;
    
    while (1)
    {
        usleep(100000);
        bg_animation();
    }
    

    sleep(1);
    GPU_close();

    return 0;
}