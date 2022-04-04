#include"syscall.h"

int main(){
    int i;
    PrintString("\t\t------> ASCII <------\t\t\n");
  for ( i = 33; i < 65; i++)
  {
    PrintNum(i);
    PrintChar('\t');
    PrintChar((char)i);
    PrintString("\t|");

    PrintNum(i + 32);
    PrintChar('\t');
    PrintChar((char)(i + 32));
    PrintString("\t|");

    if (i + 64 > 127)
    {
      PrintChar('\t');
      PrintString("\t|\n");
      break;
    }

    PrintNum(i + 64);
    PrintChar('\t');
    PrintChar((char)(i + 64));
    PrintString("\t|\n");
  }
    Halt();
}