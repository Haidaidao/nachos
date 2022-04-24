#include "syscall.h"
#include "copyright.h"

int main()
{
	int n;
	n = ReadInt();

	PrintInt(n);

	
	Halt();
	return 0;
}
