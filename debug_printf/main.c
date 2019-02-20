//Main.c lab5
//Ben Wu
//Caulin Jones
//gyro lab
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>


void delay(void) {
  int i = 5000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

void led_delay(void){
  int i = 1000000;
  while (i--> 0 ){
    asm("nop"); 
  }
}

//gets a character input
int getchar_new(void) {
  int c;
  //Modify getchar function to make sure the uart isn't busy. If it is not,
  //then it takes the input. Otherwise continue to send a dummy char  
  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != (uint16_t)RESET){
    c = USART_ReceiveData(USART1);
  }else{
    c = 0x00;
  }
  return c;
} 

int main(void){
  float gyro[3];
  f3d_led_init(); 
  f3d_user_btn_init(); 
  f3d_uart_init(); 
  f3d_gyro_init(); 

  int i=0; 
  int c; 
 
  while(1){
     
    c = getchar_new(); 
    f3d_gyro_getdata(gyro);  
    
    
    switch(c){
    case 'x': 
      i = 0; 
      break; 
    case 'y':
      i = 1; 
      break; 
    case 'z': 
      i = 2; 
      break; 
    default: 
      break; 
    }

    
    switch((int) gyro[i]){
     // negative values
    case -60 ... -6:
      f3d_led_all_off();
      f3d_led_on(1);
      led_delay();
      break;
    case -120 ...-61:
      f3d_led_all_off();
      f3d_led_on(1);
      f3d_led_on(2);
      led_delay();
      break;
    case -180 ...-121:
      f3d_led_all_off();
      f3d_led_on(1);
      f3d_led_on(2);
      f3d_led_on(3);
      led_delay();
      break;
    case -240 ... -181:
      f3d_led_all_off();
      f3d_led_on(1);
      f3d_led_on(2);
      f3d_led_on(3);
      f3d_led_on(4);
      led_delay();
      break;
    case -550 ... -241:
      f3d_led_all_off();
      f3d_led_on(1);
      f3d_led_on(2);
      f3d_led_on(3);
      f3d_led_on(4);
      f3d_led_on(5);
      led_delay();
      break;

      // positive values
    case 6 ... 60:
      f3d_led_all_off();
      f3d_led_on(1);
      led_delay();
      break;
    case 61 ...120:
      f3d_led_all_off();
      f3d_led_on(1);
      f3d_led_on(0);
      led_delay();
      break;
    case 121 ...180:
      f3d_led_all_off();
      f3d_led_on(1);
      f3d_led_on(0);
      f3d_led_on(7);
      led_delay();
      break;
    case 181 ... 240:
      f3d_led_all_off();
      f3d_led_on(1);
      f3d_led_on(0);
      f3d_led_on(7);
      f3d_led_on(6);
      led_delay();
      break;
    case 241 ... 500:
      f3d_led_all_off();
      f3d_led_on(1);
      f3d_led_on(0);
      f3d_led_on(7);
      f3d_led_on(6);
      f3d_led_on(5);
      led_delay();
      break;
      
    default:
      f3d_led_all_off();
      break;
    }
      
    
    
    //Button change    
    if(user_btn_read()){
      i++; 
      if(i>2){
	i = 0;
      } 
    }
    //Final output
    switch(i){
    case 0:
      printf("X %f\n",gyro[0]); 
      break; 
    case 1: 
      printf("Y %f\n",gyro[1]);
      break; 
    case 2: 
      printf("Z %f\n",gyro[2]); 
    default: 
      break; 
    }

    
  }
}


void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
