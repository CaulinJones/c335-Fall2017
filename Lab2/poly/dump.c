//
// dump.c
//
//  This code prints the ascii table and diferent values for the characters
//
//   Author: Caulin Jones  and Baylor Mudd
//   Date Created:  7/1/2017
//   Last Modified by: Caulin Jones
//   Assignment:  Lab2 
//   Part of:  C335
//
#include <stdio.h>
void dump_memory(void *p, int len)
{
  int i;
  // headers for the tabs
  printf("address \t char \t hexCh \t short \t integer \t float \t \t doubleFloat \n");
  for (i = 0; i < len; i++) {
    printf("%8p:  ",p+i);
    // checking for control chars
    if(*(unsigned char *)(p + i) >  32 && *(unsigned char *)(p + i) < 127){
      printf("%c \t",*(unsigned char *)(p + i));
      } else{
       printf("? \t");
      }
    //printing aligned values check
    printf(" 0x%02x \t",*(unsigned char *)(p + i));
    if(i % 2 == 0){
      printf("%+hi \t",*(unsigned short *)(p + i));
    }// other allignments values
    if(i % 4 == 0){
      printf("%+i \t %+E \t",*(unsigned int *)(p + i),*(float *)(p + i));
}
    if(i % 8 == 0){
      printf("%+E \t",*(double *)(p + i));
}
    printf("\n");
    // printf("%8p: %c \t 0x%02x \t %+hi  %+i \t %+E \t %+E \n", p + i, *(unsigned char *)(p + i),*(unsigned char *)(p + i),*(unsigned short *)(p + i),*(unsigned int *)(p + i),*(float *)(p + i),*(double*)(p + i));
  }
}
