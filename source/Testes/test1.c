#include "../drivers/7seg_display/display_7seg.c"
#include <stdio.h>
#include <unistd.h>

int main() {

    int score = 90;
    display_open();
    display_write_int(score, 1);
    display_close();

    return 0;
}