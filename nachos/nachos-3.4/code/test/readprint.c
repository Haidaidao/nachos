#include "syscall.h"
#include "copyright.h"

int main()
{
	char str[50];
	PrintString("Nhap chuoi: ");
	ReadString(str,50);
	PrintString(str);
	

	
	Halt();
	return 0;
}
