#include "syscall.h"

int main()
{
  //call SC_ReadNum to read number input
  int result;
  PrintString("Input number : ");

  result = ReadNum();
  PrintString("Output is : ");
  PrintNum(result);
  Halt();
  /* not reached */
}