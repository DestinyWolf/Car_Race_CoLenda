#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include "keys.c"

#define DRIVER_PATH "/dev/key_driver"


int main(int argc, char const *argv[])
{
  char button = ' '; 
  KEYS_open();

  while(button != '3'){
    KEYS_read(&button);
    printf("Button pressed: %c\n", button);
  }
  KEYS_close();
  return 0;
}
