/**
 * \file            colenda.c
 * \brief           arquivo souce do módulo kernel colenda
 */

/*
 * Copyright (c) 2024 Brenda Barbosa, Camila Queiroz, Maike Nascimento
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of Colenda.
 *
 * Authors:          Brenda Barbosa, Camila Queiroz, Maike de Oliveira
 */

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include "/usr/src/3.18.0/arch/arm/include/asm/io.h"
#include "./address_map_arm.h"

/* Meta information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brenda Barbosa, Camila Queiroz, Maike de Oliveira");
MODULE_DESCRIPTION("Um driver para realizar a comunicação com o processador gráfico CoLenda na FPGA");

/* Barramento de dados do processador gráfico */
#define DATAA 0x80
#define DATAB 0x70
#define WRFULL 0xb0
#define WRREG 0xc0
#define SCREEN 0xa0
#define RESET_PULSECOUNTER 0x90

/*Opcodes das instruções */
#define WBR 0 //0000
#define WSM 1 //0001
#define WBM 2 //0010
#define DP 3 //0011

/*Nome do driver */
#define DRIVER_NAME "colenda_driver"

/* Tamanho do buffer do kernel */
#define BUFF_SIZE 8

/*Driver data*/
static struct
{
  dev_t devnum;
  struct cdev cdev;
  size_t buffer_size; //tamanho do buffer do kernel
  void *LW_virtual;
  volatile int *data_a;
  volatile int *data_b;
  volatile int *wr_full;
  volatile int *wr_reg;
  volatile int *screen;
  volatile int *reset_pulsecounter;
  
} colenda_driver_data;

/*
* Implementação da função open
*/
static int colenda_driver_open(struct inode *device_file, struct file *instance){
  pr_info("%s: open was called!\n", DRIVER_NAME);
  return 0;
}

/*
* Implementação da função close
*/
static int colenda_driver_close(struct inode *device_file, struct file *instance){
  pr_info("%s: close was called!\n", DRIVER_NAME);
  return 0;
}

/*
* Implementação da função write
*/
static ssize_t colenda_driver_write(struct file *file, const char __user *buffer, size_t count,
loff_t *ppos){
  char kbuffer[colenda_driver_data.buffer_size];
  /* Selecionando o menor valor entre o tamanho do kbuffer e count */
  ssize_t size = min(sizeof(kbuffer), count); 

  /* Atualizando offset passado pelo usuário*/
  *ppos = 0;

  if(size == 0){ //sem bytes a serem escritos
    return 0;
  }

  *colenda_driver_data.wr_reg = 0;
  

  /* Verificando se a tela está sendo renderizada */
  if (!*colenda_driver_data.screen){
    printk("tela em renderização\n");
    return -1;
  }
  
  /* Copiando buffer do usuário*/
  if(copy_from_user(kbuffer, buffer, size)){
    return -EFAULT;
  }

  /* Verificando se a fila está cheia */
  if(*colenda_driver_data.wr_full){
    printk("fila cheia\n");
    return -1;
  }


  /*Escrevendo nos barramentos data A e data B */
  *colenda_driver_data.data_a = (kbuffer[4]) << 24 | (kbuffer[5]) << 16 | (kbuffer[6]) << 8 | (kbuffer[7]);
  *colenda_driver_data.data_b = (kbuffer[0]) << 24 | (kbuffer[1]) << 16 | (kbuffer[2]) << 8 | (kbuffer[3]);
  

  /*Enviando sinal para escrita na fila */
  *colenda_driver_data.wr_reg = 1;
  *colenda_driver_data.wr_reg = 0;

  return size;

}

/* Registrando callbacks*/
static const struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = colenda_driver_open,
  .release = colenda_driver_close,
  .write = colenda_driver_write
};

/*
* Implementação da função init
*/
static int __init colenda_driver_init(void){

  int result;
  /*Alocando device number*/
  result = alloc_chrdev_region(&colenda_driver_data.devnum, 0, 1, DRIVER_NAME);

  /*Inicializando o tamanho do buffer*/
  colenda_driver_data.buffer_size = BUFF_SIZE;

  /* Verificando se a alocação do chrdev foi bem sucedida */
  if(result)
  {
    pr_err("%s: failed to allocate device number!\n",DRIVER_NAME);
    return result;
  }

  /*Registrando dispositivo de caractere*/
  cdev_init(&colenda_driver_data.cdev, &fops);

  result = cdev_add(&colenda_driver_data.cdev, colenda_driver_data.devnum, 1);

  /* Verificando se o registro do dispositivo de caractere foi bem sucedido */
  if (result)
  {
    pr_err("%s: char device registration failed!\n",DRIVER_NAME);
    unregister_chrdev_region(colenda_driver_data.devnum, 1); // desalocando chrdev
    return result;
  }

  /*Mapeando lightweight HPS-to-FPGA brigde*/
  colenda_driver_data.LW_virtual = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
  
  /*Mapeando barramentos e sinais do processador gráfico*/
  colenda_driver_data.data_a = (int*) (colenda_driver_data.LW_virtual + DATAA);
  colenda_driver_data.data_b = (int*) (colenda_driver_data.LW_virtual + DATAB);
  colenda_driver_data.wr_reg = (int*) (colenda_driver_data.LW_virtual + WRREG);
  colenda_driver_data.wr_full = (int*) (colenda_driver_data.LW_virtual + WRFULL);
  colenda_driver_data.screen = (int*) (colenda_driver_data.LW_virtual + SCREEN);
  colenda_driver_data.reset_pulsecounter = (int*) (colenda_driver_data.LW_virtual + RESET_PULSECOUNTER);


  pr_info("%s: initialized!\n",DRIVER_NAME);
  return 0;

}

/*
* Implementação da função exit
*/
static void __exit colenda_driver_exit(void){
  /* Liberando o mapeamento do barramento de dados */
  iounmap(colenda_driver_data.LW_virtual);

  /* Removendo chrdev */
  cdev_del(&colenda_driver_data.cdev);

  /* Desalocando chrdev */
  unregister_chrdev_region(colenda_driver_data.devnum, 1);

  pr_info("%s: exiting!\n",DRIVER_NAME);

}

/*
* Chamada dos módulos de init e exit
*/
module_init(colenda_driver_init);
module_exit(colenda_driver_exit);