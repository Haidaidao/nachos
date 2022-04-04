#include "syscall.h"
int main(){
    int srcID, destID,byteSrc,byteDes;
    int length = 256;
    char srcName[length] , destName[length];
    char srcContent[length];
    PrintString("\n----------------COPY PROGRAM-------------------------\n");
    PrintString("Input source's filename : ");
    ReadString(srcName , length);//call SC_ReadString to read srcName
    PrintString("\n\nInput destination's filename : ");
    ReadString(destName , length);//call SC_ReadString to read destName
    srcID = Open(srcName);//open srcName and return id file
    if(srcID != -1){//success
        byteSrc = Read(srcContent, length , srcID);//read content in srcName
        Close(srcID);//close srcName
        destID = Create(destName);//create desName 
        if(destID == -1){//fail
            PrintString("\nCreate file is not succesful\n");
        }
        else{//success
            destID = Open(destName);//open desName and return id file
            if(destID != -1){//success
                if(byteSrc < 0){//fail
                    PrintString("\nSource's file can't be read\n");
                }
                else{//success
                    byteDes = Write(srcContent , length , destID);//write to desName
                    if(byteDes < 0){//fail
                        PrintString("\nSource'file can't be written\n");
                    }
                    else{//success
                        PrintString("\nCopy content from source's file to destination's file is succesful\n");
                    }
                }
            }
            else{//fail
                PrintString("\nOpen destination's file is not succesful\n");
            }
        }
    }
    else{///fail
        PrintString("\nOpen source's file is not succesful\n");
    }
    Close(destID);//close desName
    Halt();
}