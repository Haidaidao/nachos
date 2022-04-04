#include "syscall.h"

int main(){
    char filename[256];
    int check;
    PrintString("Input file name you want remove: ");
    ReadString(filename,256);//read input filename want delete
    check = Remove(filename);;//create filename and return value
    if (check == 0){//success
        PrintString("Remove file success\n");
    }
    else{//fail
        PrintString("Error remove file\n");
    }
    Halt();
}