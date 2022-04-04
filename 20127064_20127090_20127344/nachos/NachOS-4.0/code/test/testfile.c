#include "syscall.h"

int main(){
    int id , length = 256,bytes,bytes2,check; char buffer[256],buffer2[256];
    id = Open("testfile.txt");//open 'testfile.txt'
    if (id != -1){//success
        PrintString("Open 'testfile.txt' success\n");
        PrintString("Write \"Test Open, Write, Read, Close file\" to testfile.txt\n");
        bytes = Write("Test Open, Write, Read, Close file",256,id);//Write to file with id
        if (bytes<0){//fail
            PrintString("File can't be write\n");
        }
        else{//success
            PrintString("Write succesfully\n");
        }
        check = Close(id);//close file
        if (check == 0){
            PrintString("Close file is succesful\n");
        }
        else{
            PrintString("Close file is not succesful\n");
        }
        
    }
    else{
        PrintString("Open file is not succesful");
    }
    Halt();
    return 0;
}