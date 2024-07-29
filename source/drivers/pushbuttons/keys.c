#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>


#define KEYS_DRIVER_PATH "/dev/key_driver"

/* Indentificadores dos botões*/
#define BUTTON0 '0'
#define BUTTON1 '1'
#define BUTTON2 '2'
#define BUTTON3 '3'

/* Variáveis globais*/
static int keys_dev;

/*Protótipos das funções*/
int KEYS_open();
int KEYS_read(char* button);
int KEYS_close();


int
KEYS_open(){
  keys_dev = -1;
  /* Abre o arquivo com permissão de escrita*/
    keys_dev = open(KEYS_DRIVER_PATH, O_RDONLY);
    if (keys_dev == -1) {
        printf("Failed to open file!\n");
        return -1;
    }
  return 0;
}

int
KEYS_read(char *button){
  /* Realiza a leitura dos botões*/
  int ret = read(keys_dev, button, 1);
  if(ret != 0 || *button == NULL){
    printf("Failed to read keys!\n");
    return -1;
  }
  return 0; 
}

int
KEYS_close(){
  /* Caso haja algum erro ao encerrar a comunicação, retorna -1 */ 
  if (close(keys_dev) == -1) {
      printf("Failed to close file!\n");
      return -1;
  }
  return 0;
}
