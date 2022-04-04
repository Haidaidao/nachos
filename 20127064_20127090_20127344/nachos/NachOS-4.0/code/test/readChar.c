#include "syscall.h"

int 
main()
{

  //call SC_ReadChar to read character input
  char ch;
  PrintString("Input character: ");
  ch= ReadChar();
  PrintString("Output: ");
  PrintChar(ch);
  PrintString("\n");  
  Halt();
  /* not reached */
}
