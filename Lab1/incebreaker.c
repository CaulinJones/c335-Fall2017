#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {

  int fileNo = 0;
  int i;

  for(;;) {
    i++;
    char fileNum[3];
    sprintf(fileNum, "%d", i);
    printf("hello world\n");
    FILE *fp;
    fp = fopen(fileNum, "w+");
    fputs("HELLO", fp);
    fclose(fp);
    sleep(3);
  }
 return 0;
}
