#include "syscall.h"

int main(){
    //Call SC_PrintChar to print 't'
    PrintString("Use PrintChar to print 't'\n");
    PrintChar('t');
    PrintString("\n");
    Halt();
}