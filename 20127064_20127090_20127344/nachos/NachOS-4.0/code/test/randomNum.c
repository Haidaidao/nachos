#include "syscall.h"
int 
main()
{
  //call SC_RandomNum to get random number
  int result;
  result = RandomNum();
  PrintString("Random number: ");
  PrintNum(result);
  
  Halt();
  /* not reached */
}