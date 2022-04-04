#include "syscall.h"

int
main()
{
  //call SC_PrintString to read string input
  char s[10];
  PrintString("Input string(max length is 20): ");
	ReadString(s,20);
  PrintString("Output string: ");
  PrintString(s);
  PrintString("\n");

  Halt();
  /* not reached */
}