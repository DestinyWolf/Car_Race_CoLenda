#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/delay.h>
#include<linux/wait.h>
#include<asm/atomic.h>
#include<linux/errno.h>
#include "/usr/src/3.18.0/arch/arm/include/asm/io.h"
#include "../colenda/address_map_arm.h"


/* Meta information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brenda Barbosa, Camila Boa Morte, Maike de Oliveira");
MODULE_DESCRIPTION("Um driver para realizar a comunicação com botões do tipo push na FPGA");

/*Nome do driver */
#define DRIVER_NAME "key_driver"

/*Identificação da kthread*/
#define KTHREAD_NAME "polling_thread"

#define KEY_BASE 0x0

/*Definição da kthread consumidora*/
static struct task_struct *polling_kthread;

/*Definição das waitqueues */
DECLARE_WAIT_QUEUE_HEAD(read_wq);
DECLARE_WAIT_QUEUE_HEAD(poll_wq);

typedef enum {
  STATE_NOT_PRESSED,
  STATE_CLICK,
  STATE_PRESSED
} State;

typedef enum{
  NO_KEY = 15,
  K0 = 14,
  K1 = 13,
  K2 = 11,
  K3 = 7
} Keys;

typedef struct 
{
  State state;
  int8_t exit_value;
} state_machine_t;


/*Driver data*/
static struct
{
  dev_t devnum;
  struct cdev cdev;
  void *LW_virtual;
  volatile int *KEY_ptr;
  state_machine_t state_machine;
  atomic_t ready; //flag que indica que o dado está pronto pra ser lido
  atomic_t count; //numero de opens
} key_driver_data;


int polling_function(void *data){
  int8_t button_pressed;

  while (!kthread_should_stop()){
    wait_event_interruptible(poll_wq, kthread_should_stop() || (atomic_read(&key_driver_data.count) != 0));
    
    if (kthread_should_stop()) break;
    
    key_driver_data.state_machine.state = STATE_NOT_PRESSED;
    key_driver_data.state_machine.exit_value = -1;

    while (atomic_read(&key_driver_data.count) != 0){
      button_pressed = *key_driver_data.KEY_ptr;

      switch (key_driver_data.state_machine.state){
      case STATE_NOT_PRESSED:
        if (button_pressed == K0 || button_pressed == K1 || button_pressed == K2 || button_pressed == K3){
          key_driver_data.state_machine.exit_value = button_pressed;
          key_driver_data.state_machine.state = STATE_CLICK;
          atomic_inc(&key_driver_data.ready);
          wake_up_interruptible(&read_wq);
        }
        break;
      case STATE_CLICK:
        if(button_pressed != NO_KEY){
          key_driver_data.state_machine.state = STATE_PRESSED;
        }
        break;
      case STATE_PRESSED:
        if (button_pressed == NO_KEY){
          key_driver_data.state_machine.state = STATE_NOT_PRESSED;
          key_driver_data.state_machine.exit_value = -1;
        }
        break; 
      }
    } 
  }
  pr_info("%s: %s stopped!\n", DRIVER_NAME, KTHREAD_NAME);
	return 0;
}


static int key_driver_open(struct inode *device_file, struct file *instance){
  pr_info("%s: open was called!\n", DRIVER_NAME);
  atomic_inc(&key_driver_data.count);
	wake_up_interruptible(&poll_wq);
  return 0;
}

/*
* Implementação da função close
*/
static int key_driver_close(struct inode *device_file, struct file *instance){
  pr_info("%s: close was called!\n", DRIVER_NAME);
  atomic_dec(&key_driver_data.count);
  return 0;
}

/*
* Implemetação da função read
*/
static ssize_t key_driver_read(struct file *file, char __user *buf, size_t count, 
loff_t *ppos){
  int ret;
  int8_t button;
  char key = ' ';

  wait_event_interruptible(read_wq, atomic_read(&key_driver_data.ready) == 1);
  atomic_dec(&key_driver_data.ready);
  button = key_driver_data.state_machine.exit_value;
    
  
  switch (button){
    case K0:
      key = '0';
      break;
    case K1:
      key = '1';
      break;
    case K2:
      key = '2';
      break;
    case K3:
      key = '3';
      break;
    default:
      break;
  }
  
  ret = put_user(key, buf);

  /* Atualizando offset passado pelo usuário*/
  *ppos = 0;

  return ret;
}


/* Registrando callbacks*/
static const struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = key_driver_open,
  .release = key_driver_close,
  .read = key_driver_read
};

/*
* Implementação da função init
*/
static int __init key_driver_init(void){

  int result;
  /*Alocando device number*/
  result = alloc_chrdev_region(&key_driver_data.devnum, 0, 1, DRIVER_NAME);


  /* Verificando se a alocação do chrdev foi bem sucedida */
  if(result)
  {
    pr_err("%s: failed to allocate device number!\n",DRIVER_NAME);
    goto ChrRegionError;
  }

  /*Registrando dispositivo de caractere*/
  cdev_init(&key_driver_data.cdev, &fops);

  result = cdev_add(&key_driver_data.cdev, key_driver_data.devnum, 1);

  /* Verificando se o registro do dispositivo de caractere foi bem sucedido */
  if (result)
  {
    pr_err("%s: char device registration failed!\n",DRIVER_NAME);
    goto CdevAddError;
  }

  polling_kthread = kthread_run(polling_function, NULL, KTHREAD_NAME);
  if (polling_kthread == NULL)
  {
    pr_err("%s: failed to create %s", DRIVER_NAME, KTHREAD_NAME);
    goto KthreadCreateError;
  }

  /*Mapeando lightweight HPS-to-FPGA brigde*/
  key_driver_data.LW_virtual = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
  key_driver_data.KEY_ptr = key_driver_data.LW_virtual + KEY_BASE;

  atomic_set(&key_driver_data.ready, 0);
  atomic_set(&key_driver_data.count, 0);

  pr_info("%s: initialized!\n", DRIVER_NAME);
  return 0;

KthreadCreateError:
  cdev_del(&key_driver_data.cdev);
CdevAddError:
  unregister_chrdev_region(key_driver_data.devnum, 1);
ChrRegionError:
  return -1;
}

/*
* Implementação da função exit
*/
static void __exit key_driver_exit(void){
 /* Stopping consumer kthread*/
  kthread_stop(polling_kthread);

  /* Liberando o mapeamento do barramento de dados */
  iounmap(key_driver_data.LW_virtual);

  /* Removendo chrdev */
  cdev_del(&key_driver_data.cdev);

  /* Desalocando chrdev */
  unregister_chrdev_region(key_driver_data.devnum, 1);

  pr_info("%s: exiting!\n",DRIVER_NAME);
}

/*
* Chamada dos módulos de init e exit
*/
module_init(key_driver_init);
module_exit(key_driver_exit);
