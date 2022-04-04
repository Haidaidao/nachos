#include "syscall.h"
	
int main()
{
    char filename[100], content[256] ; int result = 0 , ID = -1; int numBytes = 0, tempclose;
    PrintString("\n----------------CAT PROGRAM-------------------------\n");
    PrintString("Input a file name : ");
    ReadString(filename,256);//call SC_ReadString to read filename
    ID = Open(filename);//call SC_Open to open filename and return id file
    if(ID==-1){//Open fail
        PrintString("Open File Fail");//call SC_PringString to print fail 
    }
    else{//Open success
        numBytes = Read(content,256,ID);//call SC_Read to read content in filename
        if(numBytes < 0){//File can't read
            PrintString("File is error");//call SC_PringString to print fail 
        }
        else{
            PrintString("Content is : ");
            PrintString(content);//call SC_PrintString to print content
            PrintString("\n");
            Close(ID);//Call SC_Close to close filename
        }
    }    
    Halt();
}