#include "syscall.h"

int main(){
    char filename[256];
    int check;
    PrintString("Input file name you want create: ");
    ReadString(filename,256);//read input filename want create
    check = Create(filename);//create filename and return value
    if (check == 0){//success

        PrintString("Create file success");
    }
    else{//fail

        PrintString("Error create file");

    }
    Halt();
}