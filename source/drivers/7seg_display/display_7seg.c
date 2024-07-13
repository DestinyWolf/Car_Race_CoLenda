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

#define DRIVER_FILE "/dev/display_7seg"

#define OFF 0 //0000000
#define ZERO 126 //1111110
#define ONE 48 //0110000
#define TWO 109 //1101101
#define THREE 121 //1111001
#define FOUR 51 //0110011
#define FIVE 91 //1011011
#define SIX  95 //1011111
#define SEVEN 112 //1110000
#define EIGHT 127 //1111111
#define NINE 123 //1111011

int dev = -1;

int display_open();
int display_write(short int, int);
int display_close();
int display_clear();

int display_open(){

    /* abrir o arquivo com permissão de escrita e caso exista, sobreescreve o arquivo */
    dev = open(DRIVER_FILE, O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dev == -1) {
        printf("Failed to open file!\n");
        return 0;
    }
    return 1;
}

int display_write(short int hex, int info){

    if(hex > 7 || info > 127) return -1;
    //TODO: pass info to display



    return 0;

}

int display_write_all(int hex0, int hex1, int hex2, int hex3, int hex4, int hex5){

    if(hex0+hex1+hex2+hex3+hex4+hex5 >  762) return -1;

    //TODO : pass info to display
    return 0;
}

int display_clear(){

    //TODO: pass off to displays

}
int display_close(){
    /* caso haja algum erro ao encerrar a comunicação retorna 0 */ 
    if (close(dev) == -1) {
        printf("Failed to close file!\n");
        return 0;
    }
    return 1;
}

