/* main.c ---
*
* Filename: main.c
* Description:
* Author:
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>
#include <f3d_dac.h>
#include <diskio.h>
#include <ff.h>
#define TIMER 20000

int main(void) {
f3d_gyro_init();
f3d_lcd_init();
f3d_lcd_fillScreen2(WHITE);
float test[3];

while(1){
f3d_gyro_getdata(test);
f3d_lcd_fillScreen2(WHITE);
f3d_lcd_drawChar(100, 80, 'Z', BLACK, WHITE);
f3d_lcd_drawChar(60, 80, 'Y', BLACK, WHITE);
f3d_lcd_drawChar(20, 80, 'X', BLACK, WHITE);
int x, y;

//////X AXIS
if(test[0] > 0)
for (x = 15; x < 25; x++)
for (y = 75; y > 75/(test[0]); y--)
f3d_lcd_drawPixel(x, y, RED);
else
for (x = 15; x < 25; x++)
for (y = 85; y < -1*(70/test[0]+85); y++)
f3d_lcd_drawPixel(x, y, RED);

//////Y AXIS
if(test[1] > 0)
for (x = 55; x < 65; x++)
for (y = 75; y > 75/(test[1]); y--)
f3d_lcd_drawPixel(x, y, BLUE);
else
for (x = 55; x < 65; x++)
for (y = 85; y < -1*(70/test[1]+85); y++)
f3d_lcd_drawPixel(x, y, BLUE);

//////Z AXIS
if(test[2] > 0)
for (x = 95; x < 105; x++)
for (y = 75; y > 75/(test[2]); y--)
f3d_lcd_drawPixel(x, y, GREEN);
else
for (x = 95; x < 105; x++)
for (y = 85; y < -1*(70/test[2]+85); y++)
f3d_lcd_drawPixel(x, y, GREEN);
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
