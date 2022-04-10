#include "syscall.h"
#include "copyright.h"

int main()
{
	int num;
	PrintString("Enter a number: ");
	num = ReadInt();
	PrintString("The number is ");
	PrintInt(num);

	
	Halt();
	return 0;
}
