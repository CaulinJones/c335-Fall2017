/* main.c ---
*
* Filename: main.c
* Description:
* Author: Alex Koopman, Caulin Jones
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_gyro.h>
#include <stdio.h>
#include <math.h>

#define TIMER 20000
void drawBar(int x,int y, float Height){

if(Height <120)
{ 
  for (x = 15; x < 25; x++){
    for (y = 75; y > 75/(Height); y--){
      f3d_lcd_drawPixel(x, y, RED);
    }
  }
 }else{
  for (x = 15; x < 25; x++){
    for (y = 75; y > 75/(120); y--){
      f3d_lcd_drawPixel(x, y, RED);
    }
  }
 }
 
 
}
void drawCircle(int x0, int y0, int radius)
{
  int x = radius-1;
  int y = 0;
  int dx = 1;
  int dy = 1;
  int err = dx - (radius << 1);
  
  while (x >= y)
    {
      f3d_lcd_drawPixel(x0 + x, y0 + y, RED);
      f3d_lcd_drawPixel(x0 + y, y0 + x, RED);
      f3d_lcd_drawPixel(x0 - y, y0 + x, RED);
      f3d_lcd_drawPixel(x0 - x, y0 + y, RED);
      f3d_lcd_drawPixel(x0 - x, y0 - y, RED);
      f3d_lcd_drawPixel(x0 - y, y0 - x, RED);
      f3d_lcd_drawPixel(x0 + y, y0 - x, RED);
      f3d_lcd_drawPixel(x0 + x, y0 - y, RED);
      
      if (err <= 0)
	{
	  y++;
	  err += dy;
	  dy += 2;
	}
      if (err > 0)
	{
	  x--;
	  dx += 2;
	  err += (-radius << 1) + dx;
	}
    }
}


int main(void) {
  f3d_uart_init();
  f3d_lcd_init();
  f3d_user_btn_init();
  f3d_gyro_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);
  
  nunchuk_t nunchuk = {0, 0, 0, 0, 0, 0, 0};
  float Accel[3], Mag[3],test[3], Pitch, Roll, Heading, Xh, Yh, accXnorm, accYnorm;
  int x , y, jx1, jy1;
  int counter = 0;
  f3d_lcd_fillScreen2(WHITE);
  drawCircle(65, 80, 40);
 
  float basePitch, baseRoll;
  f3d_nunchuk_read(&nunchuk);
  basePitch = atan(nunchuk.ax/(sqrt(pow(nunchuk.ay, 2) + pow(nunchuk.az, 2)))); //Pitch
  baseRoll = atan(nunchuk.ay/(sqrt(pow(nunchuk.ax, 2) + pow(nunchuk.az, 2)))); //Roll

  while(1){
    
    f3d_nunchuk_read(&nunchuk);
    delay(20);
    //printf("%hu\t", nunchuk.ax);
    //printf("%hu\t", nunchuk.ay);
    //printf("%hu\n", nunchuk.az);
    if(nunchuk.jx != 127){
      jx1 = nunchuk.jx;
      jy1 = 128;
    }
    if(nunchuk.jy != 128){
      jy1 = nunchuk.jy;
      jx1 = 127;
    }
    /////////////Compass (RIGHT)
      if(jx1 == 255){
	f3d_lcd_fillScreen2(WHITE);
	drawCircle(65,80,40);
	f3d_mag_read(Mag);
	f3d_accel_read(Accel);
	
	Pitch = atan(Accel[0]/(sqrt(pow(Accel[1], 2) + pow(Accel[2], 2)))); //Pitch
	//printf("%f\n", Pitch);
	Roll = atan(Accel[1]/(sqrt(pow(Accel[0], 2) + pow(Accel[2], 2)))); //Roll
	//printf("%f\n", Pitch);
	Xh = (Mag[0] * cos(Pitch)) + (Mag[2] * sin(Pitch)); //Compensated Xh
	Yh = (Mag[0]*sin(Roll)*sin(Pitch)) + (Mag[1]*cos(Roll)) -
	  (Mag[2]*sin(Roll)*cos(Pitch)); //Compensated Yh
	Heading = atan2(Yh,Xh); //Heading
	//if(Heading < 0)
	// Heading += 360;
	
	//printf("%f\n", Heading);
	//delay();
	int theta = 360 - Heading;
	int X = 40*cos(theta) + 65;
	int Y = 40*sin(theta) + 80;
	
	drawCircle(X,Y,10);
	//f3d_lcd_drawChar(X, Y, 'N', BLACK, WHITE);
	//delay(10);
	//f3d_lcd_drawChar(X, Y, 'N', WHITE, WHITE);
      }

      //////////////Tilt Angle (LEFT)
      if(jx1 == 0){
	f3d_lcd_fillScreen2(WHITE);
	f3d_mag_read(Mag);
	f3d_accel_read(Accel);
	
	Pitch = atan(Accel[0]/(sqrt(pow(Accel[1], 2) + pow(Accel[2], 2)))); //Pitch
	//printf("%f\n", Pitch);
	Roll = atan(Accel[1]/(sqrt(pow(Accel[0], 2) + pow(Accel[2], 2)))); //Roll
	drawCircle(-(Roll*57.2958)+80,-(Pitch*57.2958)+60,4);
      }

      //////////Gyro (UP)
      if(jy1 == 255){
	f3d_gyro_getdata(test);
	f3d_lcd_fillScreen2(WHITE); //Clear screen
	
	//Adding some graphics
	f3d_lcd_drawChar(120, 120, '-', BLACK, WHITE);
	f3d_lcd_drawChar(120, 40, '+', BLACK, WHITE);
	f3d_lcd_drawChar(100, 80, 'Z', BLACK, WHITE);
	f3d_lcd_drawChar(60, 80, 'Y', BLACK, WHITE);
	f3d_lcd_drawChar(20, 80, 'X', BLACK, WHITE);
	int x, y;
	
	//////X AXIS
	if(test[0] > 0)
	  for (x = 15; x < 25; x++)
	    for (y = 75; y > 75/(test[0]); y--)
	      f3d_lcd_drawPixel(x, y, RED);
	//Negative
	else if(test[0] < 0)
	  for (x = 15; x < 25; x++)
	    for (y = 85; y < (-1*(70/test[0]+85)); y++)
	      f3d_lcd_drawPixel(x, y, RED);
	
	//////Y AXIS
	if(test[1] > 0)
	  for (x = 55; x < 65; x++)
	    for (y = 75; y > 75/(test[1]); y--)
	      f3d_lcd_drawPixel(x, y, BLUE); 
	//Negative
	else if(test[1] < 0)
	  for (x = 55; x < 65; x++)
	    for (y = 85; y < -1*(70/test[1]+85); y++)
	      f3d_lcd_drawPixel(x, y, BLUE);
	
	//////Z AXIS
	if(test[2] > 0)
	  for (x = 95; x < 105; x++)
	    for (y = 75; y > 75/(test[2]); y--)
	      f3d_lcd_drawPixel(x, y, GREEN);
	//Negative
	else if(test[2] < 0)
	  for (x = 95; x < 105; x++)
	    for (y = 85; y < -1*(70/test[2]+85); y++)
	      f3d_lcd_drawPixel(x, y, GREEN);
	
      }

      ///////////Nunchuk (DOWN)
      if(jy1 == 0){
	f3d_nunchuk_read(&nunchuk);
	f3d_lcd_fillScreen2(WHITE); //Clear screen
	Pitch = atan(nunchuk.ax/(sqrt(pow(nunchuk.ay, 2) + pow(nunchuk.az, 2)))); //Pitch
	Roll = atan(nunchuk.ay/(sqrt(pow(nunchuk.ax, 2) + pow(nunchuk.az, 2)))); //Roll
	//printf("%f\t", Pitch);
	//printf("%f\n", Roll);
	drawCircle((baseRoll-Roll)*100+80, (basePitch-Pitch)*100+65,sqrt(nunchuk.az/10)+4);
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


