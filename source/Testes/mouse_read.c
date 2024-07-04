#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int x, y, fd, key_press;
struct input_event ev;

int read_mouse_event(int* key, int* coord_y, int* coord_x) {
    
        ssize_t n = read(fd, &ev, sizeof(ev));

        if (n == (ssize_t)-1){
            printf("erro ao ler /dev/input/event0\n");
            return 0;
        }

        if(ev.type == EV_REL){
            if (ev.code == REL_X) {
                printf("X: %d\n", ev.value);

                
                x += ev.value;
                if (x <= 0) {
                    x = 639;
                }
                if (x >= 640) {
                    x = 0;
                }
                
                
            } else if (ev.code == REL_Y) {
                printf("Y: %d\n", ev.value);
                y += ev.value;
                if (y <= 0) {
                    y = 479;
                }
                if (y >= 480) {
                    y = 0;
                }
            } else if (ev.code == REL_WHEEL) {
                printf("WHEEL: %d\n", ev.value);
            }
        } else if (ev.type == EV_KEY) {
            if (ev.code == BTN_LEFT) {
                if (ev.value) {
                    key_press = 1;
                }
                printf("botão esquerdo do mouse: %s", ev.value ? "Pressinado":"Solto");
            } else if(ev.code == BTN_RIGHT) {
                if (ev.value) {
                    key_press = 2;
                }
                printf("botão direito do mouse: %s", ev.value ? "Pressinado":"Solto");
            } else if (ev.code == BTN_MIDDLE) {
                if (ev.value) {
                    key_press = 3;
                }
                printf("botão esquerdo do mouse: %s", ev.value ? "Pressinado":"Solto");
            }
        }

    *coord_x = x;
    *coord_y = y;
    *key = key_press;
    key_press = 0;
    return 1;
    
}

int module_init() {
    x = 0;
    y = 0;
    key_press = 0;
    fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        printf("erro ao abrir /dev/input/event0\n");
        return 0;
    }
    return 1;
}

int module_exit() {
    close(fd);
    return 1;
}