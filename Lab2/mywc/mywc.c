/*****************************
 * mywc.c
 *
 *
 *
 *This code counts the words lines and characters in a text file
 *
 *   Author: Caulin Jones
 *   Date Created: 7/1/2017
 *   Last Modified: 7/7/2017
 *   Assignent: Lab2
 *   Part of:c335
 */
#include <stdio.h>
int c;

int main(void){
  int countw = 0;
  int countchar = 0;
  int countline =0;

  while ((c = getchar()) != EOF){
    countchar++;
    if((c == ' ')||(c == '\n')){
      countw++;
    }
    if(c == '\n'){
      countline++;
    }
  }
  printf("%d %d %d\n", countline, countw, countchar);
  return 0;
}

