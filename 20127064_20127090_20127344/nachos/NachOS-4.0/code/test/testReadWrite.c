#include "syscall.h"

void WriteID1(){
    //write to console with Write(buffer,charcount,1)
    int bytes;
    PrintString("Test Console IO with id = 1 in Write(buffer,charcount,1)\n");
    PrintString("Write \"NachOS-4.0\" to console\n");
    bytes = Write("NachOS-4.0",256,1);
    PrintString("\n");
    if (bytes < 0){
        PrintString("Can't be write");
    }
    else{
        PrintString("Write to console success\n");
    }
}

void WriteID0(){
    //write to console with Write(buffer,charcount,0)
    //
     int bytes;
    PrintString("Test Console IO with id = 0 in Write(buffer,charcount,0)\n");
    PrintString("Write \"NachOS-4.0\" to console\n");
    bytes = Write("NachOS-4.0",256,0);
    PrintString("\n");
    if (bytes < 0){
        PrintString("Can't be write");
    }
    else{
        PrintString("Write to console success\n");
    }
}

void ReadID0(){
    //Read from console with Read(buffer,charcount,0)

    int bytes;
    char content[256];
    PrintString("Test Console IO with id = 0 in Read(buffer,charcount,0)\n");
    PrintString("Input: ");
    bytes = Read(content,256,0);
    if (bytes < 0){
        PrintString("Can't be read");
    }
    else{
        PrintString("Output: ");
        PrintString(content);
        PrintString("\n");

    }
}

void ReadID1(){
    //Read from console with Read(buffer,charcount,1)
    int bytes;
    char content[256];
    PrintString("Test Console IO with id = 1 in Read(buffer,charcount,1)\n");
    PrintString("Input: ");
    bytes = Read(content,256,1);
    if (bytes < 0){
        PrintString("Can't be read\n");
    }
    else{
        PrintString("Output: ");
        PrintString(content);
        PrintString("\n");

    }
}

int main(){
    WriteID0();
    Halt();
    return 0;
}