#ifndef MOUSE_MODULE_COLENDA
#define MOUSE_MODULE_COLENDA


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int read_mouse_event(int* key, int* coord_x);
int module_init();
int module_exit();

#endif