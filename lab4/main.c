//Ben Wu
//Caulin Jones
//lab4 main.c
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h>
#include <f3d_uart.h>

// Simple looping delay function
void delay(void) {
  int i = 1000000; //Make it 10 times faster
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void){
  f3d_uart_init();
  
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  while(1){ 
    putchar(getchar());
    // putstring("Call the amberlamps \n");
  }
  
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif
