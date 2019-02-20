
/* main.c ---
*
* Filename: main.c
* Description:
* Author:
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
* Last-Updated:
* By:
* Update #: 0
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
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

FATFS Fatfs; /* File system object */
FIL Fil; /* File object */
BYTE Buff[128]; /* File read buffer */
FILE *fp;

typedef struct {
  uint32_t filesz;
  uint16_t creator1;
  uint16_t creator2;
  uint32_t bmp_offset;
} bmpfile_header;

typedef struct
{
  uint32_t header_sz;
  int32_t   width;
  int32_t   height;
  uint16_t nplanes;
  uint16_t bitspp;
  uint32_t compress_type;
  uint32_t bmp_bytesz;
  int32_t   hres;
  int32_t   vres;
  uint32_t ncolors;
  uint32_t nimpcolors;
 } BITMAPINFOHEADER;

typedef struct { // little endian byte order
  uint8_t b;
  uint8_t g;
  uint8_t r;
} bmppixel;


BITMAPINFOHEADER info;
bmpfile_header header;

int main(void) {
  char footer[20];
  char c[2];
  int count=0;
  int i;
  FRESULT rc; /* Result code */
  DIR dir; /* Directory object */
  FILINFO fno; /* File information object */
  UINT bw, br;
  unsigned int retval;
  int f, pixArray;
  uint16_t numbase = 0;
  uint16_t red,green,blue;

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  f3d_delay_init();
  delay(1000);
  f3d_uart_init();
  delay(1000);
  f3d_lcd_init();
  delay(1000);
  
  
  f3d_rtc_init();
  f_mount(0, &Fatfs); /* Register volume work area (never fails) */
  
  printf("\nOpen an existing file (message.txt).\n");
  rc = f_open(&Fil, "output.bmp", FA_READ);
  
  //read file size from bmp header
  //malloc size to store bmpsize without header
  //

  rc = f_read(&Fil, &c, 2, &br); 
  if (rc) die(rc);
  rc = f_read(&Fil, (void*) &header, sizeof(bmpfile_header), &br);

  if (rc) die(rc);
  rc = f_read(&Fil, (void*) &info, sizeof(BITMAPINFOHEADER), &br); 
  printf("%d\n", info.height);
  printf("%d\n", info.width);
  if(info.bitspp == 24){
    pixArray = (info.width * info.height); 
    printf("this is error");
    uint16_t image[pixArray];
    bmppixel curPix;
    printf("again");
    i=0;
    while(i<=pixArray){
      rc = f_read(&Fil,(void*) &curPix,sizeof(bmppixel),&br);
      red = ((curPix.r * 31)/255) << 11;
      green =((curPix.g * 63)/255) << 5;
      blue = (curPix.b *31)/255;
      uint16_t out = (numbase | red | green | blue);
      image[i] = out;
      i++;
    }
    int k = 1;
    int l = 0;
    int j = 0;
    while(j<=pixArray){
      if(k%128 == 0){
	l++;
	k=1;
      }
      f3d_lcd_drawPixel(40+k, l ,image[j]);
      j++;
      k++;
    }
    printf("read file");
  }else {
    printf("Thils file is not of 24 bit color \n");
  }
  
  
/*
  if (rc) die(rc);
  
  
  for (;;) {
    rc = f_readdir(&dir, &fno); 
    if (rc || !fno.fname[0]) break; 
    if (fno.fattrib & AM_DIR)
      printf(" <dir> %s\n", fno.fname);
    else
      printf("%8lu %s\n", fno.fsize, fno.fname);
  }
  if (rc) die(rc);
  
  printf("\nTest completed.\n");
  
  rc = disk_ioctl(0,GET_SECTOR_COUNT,&retval);
  printf("%d %d\n",rc,retval);
 */
  rc = f_close(&Fil);
  while (1);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
