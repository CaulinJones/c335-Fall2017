/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 12/1/17
 *           By: Alex Koopman, Caulin Jones
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> 
#include <circleObject.h>
#include <f3d_uart.h>
#include <f3d_dac.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_i2c2.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_gyro.h>
#include <f3d_rtc.h>
#include <f3d_dac.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#define TIMER 20000

#define AUDIOBUFSIZE 128

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

FATFS Fatfs;		/* File system object */
FIL fid;		/* File object */
BYTE Buff[512];		/* File read buffer */
int ret;


struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

void playAudio(char* sound){
  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;
  
  f_mount(0, &Fatfs);/* Register volume work area */
  
  // printf("\nOpen thermo.wav\n");
  rc =f_open(&fid, sound, FA_READ);
  // do this for each filename based off nunchuck info
  if (!rc) {
    struct ckhd hd;
    uint32_t  waveid;
    struct fmtck fck;
    
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    // if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
    //    return -1;
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }

    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
  
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//128x160
int arm = 2;
struct circleObject circle[6];
void makeCircles(){
  //circle[0] p1body
  circle[0].x = 60;
  circle[0].y = 40;
  circle[0].radius = 10;
  circle[0].block = 0;

  //circle[1] p2body
  circle[1].x = 60;
  circle[1].y = 120;
  circle[1].radius = 10;
  circle[1].block = 0;

  //circle[2] p1 left hand 
  circle[2].x = 75;
  circle[2].y = 45;
  circle[2].radius = 5;
  circle[2].block = 0;

  //circle[3] p1 right hand
  circle[3].x = 45;
  circle[3].y = 45;
  circle[3].radius = 5;
  circle[3].block = 0;

  //cirlce[4] p2 left hand
  circle[4].x = 75;
  circle[4].y = 115;
  circle[4].radius = 5;
  circle[4].block = 0;

  //circle[5] p2 right hand
  circle[5].x = 45;
  circle[5].y = 115;
  circle[5].radius = 5;
  circle[5].block = 0;
}
void moveBoxer1(int jx, int jy){
  if(jy > 245 && (jx > 120 && jx < 145) && (circle[2].y < 143)){
    //up
    circle2(circle[2].x, circle[2].y, 4, 5, WHITE, 2);
    circle2(circle[3].x, circle[3].y, 4, 5, WHITE, 2);
    circle2(circle[0].x, circle[0].y, 9, 10,  WHITE, 2);

    circle2(circle[2].x, circle[2].y+=2, 4, 5, BLUE, 1);
    circle2(circle[2].x, circle[2].y, 4, 5, BLUE, 2);

    circle2(circle[3].x, circle[3].y+=2, 4, 5, BLUE, 1);
    circle2(circle[3].x, circle[3].y, 4, 5, BLUE, 2);

    circle2(circle[0].x, circle[0].y+=2, 9, 10,  RED, 1);
    circle2(circle[0].x, circle[0].y, 9, 10,  RED, 2);
    
  }
  else if(jy < 100 && (jx > 120 && jx < 145) && (circle[0].y > 21)){
    
    circle2(circle[2].x, circle[2].y, 4, 5, WHITE, 1);
    circle2(circle[3].x, circle[3].y, 4, 5, WHITE, 1);
    circle2(circle[0].x, circle[0].y, 9, 10,  WHITE, 1);

    circle2(circle[2].x, circle[2].y-=2, 4, 5, BLUE, 2);
    circle2(circle[2].x, circle[2].y, 4, 5, BLUE, 1);

    circle2(circle[3].x, circle[3].y-=2, 4, 5, BLUE, 2);
    circle2(circle[3].x, circle[3].y, 4, 5, BLUE, 1);

    circle2(circle[0].x, circle[0].y-=2, 9, 10, RED, 2);
    circle2(circle[0].x, circle[0].y, 9, 10, RED, 1);

  }
  else if(jx > 245 && (jy > 120 && jy < 145) && (circle[3].x > 16)){

    circle2(circle[2].x, circle[2].y, 4, 5, WHITE, 3);
    circle2(circle[3].x, circle[3].y, 4, 5, WHITE, 3);
    circle2(circle[0].x, circle[0].y, 9, 10,  WHITE, 3);

    circle2(circle[2].x-=2, circle[2].y, 4, 5, BLUE, 4);
    circle2(circle[2].x, circle[2].y, 4, 5, BLUE, 3);

    circle2(circle[3].x-=2, circle[3].y, 4, 5, BLUE, 4);
    circle2(circle[3].x, circle[3].y, 4, 5, BLUE, 3);

    circle2(circle[0].x-=2, circle[0].y, 9, 10, RED, 4);
    circle2(circle[0].x, circle[0].y, 9, 10, RED, 3);
    
   
  }
  else if(jx < 100 && (jy > 120 && jy < 145) && (circle[2].x < 112)){
    circle2(circle[2].x, circle[2].y, 4, 5, WHITE, 4);
    circle2(circle[3].x, circle[3].y, 4, 5, WHITE, 4);
    circle2(circle[0].x, circle[0].y, 9, 10,  WHITE, 4);

    circle2(circle[2].x+=2, circle[2].y, 4, 5, BLUE, 3);
    circle2(circle[2].x, circle[2].y, 4, 5, BLUE, 4);

    circle2(circle[3].x+=2, circle[3].y, 4, 5, BLUE, 3);
    circle2(circle[3].x, circle[3].y, 4, 5, BLUE, 4);

    circle2(circle[0].x+=2, circle[0].y, 9, 10, RED, 3);
    circle2(circle[0].x, circle[0].y, 9, 10, RED, 4);
    
   
  }
  //diagonals
  else if(jx > 145 && jy > 145 && (circle[3].x > 16 && circle[3].y < 143)){
    circle2(circle[2].x, circle[2].y, 4, 5, WHITE, 6);
    circle2(circle[3].x, circle[3].y, 4, 5, WHITE, 6);
    circle2(circle[0].x, circle[0].y, 9, 10,  WHITE, 6);

    circle2(circle[2].x-=1, circle[2].y+=1, 4, 5, BLUE, 5);
    circle2(circle[2].x, circle[2].y, 4, 5, BLUE, 6);

    circle2(circle[3].x-=1, circle[3].y+=1, 4, 5, BLUE, 5);
    circle2(circle[3].x, circle[3].y, 4, 5, BLUE, 6);

    circle2(circle[0].x-=1, circle[0].y+=1, 9, 10, RED, 5);
    circle2(circle[0].x, circle[0].y, 9, 10, RED, 6);
    
   
  }
  else if(jx < 130 && jy < 130 && (circle[2].x < 112 && circle[0].y > 21)){
    circle2(circle[2].x, circle[2].y, 4, 5, WHITE, 5);
    circle2(circle[3].x, circle[3].y, 4, 5, WHITE, 5);
    circle2(circle[0].x, circle[0].y, 9, 10,  WHITE, 5);

    circle2(circle[2].x+=1, circle[2].y-=1, 4, 5, BLUE, 6);
    circle2(circle[2].x, circle[2].y, 4, 5, BLUE, 5);

    circle2(circle[3].x+=1, circle[3].y-=1, 4, 5, BLUE, 6);
    circle2(circle[3].x, circle[3].y, 4, 5, BLUE, 5);

    circle2(circle[0].x+=1, circle[0].y-=1, 9, 10, RED, 6);
    circle2(circle[0].x, circle[0].y, 9, 10, RED, 5);
    
   
  }
  else if(jx > 145 && jy < 130 && (circle[3].x > 16 && circle[0].y > 21)){
    circle2(circle[2].x, circle[2].y, 4, 5, WHITE, 8);
    circle2(circle[3].x, circle[3].y, 4, 5, WHITE, 8);
    circle2(circle[0].x, circle[0].y, 9, 10,  WHITE, 8);

    circle2(circle[2].x-=1, circle[2].y-=1, 4, 5, BLUE, 7);
    circle2(circle[2].x, circle[2].y, 4, 5, BLUE, 8);

    circle2(circle[3].x-=1, circle[3].y-=1, 4, 5, BLUE, 7);
    circle2(circle[3].x, circle[3].y, 4, 5, BLUE, 8);

    circle2(circle[0].x-=1, circle[0].y-=1, 9, 10, RED, 7);
    circle2(circle[0].x, circle[0].y, 9, 10, RED, 8);
    
   
  }
  else if(jx < 130 && jy > 145 && (circle[2].x < 112 && circle[2].y < 143)){
    circle2(circle[2].x, circle[2].y, 4, 5, WHITE, 7);
    circle2(circle[3].x, circle[3].y, 4, 5, WHITE, 7);
    circle2(circle[0].x, circle[0].y, 9, 10,  WHITE, 7);

    circle2(circle[2].x+=1, circle[2].y+=1, 4, 5, BLUE, 8);
    circle2(circle[2].x, circle[2].y, 4, 5, BLUE, 7);

    circle2(circle[3].x+=1, circle[3].y+=1, 4, 5, BLUE, 8);
    circle2(circle[3].x, circle[3].y, 4, 5, BLUE, 7);

    circle2(circle[0].x+=1, circle[0].y+=1, 9, 10, RED, 8);
    circle2(circle[0].x, circle[0].y, 9, 10, RED, 7);
    
   
  }
}

void moveBoxer2(int jx, int jy){
  if(jy > 245 && (jx > 120 && jx < 145) && (circle[4].y > 17)){
    circle2(circle[4].x, circle[4].y, 4, 5, WHITE, 1);
    circle2(circle[5].x, circle[5].y, 4, 5, WHITE, 1);
    circle2(circle[1].x, circle[1].y, 9, 10,  WHITE, 1);

    circle2(circle[4].x, circle[4].y-=2, 4, 5, RED, 2);
    circle2(circle[4].x, circle[4].y, 4, 5, RED, 1);

    circle2(circle[5].x, circle[5].y-=2, 4, 5, RED, 2);
    circle2(circle[5].x, circle[5].y, 4, 5, RED, 1);

    circle2(circle[1].x, circle[1].y-=2, 9, 10, BLUE, 2);
    circle2(circle[1].x, circle[1].y, 9, 10, BLUE, 1);
   
    
  }
  else if(jy < 100 && (jx > 120 && jx < 145) && (circle[1].y < 139)){

    circle2(circle[4].x, circle[4].y, 4, 5, WHITE, 2);
    circle2(circle[5].x, circle[5].y, 4, 5, WHITE, 2);
    circle2(circle[1].x, circle[1].y, 9, 10,  WHITE, 2);

    circle2(circle[4].x, circle[4].y+=2, 4, 5, RED, 1);
    circle2(circle[4].x, circle[4].y, 4, 5, RED, 2);

    circle2(circle[5].x, circle[5].y+=2, 4, 5, RED, 1);
    circle2(circle[5].x, circle[5].y, 4, 5, RED, 2);

    circle2(circle[1].x, circle[1].y+=2, 9, 10,  BLUE, 1);
    circle2(circle[1].x, circle[1].y, 9, 10,  BLUE, 2);
    
   

  }
  else if(jx > 245 && (jy > 120 && jy < 145) && (circle[4].x < 112)){
    circle2(circle[4].x, circle[4].y, 4, 5, WHITE, 4);
    circle2(circle[5].x, circle[5].y, 4, 5, WHITE, 4);
    circle2(circle[1].x, circle[1].y, 9, 10,  WHITE, 4);

    circle2(circle[4].x+=2, circle[4].y, 4, 5, RED, 3);
    circle2(circle[4].x, circle[4].y, 4, 5, RED, 4);

    circle2(circle[5].x+=2, circle[5].y, 4, 5, RED, 3);
    circle2(circle[5].x, circle[5].y, 4, 5, RED, 4);

    circle2(circle[1].x+=2, circle[1].y, 9, 10, BLUE, 3);
    circle2(circle[1].x, circle[1].y, 9, 10, BLUE, 4);
   
    
   
  }
  else if(jx < 100 && (jy > 120 && jy < 145) && (circle[5].x > 16)){
    
    
    circle2(circle[4].x, circle[4].y, 4, 5, WHITE, 3);
    circle2(circle[5].x, circle[5].y, 4, 5, WHITE, 3);
    circle2(circle[1].x, circle[1].y, 9, 10,  WHITE, 3);

    circle2(circle[4].x-=2, circle[4].y, 4, 5, RED, 4);
    circle2(circle[4].x, circle[4].y, 4, 5, RED, 3);

    circle2(circle[5].x-=2, circle[5].y, 4, 5, RED, 4);
    circle2(circle[5].x, circle[5].y, 4, 5, RED, 3);

    circle2(circle[1].x-=2, circle[1].y, 9, 10, BLUE, 4);
    circle2(circle[1].x, circle[1].y, 9, 10, BLUE, 3);
   
  }
 //diagonals
  else if(jx > 145 && jy > 145 && (circle[4].x < 112 && circle[4].y > 17)){
    circle2(circle[4].x, circle[4].y, 4, 5, WHITE, 5);
    circle2(circle[5].x, circle[5].y, 4, 5, WHITE, 5);
    circle2(circle[1].x, circle[1].y, 9, 10,  WHITE, 5);

    circle2(circle[4].x+=1, circle[4].y-=1, 4, 5, RED, 6);
    circle2(circle[4].x, circle[4].y, 4, 5, RED, 5);

    circle2(circle[5].x+=1, circle[5].y-=1, 4, 5, RED, 6);
    circle2(circle[5].x, circle[5].y, 4, 5, RED, 5);

    circle2(circle[1].x+=1, circle[1].y-=1, 9, 10, BLUE, 6);
    circle2(circle[1].x, circle[1].y, 9, 10, BLUE, 5);
    
   
  }
  else if((jx < 125 && jy < 130) && (circle[5].x > 16 && circle[1].y < 139)){
    circle2(circle[4].x, circle[4].y, 4, 5, WHITE, 6);
    circle2(circle[5].x, circle[5].y, 4, 5, WHITE, 6);
    circle2(circle[1].x, circle[1].y, 9, 10,  WHITE, 6);

    circle2(circle[4].x-=1, circle[4].y+=1, 4, 5, RED, 5);
    circle2(circle[4].x, circle[4].y, 4, 5, RED, 6);

    circle2(circle[5].x-=1, circle[5].y+=1, 4, 5, RED, 5);
    circle2(circle[5].x, circle[5].y, 4, 5, RED, 6);

    circle2(circle[1].x-=1, circle[1].y+=1, 9, 10, BLUE, 5);
    circle2(circle[1].x, circle[1].y, 9, 10, BLUE, 6);
    
   
  }
 else if((jx > 145 && jy < 125) && (circle[4].x < 112 && circle[1].y < 139)){
    circle2(circle[4].x, circle[4].y, 4, 5, WHITE, 7);
    circle2(circle[5].x, circle[5].y, 4, 5, WHITE, 7);
    circle2(circle[1].x, circle[1].y, 9, 10,  WHITE, 7);

    circle2(circle[4].x+=1, circle[4].y+=1, 4, 5, RED, 8);
    circle2(circle[4].x, circle[4].y, 4, 5, RED, 7);

    circle2(circle[5].x+=1, circle[5].y+=1, 4, 5, RED, 8);
    circle2(circle[5].x, circle[5].y, 4, 5, RED, 7);

    circle2(circle[1].x+=1, circle[1].y+=1, 9, 10, BLUE, 8);
    circle2(circle[1].x, circle[1].y, 9, 10, BLUE, 7);
    
   
  }
 else if((jx < 125 && jy > 145) && (circle[5].x > 16 && circle[5].y > 17)){
    circle2(circle[4].x, circle[4].y, 4, 5, WHITE, 8);
    circle2(circle[5].x, circle[5].y, 4, 5, WHITE, 8);
    circle2(circle[1].x, circle[1].y, 9, 10,  WHITE, 8);

    circle2(circle[4].x-=1, circle[4].y-=1, 4, 5, RED, 7); 
    circle2(circle[4].x, circle[4].y, 4, 5, RED, 8);

    circle2(circle[5].x-=1, circle[5].y-=1, 4, 5, RED, 7);
    circle2(circle[5].x, circle[5].y, 4, 5, RED, 8);

    circle2(circle[1].x-=1, circle[1].y-=1, 9, 10, BLUE, 7);
    circle2(circle[1].x, circle[1].y, 9, 10, BLUE, 8);
    
    }
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

int radiusCheck(int x1 ,int y1, int x2,int y2, int rad1, int rad2 ){
  int out = 0;
  double square_difference_x =(double) ((x2 - x1) * (x2 - x1));
  double square_difference_y =(double) ((y2 - y1) * (y2 - y1));
  double sum = square_difference_x + square_difference_y;
  double value = sqrt(sum);
  double dist =(double) rad1+rad2;
  if(value <= dist){
    out = 1;
  }
  return out;
}
int p1accavg = 0;
int p2accavg = 0;
int p1Health = 20;
int p2Health = 20;

void p1Bounce(){
  //will animate p1 back and check collision between pixel redraws
  moveBoxer1(138, 20);
}
void p2Bounce(){
  //animate p2 back and check collision between pixel redraws
  moveBoxer2(138, 20);
}
 bothBounce(){
  p1Bounce();
  p2Bounce();
}

void p1loseHealth(){
  //p1healthfield loses 
  p1Health--;
  p1Bounce();
}
void p2loseHealth(){
  p2Health--;
  p2Bounce();
}

int collisionCheck(){
  //need collishion check for boarders
  int out = 0;
    //p1body collision check
   //if p1body hit p2body bouncd back both players in bounch back function
  if (radiusCheck(circle[0].x,circle[0].y,circle[1].x,circle[1].y,circle[0].radius,circle[1].radius)){ bothBounce();out = 1;}  // p1body Ch p2body | both bounce back
  if (radiusCheck(circle[2].x,circle[2].y,circle[4].x,circle[4].y,circle[2].radius,circle[4].radius)){ p1Bounce();out = 1;}  // p1lhand check p2lhand | p1 bounce back
  if (radiusCheck(circle[2].x,circle[2].y,circle[5].x,circle[5].y,circle[2].radius,circle[5].radius)){ p1Bounce();out = 1;}  // p1lhand check p2Rhand | p1 boucne back
  if (radiusCheck(circle[4].x,circle[4].y,circle[2].x,circle[2].y,circle[4].radius,circle[2].radius)){ p2Bounce();out = 1;} // p2lhand check p1lhand | p2 bounces back
  if (radiusCheck(circle[4].x,circle[4].y,circle[3].x,circle[3].y,circle[4].radius,circle[3].radius)){ p2Bounce();out = 1;} //p2lhand chekc p1Rhand  | p2 boucne back
  if (radiusCheck(circle[3].x,circle[3].y,circle[4].x,circle[4].y,circle[3].radius,circle[4].radius)){ p1Bounce();out = 1;} //p1Rhand check p2lhand  | p1 bounce back
  if (radiusCheck(circle[3].x,circle[3].y,circle[5].x,circle[5].y,circle[3].radius,circle[5].radius)){ p1Bounce();out = 1;} //p1Rhand hcekc p2Rhand  | p1 bounce back
  if (radiusCheck(circle[5].x,circle[5].y,circle[3].x,circle[3].y,circle[5].radius,circle[3].radius)){ p2Bounce();out = 1;} //p2Rhand check p1Rhand  | p2 bounce back
  if (radiusCheck(circle[5].x,circle[5].y,circle[2].x,circle[2].y,circle[5].radius,circle[2].radius)){ p2Bounce();out = 1;} //p2Rhand chek p1Lhand   | p2 boucne back
  if (radiusCheck(circle[2].x,circle[2].y,circle[1].x,circle[1].y,circle[2].radius,circle[1].radius)){ p2loseHealth();out = 1;} //p1lhand check p2body   | p2 lose health
  if (radiusCheck(circle[4].x,circle[4].y,circle[0].x,circle[0].x,circle[4].radius,circle[0].radius)){ p1loseHealth();out = 1;} //p2lhand hcekc p1body   | p1 lose health
  if (radiusCheck(circle[5].x,circle[5].y,circle[0].x,circle[0].y,circle[5].radius,circle[0].radius)){ p1loseHealth();out = 1;} //p2rhand hcekc p1body   | p1 lose health
  if (radiusCheck(circle[3].x,circle[3].y,circle[1].x,circle[1].y,circle[3].radius,circle[0].radius)){ p2loseHealth();out = 1;} //p1Rhand check p2body   | p2 lose health
  return out;
}

void boxer1Block(int i){
  if(circle[0].block == 1 && i == 0){
    circle2perim(circle[2].x, circle[2].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, WHITE);
    
    circle2perim(circle[3].x, circle[3].y, 4, 5, WHITE);//hand2
    DrawFilledCircle(circle[3].x, circle[3].y, 4, WHITE);
    
    circle[0].block = 0;
    circle[2].x += 8;
    circle[2].y -= 8;
    circle[3].x -= 8;
    circle[3].y -= 8;

    circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);
    
    circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand2
    DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);
  
  }
  else if(i && circle[0].block == 0){
    circle2perim(circle[2].x, circle[2].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, WHITE);
  
    circle2perim(circle[3].x, circle[3].y, 4, 5, WHITE);//hand2
    DrawFilledCircle(circle[3].x, circle[3].y, 4, WHITE);
    
    circle[0].block = 1;
    circle[2].x -= 8;
    circle[2].y += 8;
    circle[3].x += 8;
    circle[3].y += 8;

    circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);

    circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand2
    DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);
  }
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void boxer1Punch(){
  if(arm % 2){
    //set fists to front
    circle2perim(circle[2].x, circle[2].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, WHITE);
  
    circle[2].x -= 11;
    circle[2].y += 11;
    collisionCheck();
    circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);
  
    //extend
    circle2perim(circle[2].x, circle[2].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, WHITE);
  
    circle[2].y += 5;
    collisionCheck();
    circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);

    //unextend
    circle2perim(circle[2].x, circle[2].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, WHITE);
  
    circle[2].y -= 5;
 
    circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);

    //set back to side
    circle2perim(circle[2].x, circle[2].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, WHITE);
  
    
    circle[2].x += 11;
    circle[2].y -= 11;
 
    circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
    DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);

  }
  else{
    //set fists to front
    circle2perim(circle[3].x, circle[3].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[3].x, circle[3].y, 4, WHITE);
  
    circle[3].x += 11;
    circle[3].y += 11;
    collisionCheck();
    circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand1
    DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);
  
    //extend
    circle2perim(circle[3].x, circle[3].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[3].x, circle[3].y, 4, WHITE);
  
    circle[3].y += 5;
    collisionCheck();
    circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand1
    DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);

    //unextend
    circle2perim(circle[3].x, circle[3].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[3].x, circle[3].y, 4, WHITE);
  
    circle[3].y -= 5;
 
    circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand1
    DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);

    //set back to side
    circle2perim(circle[3].x, circle[3].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[3].x, circle[3].y, 4, WHITE);
  
    circle[3].x -= 11;
    circle[3].y -= 11;
 
    circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand1
    DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);
  }
  arm++;

}
void boxer1Ultimate(){
  //set fists to front
  circle2perim(circle[2].x, circle[2].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[2].x, circle[2].y, 4, WHITE);
  
  circle[2].x -= 11;
  circle[2].y += 11;
  collisionCheck();
  circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
  DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);

  //set fists to front
  circle2perim(circle[3].x, circle[3].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[3].x, circle[3].y, 4, WHITE);
  
  circle[3].x += 11;
  circle[3].y += 11;
  collisionCheck();
  circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand1
  DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);
  
  //extend
  circle2perim(circle[2].x, circle[2].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[2].x, circle[2].y, 4, WHITE);
  
  circle[2].y += 5;
  collisionCheck();
  circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
  DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);
  //extend
  circle2perim(circle[3].x, circle[3].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[3].x, circle[3].y, 4, WHITE);
  
  circle[3].y += 5;
  collisionCheck();
  circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand1
  DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);

  //unextend
  circle2perim(circle[2].x, circle[2].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[2].x, circle[2].y, 4, WHITE);
  
  circle[2].y -= 5;
 
  circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
  DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);
  //unextend
  circle2perim(circle[3].x, circle[3].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[3].x, circle[3].y, 4, WHITE);
  
  circle[3].y -= 5;
 
  circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand1
  DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);

  //set back to side
  circle2perim(circle[2].x, circle[2].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[2].x, circle[2].y, 4, WHITE);
  
    
  circle[2].x += 11;
  circle[2].y -= 11;
 
  circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
  DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);
  //set back to side
  circle2perim(circle[3].x, circle[3].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[3].x, circle[3].y, 4, WHITE);
  
  circle[3].x -= 11;
  circle[3].y -= 11;
 
  circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand1
  DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);
  
}

void boxer2Block(int i){
  if(circle[1].block == 1 && i == 0){
    circle2perim(circle[4].x, circle[4].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, WHITE);
    
    circle2perim(circle[5].x, circle[5].y, 4, 5, WHITE);//hand2
    DrawFilledCircle(circle[5].x, circle[5].y, 4, WHITE);
    
    circle[1].block = 0;
    circle[4].x += 8;
    circle[4].y += 8;
    circle[5].x -= 8;
    circle[5].y += 8;
    collisionCheck();
    circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);
    
    circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand2
    DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);
  
  }
  else if(i && circle[1].block == 0){
    circle2perim(circle[4].x, circle[4].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, WHITE);
  
    circle2perim(circle[5].x, circle[5].y, 4, 5, WHITE);//hand2
    DrawFilledCircle(circle[5].x, circle[5].y, 4, WHITE);
    
    circle[1].block = 1;
    circle[4].x -= 8;
    circle[4].y -= 8;
    circle[5].x += 8;
    circle[5].y -= 8;
    collisionCheck();
    circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);

    circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand2
    DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);
  }
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void boxer2Punch(){
  if(arm % 2){
    //set fists to front
    circle2perim(circle[4].x, circle[4].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, WHITE);
  
    circle[4].x -= 11;
    circle[4].y -= 11;
    collisionCheck();
    circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);
  
    //extend
    circle2perim(circle[4].x, circle[4].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, WHITE);
  
    circle[4].y -= 5;
    collisionCheck();
    circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);

    //unextend
    circle2perim(circle[4].x, circle[4].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, WHITE);
  
    circle[4].y += 5;
    collisionCheck();
    circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);

    //set back to side
    circle2perim(circle[4].x, circle[4].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, WHITE);
  
    
    circle[4].x += 11;
    circle[4].y += 11;
 
    circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
    DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);

  }
  else{
    //set fists to front
    circle2perim(circle[5].x, circle[5].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[5].x, circle[5].y, 4, WHITE);
  
    circle[5].x += 11;
    circle[5].y -= 11;
    collisionCheck();
    circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand1
    DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);
  
    //extend
    circle2perim(circle[5].x, circle[5].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[5].x, circle[5].y, 4, WHITE);
  
    circle[5].y -= 5;
    collisionCheck();
    circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand1
    DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);

    //unextend
    circle2perim(circle[5].x, circle[5].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[5].x, circle[5].y, 4, WHITE);
  
    circle[5].y += 5;
 
    circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand1
    DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);

    //set back to side
    circle2perim(circle[5].x, circle[5].y, 4, 5, WHITE);//hand1
    DrawFilledCircle(circle[5].x, circle[5].y, 4, WHITE);
  
    circle[5].x -= 11;
    circle[5].y += 11;
 
    circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand1
    DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);
  }
  arm++;

}

void boxer2Ultimate(){
 
  //set fists to front
  circle2perim(circle[4].x, circle[4].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[4].x, circle[4].y, 4, WHITE);
  
  circle[4].x -= 11;
  circle[4].y -= 11;
  collisionCheck();
  circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
  DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);
  //set fists to front
  circle2perim(circle[5].x, circle[5].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[5].x, circle[5].y, 4, WHITE);
  
  circle[5].x += 11;
  circle[5].y -= 11;
  collisionCheck();
  circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand1
  DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);
  
  //extend
  circle2perim(circle[4].x, circle[4].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[4].x, circle[4].y, 4, WHITE);
  
  circle[4].y -= 5;
  collisionCheck();
  circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
  DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);
  //extend
  circle2perim(circle[5].x, circle[5].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[5].x, circle[5].y, 4, WHITE);
  
  circle[5].y -= 5;
  collisionCheck();
  circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand1
  DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);

  //unextend
  circle2perim(circle[4].x, circle[4].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[4].x, circle[4].y, 4, WHITE);
  
  circle[4].y += 5;
  collisionCheck();
  circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
  DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);
  //unextend
  circle2perim(circle[5].x, circle[5].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[5].x, circle[5].y, 4, WHITE);
  
  circle[5].y += 5;
 
  circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand1
  DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);


  //set back to side
  circle2perim(circle[4].x, circle[4].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[4].x, circle[4].y, 4, WHITE);
  
    
  circle[4].x += 11;
  circle[4].y += 11;
 
  circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
  DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);
  //set back to side
  circle2perim(circle[5].x, circle[5].y, 4, 5, WHITE);//hand1
  DrawFilledCircle(circle[5].x, circle[5].y, 4, WHITE);
  
  circle[5].x -= 11;
  circle[5].y += 11;
 
  circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand1
  DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);

}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void checkHealth(){
  if(p1Health <= 15)
    f3d_led_off(11);
  if(p1Health <= 10)
    f3d_led_off(10);
  if(p1Health <= 5)
    f3d_led_off(9);
  if(p1Health <= 0)
    f3d_led_off(8);
  
  if(p2Health <= 15)
    f3d_led_off(15);
  if(p2Health <= 10)
    f3d_led_off(14);
  if(p2Health <= 5)
    f3d_led_off(13);
  if(p2Health <= 0)
    f3d_led_off(12);
}

int main(void) {

  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;

  f3d_led_init();
  f3d_uart_init();
  delay(10);
  f3d_timer2_init();
  delay(10);
  f3d_rtc_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_dac_init();
  delay(10); 
  f3d_delay_init();
  
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_i2c1_init();
  delay(10);

  f3d_i2c2_init();
  delay(10);
  printf("inits");
  f3d_nunchuk_init();
  delay(10);
  printf("yo");

  playAudio("ring.wav");
  
  delay(10);
  nunchuk_t nunchuk1 = {0, 0, 0, 0, 0, 0, 0};
  nunchuk_t nunchuk2 = {0, 0, 0, 0, 0, 0, 0};

  makeCircles();
  f3d_lcd_fillScreen2(WHITE);
  drawStage(10, YELLOW);

  //player1
  circle2perim(circle[0].x, circle[0].y, 9, 10, RED);
  DrawFilledCircle(circle[0].x, circle[0].y, 9, RED);//body

  circle2perim(circle[2].x, circle[2].y, 4, 5, BLUE);//hand1
  DrawFilledCircle(circle[2].x, circle[2].y, 4, BLUE);

  circle2perim(circle[3].x, circle[3].y, 4, 5, BLUE);//hand2
  DrawFilledCircle(circle[3].x, circle[3].y, 4, BLUE);
  
  //player2
  circle2perim(circle[1].x, circle[1].y, 9, 10, BLUE);
  DrawFilledCircle(circle[1].x, circle[1].y, 9, BLUE);//body

  circle2perim(circle[4].x, circle[4].y, 4, 5, RED);//hand1
  DrawFilledCircle(circle[4].x, circle[4].y, 4, RED);

  circle2perim(circle[5].x, circle[5].y, 4, 5, RED);//hand2
  DrawFilledCircle(circle[5].x, circle[5].y, 4, RED);

  f3d_led_all_on();
  int count;

  for(count = 1; count < 100; count++){
    f3d_nunchuk_read(&nunchuk1);
    f3d_nunchuk_read2(&nunchuk2);
    p1accavg = (p1accavg + nunchuk1.ay) / 2;
    p2accavg = (p2accavg + nunchuk2.ay) / 2;
  }

  
  delay(100);
  while(1){
    
    f3d_nunchuk_read(&nunchuk1);
    f3d_nunchuk_read2(&nunchuk2);
    
    boxer1Block(nunchuk1.c);
    boxer2Block(nunchuk2.c);

    if(nunchuk1.ay > p1accavg+200){
      boxer1Ultimate();
    }
    if(nunchuk2.ay > p2accavg+200){
      boxer2Ultimate();
    }
    if(nunchuk1.z && !circle[0].block){
      boxer1Punch(nunchuk1.z);
    }
    if(nunchuk2.z && !circle[1].block){
      boxer2Punch(nunchuk2.z);
    }
    if(!circle[0].block){
      moveBoxer1(nunchuk1.jx, nunchuk1.jy);
    }
    if(!circle[1].block){
      moveBoxer2(nunchuk2.jx, nunchuk2.jy);
    }
    collisionCheck();
    checkHealth();
   
    
    if(p1Health <= 0 && p2Health <=0){
      f3d_lcd_drawString(20, 100, "IT IS A DRAW!", BLACK, WHITE);
      break;
    }
    if(p1Health <= 0){
      f3d_lcd_drawString(20, 100, "PLAYER 2 WINS!!!", GREEN, WHITE);
      break;
    }
    if(p2Health <= 0){
      f3d_lcd_drawString(20, 100, "PLAYER 1 WINS!!", GREEN, WHITE);
      break;
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
