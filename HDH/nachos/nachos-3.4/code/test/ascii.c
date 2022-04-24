#include "syscall.h"
#include "copyright.h"

int main()
{	
	char ch;
	int i = 0;
	
	PrintString("\n\n\n");
	PrintString("\tASCII TABLE");
	for(i;i < 255;i++)
	{
		ch = (char) i;

		PrintChar('\n');
		
		PrintChar('\t');
		PrintInt(i);

		PrintChar('\t');
		PrintChar(ch);
	}
	PrintString("\n\n\n");

	Halt();
	return 0;
}
