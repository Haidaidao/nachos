#include "syscall.h" 
#define maxlen 32 
int 
main() 
{ 
    int check;
    PrintString("Create file: testfile.txt\n");
    check = Create("testfile.txt");//create testfile.txt and return value
    if (check == 0) {//success
        PrintString("Create testfile.txt success\n");
    }
    else{//fail
        PrintString("Create testfile.txt fail\n");
    }
    Halt(); 
} 