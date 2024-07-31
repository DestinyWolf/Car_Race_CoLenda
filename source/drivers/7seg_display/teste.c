#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>
#include "display_7seg.h"



int main(){
	display_open();
	display_clear();
	//display_write_digit(HEX3, 4);
	//display_write_int(12345);
	//display_write_score(345, 0);
	//uint8_t letters[] = {P_DISPLAY,A_DISPLAY,U_DISPLAY,S_DISPLAY,E_DISPLAY,OFF_DISPLAY};
	//display_write_word(letters);
	display_title_animation();
	display_close();

	return 0;
}
