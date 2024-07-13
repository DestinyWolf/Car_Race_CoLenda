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
MODULE_DESCRIPTION("Key press handler");

/* Barramento de dados do processador gráfico */
#define DATAA 0x80
#define DATAB 0x70
#define WRFULL 0xb0
#define WRREG 0xc0
#define SCREEN 0xa0
#define RESET_PULSECOUNTER 0x90
#define KEYBASE 0

/*Nome do driver */
#define DRIVER_NAME "key_handler"

/*Identificação da kthread*/
#define KTHREAD_NAME "key_capture"

/*Definição da kthread consumidora*/
static struct task_struct *kthread_key_handler;

DECLARE_WAIT_QUEUE_HEAD(handler_wq);

/*Driver data*/
static struct
{
  void *LW_virtual;
  volatile int *KEY_ptr;
  volatile int *data_a;
  volatile int *data_b;
  volatile int *wr_full;
  volatile int *wr_reg;
  volatile int *screen;
  volatile int *reset_pulsecounter;
  int counter;
  
} key_handler_data; 


int handler_key_press(void *data){
	
	pr_info("%s: %s is running now!", DRIVER_NAME, KTHREAD_NAME);
	
	
	while(!kthread_should_stop()){
	
		if(wait_event_interruptible(handler_wq, kthread_should_stop() || *key_handler_data.KEY_ptr == 14)) return -ERESTARTSYS;
		
		if(kthread_should_stop()){
			pr_info("%s: %s finished!\n", DRIVER_NAME, KTHREAD_NAME);
			return 0;
		}
		pr_info("%s: button pressed!\n", KTHREAD_NAME);
		*key_handler_data.data_a = 0;
		*key_handler_data.data_b = ++key_handler_data.counter;
		*key_handler_data.wr_reg = 1;
		*key_handler_data.wr_reg = 0;
		
		if(key_handler_data.counter == 511) key_handler_data.counter = 0;
		
	}

	pr_info("%s: %s finished!\n", DRIVER_NAME, KTHREAD_NAME);
	return 0;
}



/*
* Implementação da função init
*/
static int __init key_handler_init(void){

	kthread_key_handler = kthread_run(handler_key_press, NULL, KTHREAD_NAME);
	
	if(kthread_key_handler == NULL){
		pr_err("%s: failed to create thread!\n", DRIVER_NAME);
		return -1;
	}

	key_handler_data.LW_virtual = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
	/*Mapeando barramentos e sinais do processador gráfico*/
	key_handler_data.data_a = (int*) (key_handler_data.LW_virtual + DATAA);
	key_handler_data.data_b = (int*) (key_handler_data.LW_virtual + DATAB);
	key_handler_data.wr_reg = (int*) (key_handler_data.LW_virtual + WRREG);
	key_handler_data.wr_full = (int*) (key_handler_data.LW_virtual + WRFULL);
	key_handler_data.screen = (int*) (key_handler_data.LW_virtual + SCREEN);
	key_handler_data.reset_pulsecounter = (int*) (key_handler_data.LW_virtual + RESET_PULSECOUNTER);
	
	key_handler_data.KEY_ptr = key_handler_data.LW_virtual + KEYBASE; 
	
	key_handler_data.counter = 0;
	pr_info("%s: initialized!", DRIVER_NAME);
	return 0;
}


/*
* Implementação da função exit
*/
static void __exit key_handler_exit(void){
 /* Stopping consumer kthread*/
  kthread_stop(kthread_key_handler);

  /* Liberando o mapeamento do barramento de dados */
  iounmap(key_handler_data.LW_virtual);

	pr_info("%s: exiting!\n",DRIVER_NAME);
}

/*
* Chamada dos módulos de init e exit
*/
module_init(key_handler_init);
module_exit(key_handler_exit);




