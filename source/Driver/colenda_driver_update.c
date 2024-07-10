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
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/delay.h>
#include<asm/atomic.h>
#include<linux/mutex.h>
#include<linux/kfifo.h>
#include "/usr/src/3.18.0/arch/arm/include/asm/io.h"
#include "./address_map_arm.h"

/* Meta information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brenda Barbosa, Camila Boa Morte, Maike de Oliveira");
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

/*Identificação da kthread*/
#define KTHREAD_NAME "consume_kfifo_thread"

/*Tamanho do buffer circular*/
#define KFIFO_SIZE  4096 //4096 / 8 = 512 (instruções)

/* Tamanho das instruções recebidas*/
#define INSTRUCTION_SIZE 8

/*Definição da kthread consumidora*/
static struct task_struct *kthread_instruction_consumer;

/* Definição de mutexes */
static struct mutex write_lock;

/* Definição da kfifo */
static struct kfifo kfifo_instructions;

/*Definição das waitqueues */
DECLARE_WAIT_QUEUE_HEAD(consumer_wq);
DECLARE_WAIT_QUEUE_HEAD(write_wq);

/*Driver data*/
static struct
{
  dev_t devnum;
  struct cdev cdev;
  void *LW_virtual;
  volatile int *data_a;
  volatile int *data_b;
  volatile int *wr_full;
  volatile int *wr_reg;
  volatile int *screen;
  volatile int *reset_pulsecounter;
  
} colenda_driver_data;

/* Função a ser executada pela kthread*/
int consume_instruction(void *data){
  char instruction[INSTRUCTION_SIZE];

  pr_info("%s: %s is running now!", DRIVER_NAME, KTHREAD_NAME);
  //screen = 1 -> tela foi renderizada
  while(!kthread_should_stop()){
    
    if (wait_event_interruptible(consumer_wq, !kfifo_is_empty(&kfifo_instructions)  || kthread_should_stop())) return -ERESTARTSYS;

    if (kthread_should_stop()){
      pr_info("%s: %s stopped!", DRIVER_NAME, KTHREAD_NAME);
      return 0;
    }else if (!*colenda_driver_data.screen || *colenda_driver_data.wr_full){
      continue;
    }
     
    
    /*Checando se algum elemento foi removido*/
    kfifo_out(&kfifo_instructions, instruction, INSTRUCTION_SIZE);
    wake_up_interruptible(&write_wq);

    /*Escrevendo nos barramentos data A e data B */
    *colenda_driver_data.data_a = (instruction[4]) << 24 | (instruction[5]) << 16 | (instruction[6]) << 8 | (instruction[7]);
    *colenda_driver_data.data_b = (instruction[0]) << 24 | (instruction[1]) << 16 | (instruction[2]) << 8 | (instruction[3]);
    

    /*Enviando sinal para escrita na fila */
    *colenda_driver_data.wr_reg = 1;
    *colenda_driver_data.wr_reg = 0;

  }
  
  return 0;
  }

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
  unsigned int copied; //bytes copiados do usuário
  int ret = 0;

  /* Verificando se o tamanho da instrução está correto */
  if(count != INSTRUCTION_SIZE) return -1;
   
  
  if(mutex_lock_interruptible(&write_lock)) return -ERESTARTSYS;
  
  while (kfifo_is_full(&kfifo_instructions)){ // kfifo full
    mutex_unlock(&write_lock); // release lock
    /*wait for space in kfifo*/
    if (wait_event_interruptible(write_wq, !kfifo_is_full(&kfifo_instructions)))
      return -ERESTARTSYS;

    if (mutex_lock_interruptible(&write_lock))
      return -ERESTARTSYS; // adquire lock
  }
  
  ret = kfifo_from_user(&kfifo_instructions, buffer, count, &copied);

  mutex_unlock(&write_lock);

  wake_up_interruptible(&consumer_wq);
  
  /* Atualizando offset passado pelo usuário*/
  *ppos = 0;
  
  return ret ? ret : copied;
}

static ssize_t colenda_driver_read(struct file *file, char __user *buf, size_t count, 
loff_t *ppos){
  int ret;
  
  if(count != 1) return -1;

  /*Copiando valor de screen para o usuário*/
  ret = put_user((char)(*colenda_driver_data.screen), buf);

  return ret;
}

/* Registrando callbacks*/
static const struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = colenda_driver_open,
  .release = colenda_driver_close,
  .write = colenda_driver_write,
  .read = colenda_driver_read
};

/*
* Implementação da função init
*/
static int __init colenda_driver_init(void){

  int result;
  /*Alocando device number*/
  result = alloc_chrdev_region(&colenda_driver_data.devnum, 0, 1, DRIVER_NAME);


  /* Verificando se a alocação do chrdev foi bem sucedida */
  if(result)
  {
    pr_err("%s: failed to allocate device number!\n",DRIVER_NAME);
    goto ChrRegionError;
  }

  /*Registrando dispositivo de caractere*/
  cdev_init(&colenda_driver_data.cdev, &fops);

  result = cdev_add(&colenda_driver_data.cdev, colenda_driver_data.devnum, 1);

  /* Verificando se o registro do dispositivo de caractere foi bem sucedido */
  if (result)
  {
    pr_err("%s: char device registration failed!\n",DRIVER_NAME);
    goto CdevAddError;
  }

  /*Criando buffer de instruções*/
  result = kfifo_alloc(&kfifo_instructions, KFIFO_SIZE, GFP_KERNEL);
  if (result)
  {
    pr_err("%s: erro no malloc da fifo!", DRIVER_NAME);
    goto KfifoAllocError;
  }

  kthread_instruction_consumer = kthread_run(consume_instruction, NULL, KTHREAD_NAME);
  if (kthread_instruction_consumer == NULL)
  {
    pr_err("%s: failed to create %s", DRIVER_NAME, KTHREAD_NAME);
    goto KthreadCreateError;
  }

  /*Incializando mutex*/
  mutex_init(&write_lock);


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

KthreadCreateError:
  kfifo_free(&kfifo_instructions);
KfifoAllocError:
  cdev_del(&colenda_driver_data.cdev);
CdevAddError:
  unregister_chrdev_region(colenda_driver_data.devnum, 1);
ChrRegionError:
  return -1;
}

/*
* Implementação da função exit
*/
static void __exit colenda_driver_exit(void){
 /* Stopping consumer kthread*/
 kthread_stop(kthread_instruction_consumer);

  /*Liberando espaço alocado para a kfifo de instruções*/
  kfifo_free(&kfifo_instructions);

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