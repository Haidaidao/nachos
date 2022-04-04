#include "syscall.h"
	
int 
main()
{
    char filename[100], content[256] ; int result = 0 , ID = -1; int numBytes = 0, tempclose,check;
    PrintString("Input a file name : ");
    ReadString(filename,256);///read filename
    ID = Open(filename);//open filename
    if(ID==-1){//fail
        PrintString("Open File Fail");
    }
    else{//success
        numBytes = Write("Demo NACHOS-4.0",256,ID);//write to filename 
        if(numBytes < 0){//fail
            PrintString("File is error");
        }
        else{//success
            PrintString("Content is : ");
            PrintString(content);
        }
        check = Close(ID);//close file with id
        if (check == 0){
            PrintString("Close file is succesful\n");
        }
        else{
            PrintString("Close file is not succesful\n");
        }

    }
    Halt();
}