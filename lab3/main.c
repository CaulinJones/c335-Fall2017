/*
***************
** main.c
** This file is the main file for my lab 3 that is a program that turns on seque**ntally leds o nthe discovery board
**Date created: 7/8/2017
**Date Last Modified: 7/13/2017
**Authors: Caulin Jones and Baylor Mudd
 */




#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h>

// Simple looping delay function
void delay(void) {
  int i = 5000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  
  // initializes system clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  //initalize led pin
  f3d_led_init();
  //initialize button
  f3d_user_btn_init();
  
  GPIO_InitTypeDef GPIO_InitStructure;

  //initializing button specs
  /*
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   sending button commands to board
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  */
  int i = 0;
  while(1){
    while(user_btn_read()) {
      f3d_led_on(i);
    }
    f3d_led_on(i);
    delay();
    f3d_led_off(i);
    i++;
    if(i == 8){
      f3d_led_all_on();
      delay();
      f3d_led_all_off();
      i=0;
    }
  }
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
