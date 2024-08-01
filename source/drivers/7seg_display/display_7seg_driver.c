#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h> //copy_from_user
#include<linux/ioctl.h>
#include <linux/slab.h> //kmalloc
#include "/usr/src/3.18.0/arch/arm/include/asm/io.h"
#include "../colenda/address_map_arm.h"
#include "./display_7seg_driver.h"

/* Meta information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brenda Barbosa, Camila Boa Morte, Maike de Oliveira");
MODULE_DESCRIPTION("Um módulo kernel para realizar a comunicação com os displays de 7 segmentos na FPGA");

#define HEX5_BASE               0x10
#define HEX4_BASE               0x20
#define HEX3_BASE               0x30
#define HEX2_BASE               0x40
#define HEX1_BASE               0x50
#define HEX0_BASE               0x60


/*Driver data*/
static struct{
  dev_t devnum;
  struct cdev cdev;
  void *LW_virtual;
  volatile int* hex0_ptr;
  volatile int* hex1_ptr;
  volatile int* hex2_ptr;
  volatile int* hex3_ptr;
  volatile int* hex4_ptr;
  volatile int* hex5_ptr;
} display_7seg_data;


/*
* Implementação da função open
*/
static int
display_7seg_open(struct inode *device_file, struct file *instance){
  pr_info("%s: open was called!\n", DRIVER_NAME);
  return 0;
}

/*
* Implementação da função close
*/
static int
display_7seg_close(struct inode *device_file, struct file *instance){
  pr_info("%s: close was called!\n", DRIVER_NAME);
  return 0;
}

/*
 * Implementando a funão ioctl
*/
static long int
display_7seg_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
  if(cmd == WR_VALUE){
    /*Copiando dados do espaço de usuário */
    ioctl_args_t* args_kernel = kmalloc(sizeof(ioctl_args_t), GFP_KERNEL);

    if(copy_from_user(args_kernel, (void*) arg, sizeof( ioctl_args_t))){
      pr_err("%s: error copyind data to the kernel\n", DRIVER_NAME);
      return -EFAULT;
    }

    /*Passando dado para o display correto*/
    switch (args_kernel->hex_id){
    case 0:
      *display_7seg_data.hex0_ptr = args_kernel->data;
      break;
    case 1:
      *display_7seg_data.hex1_ptr = args_kernel->data;
      break;
    case 2:
      *display_7seg_data.hex2_ptr = args_kernel->data;
      break;
    case 3:
      *display_7seg_data.hex3_ptr = args_kernel->data;
      break;
    case 4:
      *display_7seg_data.hex4_ptr = args_kernel->data;
      break;
    case 5:
      *display_7seg_data.hex5_ptr = args_kernel->data;
      break;
    }  
  }
  
  //kfree(args_kernel);
  return 0;
}

/* Registrando callbacks*/
static const struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = display_7seg_open,
  .release = display_7seg_close,
  .unlocked_ioctl = display_7seg_ioctl,
};

/*
* Implementação da função init
*/
static int __init
display_7seg_init(void){

  int result;
  /*Alocando device number*/
  result = alloc_chrdev_region(&display_7seg_data.devnum, 0, 1, DRIVER_NAME);


  /* Verificando se a alocação do chrdev foi bem sucedida */
  if(result){
    pr_err("%s: failed to allocate device number!\n", DRIVER_NAME);
    goto ChrRegionError;
  }

  /*Registrando dispositivo de caractere*/
  cdev_init(&display_7seg_data.cdev, &fops);

  result = cdev_add(&display_7seg_data.cdev, display_7seg_data.devnum, 1);

  /* Verificando se o registro do dispositivo de caractere foi bem sucedido */
  if (result){
    pr_err("%s: char device registration failed!\n",DRIVER_NAME);
    goto CdevAddError;
  }

  /*Mapeando lightweight HPS-to-FPGA brigde*/
  display_7seg_data.LW_virtual = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

  /*Mapeando os endereços dos 6 displays*/
  display_7seg_data.hex0_ptr = (int*) (display_7seg_data.LW_virtual + HEX0_BASE);
  display_7seg_data.hex1_ptr = (int*) (display_7seg_data.LW_virtual + HEX1_BASE);
  display_7seg_data.hex2_ptr = (int*) (display_7seg_data.LW_virtual + HEX2_BASE);
  display_7seg_data.hex3_ptr = (int*) (display_7seg_data.LW_virtual + HEX3_BASE);
  display_7seg_data.hex4_ptr = (int*) (display_7seg_data.LW_virtual + HEX4_BASE);
  display_7seg_data.hex5_ptr = (int*) (display_7seg_data.LW_virtual + HEX5_BASE);

  pr_info("%s: initialized!\n",DRIVER_NAME);
  return 0;

CdevAddError:
  unregister_chrdev_region(display_7seg_data.devnum, 1);
ChrRegionError:
  return -1;
}

/*
 * Implementação da função exit
*/
static void __exit
display_7seg_exit(void){
  /* Liberando o mapeamento do barramento de dados */
  iounmap(display_7seg_data.LW_virtual);

  /* Removendo chrdev */
  cdev_del(&display_7seg_data.cdev);

  /* Desalocando chrdev */
  unregister_chrdev_region(display_7seg_data.devnum, 1);

  pr_info("%s: exiting!\n",DRIVER_NAME);
}

/*
 * Chamada dos módulos de init e exit
*/
module_init(display_7seg_init);
module_exit(display_7seg_exit);
