#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct CELL *LIST;
  struct CELL{
    int val;
    LIST next;
  };
  
  extern LIST stack;
  
  extern void push(int val){
    LIST c = (LIST) malloc(sizeof(struct CELL));
    if(c) {
      c->val = val;
      c->next = stack;
      stack = c;
      
    }else {
    printf("Stack is empty");
    }
    
  }
  
int main(void){
  while(1){
    struct CELL stack;
    char input[25];
    scanf("%s", input);
    int num = atoi(&input[0]);
    push(num);
    printf("%i", stack.val);
    break;
    }
    
  return 0;

}
  
  
  
