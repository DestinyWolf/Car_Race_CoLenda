#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>


#define DRIVER_PATH "/dev/key_driver"

/* Indentificadores dos botões*/
#define BUTTON0 '0'
#define BUTTON1 '1'
#define BUTTON2 '2'
#define BUTTON3 '3'

/* Variáveis globais*/
static int dev = -1;

/*Protótipos das funções*/
int KEYS_open();
char KEYS_read();
int KEYS_close();


int KEYS_open(){
  /* abrir o arquivo com permissão de escrita e caso exista, sobreescreve o arquivo */
    dev = open(DRIVER_PATH, O_RDONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dev == -1) {
        printf("Failed to open file!\n");
        return -1;
    }
  return 0;
}

char KEYS_read(){
  char button_pressed = ' ';

  read(dev, &button_pressed, 1);

  return button_pressed;
}

int KEYS_close(){
  /* caso haja algum erro ao encerrar a comunicação retorna -1 */ 
  if (close(dev) == -1) {
      printf("Failed to close file!\n");
      return -1;
  }
  return 0;
}