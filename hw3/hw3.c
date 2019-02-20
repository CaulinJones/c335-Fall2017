//hw3.c
//Dissassembler for hw3
//By: Caulin Jones
//c335 Fall-2017
#include <stdio.h>

char *reg[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
char *opCodes[] = {"ands","eors","lsls","lsrs","asrs","adcs","sbcs","rors","tst","rsbs","cmp","cmns","orrs","muls","bics","mvns"};
//int input;
#define rd(x) ((x) & 7)
#define rm(x) ((x >> 3) & 7)
#define rn(x) ((x >> 6) & 7)

void assembEncoder(int inst){
  // checking binary header for add or sub
  if((inst >> 13) == 0){
    //checking if add register (rm ,rn ,rd) 
    if((inst >> 9) == 12){
      printf("    adds %s,%s,%s\n" , reg[inst & 7], reg[(inst >> 3) & 7], reg[(inst >> 6) & 7]);
      
    }else if((inst >> 9)== 13){
      printf("    subs %s,%s,%s\n" , reg[inst & 7], reg[(inst >> 3) & 7], reg[(inst >> 6) & 7]);
    }else if((inst >> 9) == 14){
      printf("    adds %s,%s,#%d\n", reg[rd(inst)], reg[rm(inst)], rn(inst) );
  
    }else if((inst >> 9) == 15){
      printf("    subs %s,%s,#%d\n", reg[rd(inst)],reg[rm(inst)],rn(inst));
    }else {
    printf(".hword 0x\n", inst);
  }
  //checking for comp move immediate
  }else if((inst >> 13) == 1){
    if((inst >> 11) == 8){
      printf("    mov %s,#%d\n", reg[(inst >> 8) & 7],(inst & 127 ));
    }
    if((inst >> 11) == 9){
      printf("    cmp %s,#%d\n" , reg[(inst >> 8) & 7],(inst & 127));
    }
    if((inst >> 11) == 10){
      printf("    adds %s,#%d\n" , reg[(inst >> 8) & 7],(inst & 127));
    } 
    if((inst >> 11) == 11){
      printf("    subs %s,#%d\n" , reg[(inst >> 8) & 7],(inst & 127));
    }else {
    printf(".hword 0x", inst);
  }

    //checking for Data-processing instructions
  }else if((inst >> 13) == 2){
    printf("    %s %s,%s\n", opCodes[(inst >> 6) & 15] , reg[rd(inst)], reg[rm(inst)]);
  }else {
    printf(".hword 0x\n", inst);
  }

  

}

void main(void){
  int input;
  printf(".text\n.syntax unified\n.thumb\n");
  while(scanf("%x",&input) == 1){
    // scanf("%x",&input);
    assembEncoder(input);
   
  }



}
