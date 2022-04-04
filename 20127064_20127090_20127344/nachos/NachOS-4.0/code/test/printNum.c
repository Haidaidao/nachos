#include "syscall.h"
int main()
{
    //call SC_PrintNum to print -1356
    PrintString("Use PrintNum to print -1356\n");
    PrintNum(-1356);
    Halt();
}