#include "syscall.h" 
#define maxlen 32 
int 
main() 
{ 
    int check;
    PrintString("Remove file: 'testfile.txt'\n");
    check = Remove("testfile.txt");//call SC_Remove to delete testfile.txt
    if (check == 0) {//success
        PrintString("Remove testfile.txt success\n");
    }
    else{//fail
        PrintString("Remove testfile.txt fail\n");
    }
    Halt(); 
} 