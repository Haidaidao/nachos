#include "syscall.h"
	
int main()
{
    char filename[100], content[256] ; int result = 0 , ID = -1; int numBytes = 0, tempclose;
    PrintString("Input a file name : ");
    ReadString(filename,256);//read input filename
    PrintString("File ID: ");
    ID = Open(filename);//open filename and return id file
    if(ID==-1){//fail
        PrintString("Open File Fail");
    }
    else{//success
        numBytes = Read(content,256,ID);//read content in filename with id
        if(numBytes < 0){//fail
            PrintString("File is error");
        }
        else{//success
            PrintString("Content is : ");
            PrintString(content);//print content in filename
            PrintString("\n");
        }
        Close(ID);

    }    
    Halt();
}