/* 
* Este arquivo implementa funções para o acesso aos componentes disponíveis na 
* placa DE1-SoC com ARM Cortex A9.
* Este código em sua totalidade foi copiado do manual 'Using Linux* on DE-series Boards' disponibilizado pela
* empresa FPGA Academy a partir do link <https://fpgacademy.org/tutorials.html>.
* Nenhuma alteração foi realizada.
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "./address_map_arm.h"
#include"../Lib/colenda.h"
/* Prototypes for functions used to access physical memory addresses */
int open_physical(int);
void *map_physical(int, unsigned int, unsigned int);
void close_physical(int);
int unmap_physical(void *, unsigned int);

/* Open /dev/mem to give access to physical addresses */
int open_physical(int fd)
{
  if (fd == -1) // check if already open
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
    {
      printf("ERROR: could not open \"/dev/mem\"...\n");
      return (-1);
    }
  return fd;
}

/* Close /dev/mem to give access to physical addresses */
void close_physical(int fd)
{
  close(fd);
}

/* Establish a virtual address mapping for the physical addresses starting
 * at base and extending by span bytes */
void *map_physical(int fd, unsigned int base, unsigned int span)
{
  void *virtual_base;
  // Get a mapping from physical addresses to virtual addresses
  virtual_base = mmap(NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED,
                      fd, base);
  if (virtual_base == MAP_FAILED)
  {
    printf("ERROR: mmap() failed...\n");
    close(fd);
    return (NULL);
  }
  return virtual_base;
}

/* Close the previously-opened virtual address mapping */
int unmap_physical(void * virtual_base, unsigned int span) {
  if (munmap(virtual_base, span) != 0) {
    printf ("ERROR: munmap() failed...\n");
    return (-1);
  }
  return 0;
}

int main(int argc, char const *argv[])
{
  int fd = -1;
  void *LW_virtual;
  volatile int *KEY_ptr;

  fd = open_physical(fd);
  LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

  KEY_ptr = (int *) LW_virtual + 0x0;
  GPU_open();
  color_t gray = {3,3,3};

  while(1){
    if(*KEY_ptr == 14){
      printf("\nCapture\n");
      set_background_color(gray);
      GPU_close();
      unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
      close_physical(fd);
      return 0;
    }
  }
  return 0;
}
