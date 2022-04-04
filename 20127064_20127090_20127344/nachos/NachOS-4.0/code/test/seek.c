#include "syscall.h"

int main(){
    int id , length = 256,bytes,bytes2,check; char buffer[256],buffer2[256];
    id = Open("testSeek.txt");//open file testSeek.txt
    if (id != -1){//success
        PrintString("Open 'testSeek.txt' success\n");

        bytes = Read(buffer , length, id);//read content in file
         if(bytes < 0){//fail
            PrintString("File can't be read");
        }
        else{//success
            PrintString("The content of file is : ");
            PrintString(buffer);
            PrintString("\n");
            PrintString("Seek to postion is 5 and read 7 char in file");
            PrintString("\n");
            Seek(5,id);//seek to position is 5 in file
            bytes2 = Read(buffer2 , 7, id);//read 7 char in  from pos seek
            if(bytes2 < 0){//read fail
                PrintString("File can't be read");
            }
            else{//read success
                PrintString("The content of file is : ");
                PrintString(buffer2);
                PrintString("\n");
            }
        }
        check = Close(id);//close file with id
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