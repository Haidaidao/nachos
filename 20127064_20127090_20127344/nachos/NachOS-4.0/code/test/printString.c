#include "syscall.h"
int main(){
    //call SC_PrintString to print "Hello, World!"
    PrintString("Use PrintString to print \"Hello, World!\"\n");
    PrintString("Hello, World!\n");
    Halt();
}