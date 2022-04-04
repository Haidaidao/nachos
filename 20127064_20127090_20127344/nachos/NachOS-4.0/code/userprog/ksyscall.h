/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "synchconsole.h"
#include <stdlib.h>     
#include <time.h>   


void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysReadNum(){
  char res;
  long long result = 0;
  int count = 0;
  int countError = 0;
  char temp;
  //use loop to read string from input and convert it to number
  while ((res = kernel->synchConsoleIn->GetChar()) != '\n' && res != '\r')
  {
    if (count == 0)
      temp = res;

    if (res < '0' || res > '9')
    {
      if ((temp != '-' && count == 0) || count != 0)
      {
        countError++;
      }
    }

    if ((temp != '-' && count == 0) || count != 0)
      result = result * 10 + (res - '0');
    count++;
  }

  if (countError != 0)
  {
    //DEBUG(dbgSys, "ERROR: Input is not integer number\n");
    printf("ERROR: Input is not integer number\n");
    return 0;
  }

  if (temp == '-')
  {
    if (result > 2147483648)
    {
      //DEBUG(dbgSys, "ERROR: Size of number exceeds the limit of type int\n");
      printf("ERROR: Size of number exceeds the limit of type int\n");
      return 0;
    }
    return -result;
  }

  if (result > 2147483647)
  {
    //DEBUG(dbgSys, "ERROR: Size of number exceeds the limit of type int\n");
    printf("ERROR: Size of number exceeds the limit of type int\n");
    return 0;
  }

  return result;
}

char SysReadChar()
{
  char ch = '\0';
  char result;
  int count = 0;
  // Use GetChar in synchconsole.h to reed input from keyboard
  // Read one char, if > 2 char input system return '\0'
  do
  {
    if (ch == '\n' || ch == '\r')
    {
      break;
    }
    count++;
    result = ch;
    ch = kernel->synchConsoleIn->GetChar();
  } while (true);

  if (count > 2)
  {
    //DEBUG(dbgSys, "ERROR: Just Enter one character"<< "\n");
    printf("ERROR: Just Enter one character\n");
    return '\0';
  }

  return result;
}
int SysReadString(char *input, int length)
{
  memset(input, 0, length);//set input is '-' (0 in ascii)
  //use loop to read char and make it to string
  int count = 0;
  for (int i = 0; i < length; i++)
  {
    if (i == length -1)//set last input is '\0'
    {
      char cut;
      do
      {
        cut = kernel->synchConsoleIn->GetChar();
      } while (cut != '\n' && cut != '\r');
      
      input[i] = '\0';
      break;
    }

    char ch = kernel->synchConsoleIn->GetChar();
    if (ch == '\n' || ch == '\r')
    {
      input[i] = '\0';
      break;
    }
    else
    {
      input[i] = ch;
      count++;
    }
  }
  return count;
}
void IncreasePC() /* Modify return point */
{
  /* set previous programm counter (//DEBUGging only)*/
  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

  /* set next programm counter for brach execution */
  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}


int SysRandomNum(){
      int random;
			srand(time(NULL));

      // use rand() in system to get random number
      random = 1 + rand() % (RAND_MAX - 1 + 1); 
      return random;
}


int pow(int num, int ex)
{
  //calculate num^ex buy loop
  if (ex <= 0)
    return 1;

  int result = 1;
  for (int i = 0; i < ex; i++)
  {
    result *= num;
  }

  return result;
}

int countDigit(int num)
{
  //count numbers by division 10
  if (num == 0)
  {
    return 1;
  }

  int temp = num;
  int count = 0;
  while (temp != 0)
  {
    count++;
    temp = temp / 10;
  }

  return count;
}

void SysPrintChar(char c){
  //use PutChar in synchconsole.h to print char 
  kernel->synchConsoleOut->PutChar(c);
}

void SysPrintNum(int num)
{
  //separate numbers from left to right and use PutChar in synchconsole.h to print them out as char
  int temp = num;
  if (temp > 2147483647){
    char *error = "ERROR: Size of number exceeds the limit of type int";
    for (int i=0 ;i <strlen(error);i++){
    kernel->synchConsoleOut->PutChar(error[i]);

    }
    return;
  }
  if (temp <-2147483648){
    char *error = "ERROR: Size of number exceeds the limit of type int";
    for (int i=0 ;i <strlen(error);i++){
    kernel->synchConsoleOut->PutChar(error[i]);
    }
    return;
  }
  if (num < 0)
  {
    kernel->synchConsoleOut->PutChar('-');
    temp = -temp;
  }

  int countNum = countDigit(temp);


  char *buf;
  buf = new char[countNum+1];


  int i=0;
  while (countNum > 0){
      buf[i] = (char)(temp%10 +48);
      temp=temp/10;
      i++;
      countNum--;

  }
  countNum = i;

  for (i=countNum-1;i>=0;i--) {
    
    kernel->synchConsoleOut->PutChar(buf[i]);
  }

  delete[] buf;

}

int lengthString(char* buffer){
  //get string of buffer
  int len = strlen(buffer);
  return len;
}

void SysPrintString(char* buffer){
  //use loop to print char from buffer
  for (int i=0;i<lengthString(buffer);i++){
      kernel->synchConsoleOut->PutChar(buffer[i]);
  }
}


char* User2System(int virtAddr,int limit) 
{ 
    int i;// index 
    int oneChar; 
    char* kernelBuf = NULL; 
    
    kernelBuf = new char[limit +1];//need for terminal string 
    if (kernelBuf == NULL) 
      return kernelBuf; 
      memset(kernelBuf,0,limit+1); 
    
    //printf("\n Filename u2s:"); 
    for (i = 0 ; i < limit ;i++) 
    { 
        kernel->machine->ReadMem(virtAddr+i,1,&oneChar); 
        kernelBuf[i] = (char)oneChar; 
        //printf("%c",kernelBuf[i]); 
        if (oneChar == 0) 
            break; 
    } 
    return kernelBuf; 
}
int System2User(int virtAddr,int len,char* buffer) 
{ 
    if (len < 0) return -1; 
    if (len == 0)return len; 
    int i = 0; 
    int oneChar = 0 ; 
    do{ 
      oneChar= (int) buffer[i]; 
      kernel->machine->WriteMem(virtAddr+i,1,oneChar); 
      i ++; 
    }while(i < len && oneChar != 0); 
    return i; 
} 
int check(vector<int> openning, int ID){
    bool flag = false;
    for(int i = 0 ; i < openning.size() ; i++){
        if(openning[i] == ID){
          flag = true;
          break;
        }
    }
    if(flag == true){
      printf("File is openning, do you want to continue ?\n\n");
      printf("1.Yes\n");
      printf("2.No.\n");
      int result;
      result = SysReadNum();
      if(result == 1) return 1;
      else return -1;
    }
    return 0;
}
#endif /* ! __USERPROG_KSYSCALL_H__ */
