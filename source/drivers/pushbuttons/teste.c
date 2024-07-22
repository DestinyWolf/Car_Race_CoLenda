#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define DRIVER_PATH "/dev/key_driver"


int main(int argc, char const *argv[])
{
  int dev = -1;
  char button;
  /* abrir o arquivo com permissão de escrita e caso exista, sobreescreve o arquivo */
  dev = open(DRIVER_PATH, O_RDONLY);
  if (dev == -1) {
      printf("Failed to open file!\n");
      return -1;
  }

  while(button != '3'){
    read(dev, &button, 1);

    printf("Button pressed: %c\n", button);
  }
  close(dev);
  return 0;
}
