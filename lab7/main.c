/* main.c ---
*
* Filename: main.c
* Description:
* Author:Caulin Jones and Alex Koopman
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
/*
void rotate(){
//tilt angles 3.1
int rad = angle*M_PI/180;
newx = x * cos(rad) - y * sin(rad);
newy = y * cos(rad) + x * sin(rad);
}
*/
//equation 1: Heading = arcttan(Yh/Xh)

//equation 2: Xh = Xm * cosPitch + Zm * sinPitch
// Yh = Xm * sinRollsinPitch + Ym * cosRoll - Zm * sinRollcosPitch
// where Xm, Ym, and Zm are mag sensor measurements
//Pitch = arctan(Accelx1/sqrt((Accely1)**2+(Accelz1)**2)
//Roll = arctan(Accely1/sqrt((Accelx1)**2+(Accelz1)**2)


int main(void) {
f3d_uart_init();
f3d_lcd_init();
setvbuf(stdin, NULL, _IONBF, 0);
setvbuf(stdout, NULL, _IONBF, 0);
setvbuf(stderr, NULL, _IONBF, 0);

f3d_i2c1_init();
delay(10);
f3d_accel_init();
delay(10);
f3d_mag_init();
delay(10);

 float Accel[3], Mag[3], Pitch, Roll, Heading, Xh, Yh, accXnorm, accYnorm;
 int x , y;
f3d_lcd_fillScreen2(WHITE);
drawCircle(65, 80, 40);

while(1){
  f3d_lcd_fillScreen2(WHITE);
  drawCircle(65,80,40);
  f3d_mag_read(Mag);
  f3d_accel_read(Accel);

Pitch = atan(Accel[0]/(sqrt(pow(Accel[1], 2) + pow(Accel[2], 2)))); //Pitch
//printf("%f\n", Pitch);
Roll = atan(Accel[1]/(sqrt(pow(Accel[0], 2) + pow(Accel[2], 2)))); //Roll
//printf("%f\n", Pitch);
 drawCircle(-(Roll*57.2958)+80,-(Pitch*57.2958)+60,4);
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
// Set up your inits before you go ahead


}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
while (1);
}
#endif

/* main.c ends here */
