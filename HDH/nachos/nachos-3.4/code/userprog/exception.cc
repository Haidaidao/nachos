// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MaxFileLength 32
#define MaxLength 255

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

// Input: - User space address (int)
// - Limit of buffer (int)
// Output:- Buffer (char*)
// Purpose: Copy buffer from User memory space to System memory space
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
		machine->ReadMem(virtAddr+i,1,&oneChar);
		kernelBuf[i] = (char)oneChar;
		//printf("%c",kernelBuf[i]);
		if (oneChar == 0)
		break;
	}
	return kernelBuf;
}

// Input: - User space address (int)
//- Limit of buffer (int)
//- Buffer (char[])
// Output:- Number of bytes copied (int)
// Purpose: Copy buffer from System memory space to User memory space
int System2User(int virtAddr,int len,char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0 ;
	do{
		oneChar= (int) buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i ++;
	}while(i < len && oneChar != 0);
	return i;
}


void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, counter);
	counter = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, counter);
	machine->WriteRegister(NextPCReg, counter + 4);
}


void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch(which)
	{
	case NoException:
		return;
    	case SyscallException: 
		switch(type) 
		{
		case SC_Halt:
			DEBUG('a', "Shutdown, initiated by user program.\n");
			printf ("\n\nShutdown, initiated by user program.");
   			interrupt->Halt();
			break;
		case SC_Create:
		{
			int virtAddr;
			char* filename;
			DEBUG('a',"\n SC_Create call ...");
			DEBUG('a',"\n Reading virtual address of filename");
			// L???y tham s??? t??n t???p tin t??? thanh ghi r4
			virtAddr = machine->ReadRegister(4);
			DEBUG ('a',"\n Reading filename.");
			// MaxFileLength l?? = 32
			filename = User2System(virtAddr,MaxFileLength+1);
			if (filename == NULL)
			{
				printf("\n Not enough memory in system");
				//DEBUG(???a???,"\n Not enough memory in system");
				machine->WriteRegister(2,-1); // tr??? v??? l???i cho ch????ng
				// tr??nh ng?????i d??ng
				delete filename;
				IncreasePC();
				return;
			}
			DEBUG('a',"\n Finish reading filename.");
			//DEBUG(???a???,"\n File name : '"<<filename<<"'");
			// Create file with size = 0
			// D??ng ?????i t?????ng fileSystem c???a l???p OpenFile ????? t???o file,
			// vi???c t???o file n??y l?? s??? d???ng c??c th??? t???c t???o file c???a h??? ??i???u
			// h??nh Linux, ch??ng ta kh??ng qu???n ly tr???c ti???p c??c block tr??n
			// ????a c???ng c???p ph??t cho file, vi???c qu???n ly c??c block c???a file
			// tr??n ??? ????a l?? m???t ????? ??n kh??c
			if (!fileSystem->Create(filename, 0))
			{
				printf("\n Error create file '%s'",filename);
				machine->WriteRegister(2,-1);
				delete filename;
				IncreasePC();
				return;
			}
			machine->WriteRegister(2,0); // tr??? v??? cho ch????ng tr??nh
			// ng?????i d??ng th??nh c??ng
			delete filename;
			IncreasePC();
			break;
		}
		case SC_ReadString: // void ReadString(char buffer[], int length);
		{
			char* buf = new char[MaxLength]; // allocate buffer
			if(buf == 0) // if there are not enough no memory
			{
				delete[] buf;
				IncreasePC();
				break;
			}
			int virtAddr = machine->ReadRegister(4); // get first parameter
			int length = machine->ReadRegister(5); // get second parameter
			int sz = gSynchConsole->Read(buf,length); // read from console
			System2User(virtAddr, sz, buf); // transfer to user space
			delete[] buf; // deallocate buffer
			IncreasePC();
			break;
		}
		case SC_PrintString: // void PrintString(char buffer[]);
		{
			int virtAddr = machine->ReadRegister(4); // get parameter
			int i = 0;
			char *buf = new char[MaxLength]; // allocate buffer
			if(buf == 0) // if there are not enough no memory
			{
				delete[] buf;
				IncreasePC();
				break;
			}
			buf = User2System(virtAddr, MaxLength + 1); // transfer to kernel space 
			while(buf[i] != 0) //&& buf[i] != '\n') // print one character at a time until 0(end of buffer) or \n(endline)
			{
				gSynchConsole->Write(buf + i,1);
				i++;
			}
			//gSynchConsole->Write(buf + i,1); 
			delete[] buf; // deallocate buffer
			IncreasePC();
			break;
		}
		case SC_ReadChar: // char ReadChar();
		{
			char buf;
			gSynchConsole->Read(&buf,1); //read from console
			machine->WriteRegister(2,buf);
			IncreasePC();
			break;
		}
		case SC_PrintChar: // void PrintChar(char character);
		{
			char *buf = new char;
			*buf = machine->ReadRegister(4); // get parameter  
			
			gSynchConsole->Write(buf,1);
			delete buf;
			IncreasePC();
			break;
		}
		case SC_ReadInt: // int ReadInt();
		{
			char* buf = new char[MaxLength]; // allocate buffer
			if(buf == 0) // if there are not enough no memory
			{
				delete[] buf;
				IncreasePC();
				break;
			}
			int length = gSynchConsole->Read(buf,MaxLength); // read from console
			
			if(((buf[0] < 48)&&(buf[0] != '-')) || (buf[0] > 57))	// check the first character 
			{							// (because it can be '-' for negative value)
				delete[] buf;
				machine->WriteRegister(2,0);
				IncreasePC();
				break;
			}

			bool neg = (buf[0] == '-')? 1:0; // check if the integer value is negtive
			int number = 0;
			int first_num_index = (neg)? 1:0;
			
			bool notint = 0;
			for(int i = first_num_index; i < length; i++) 	// get the integer value from the string(buf) 
			{						// start from left most numeric character
				if((buf[i] < 48) || (buf[i] > 57))
				{	
					// if any character is not a number (0 -> 9) mean it is not integer
					delete[] buf;
					machine->WriteRegister(2,0);
					notint = 1;
					break;
				}

				unsigned int temp = 10*number + (int)(buf[i] - 48); 
				if (temp > 2147483647) // check if the value is larger than what integer can store
				{
					delete[] buf;
					//printf("Long");
					machine->WriteRegister(2,0);
					notint = 1;
					break;
				}
				
				number = (int) temp;
			}

			if(notint)
			{
				IncreasePC();
				break;
			}

			machine->WriteRegister(2,(neg)? -number:number); // return number (pos/neg)
			IncreasePC();
			break;	
		}
		case SC_PrintInt: // void PrintInt(int number);
		{
			int* num = new int;
			*num = machine->ReadRegister(4); // get parameter

			if(*num == 0) gSynchConsole->Write("0" , 1); // special case to for 0

			bool neg = (*num < 0)? 1:0; // check if the number is negative
			if(neg) *num = -*num;
			//if (!neg) printf("Neg");
			char* str = new char[15];
			int i = 14;
			

			while (*num != 0) // get each number of the interger start from right most
			{
				//printf("%d",*num % 10);
				str[i] = *num % 10 + 48; 
				i--;
				*num /= 10;
			}

			if (neg) // add - if negative
			{
				str[i] = '-';
				i--;
			}

			//char *buf = new char[15];
			//buf = User2System((int) str, 15); // transfer to kernel space 
			
			gSynchConsole->Write(str + i + 1 , 14 - i + 1); // write to console

			delete num;
			delete[] str;
			//delete[] buf;
			IncreasePC();
			break;
			
		}
		case SC_Exec:
		{
			// Input: vi tri int
			// Output: Fail return -1, Success: return id cua thread dang chay
			// SpaceId Exec(char *name);
			int virtAddr;
			virtAddr = machine->ReadRegister(4);	// doc dia chi ten chuong trinh tu thanh ghi r4
			char* name;
			name = User2System(virtAddr, MaxFileLength + 1); // Lay ten chuong trinh, nap vao kernel
	
			if(name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
				//IncreasePC();
				return;
			}
			OpenFile *oFile = fileSystem->Open(name);
			if (oFile == NULL)
			{
				printf("\nExec:: Can't open this file.");
				machine->WriteRegister(2,-1);
				IncreasePC();
				return;
			}

			delete oFile;

			// Return child process id
			int id = pTable->ExecUpdate(name); 
			machine->WriteRegister(2,id);

			delete[] name;	
			IncreasePC();
			break;
		}
		case SC_Join:
		{       
			// int Join(SpaceId id)
			// Input: id dia chi cua thread
			// Output: 
			int id = machine->ReadRegister(4);
			
			int res = pTable->JoinUpdate(id);
			
			machine->WriteRegister(2, res);
			IncreasePC();
			return;
		}
		case SC_Exit:
		{
			//void Exit(int status);
			// Input: status code
			int exitStatus = machine->ReadRegister(4);

			if(exitStatus != 0)
			{
				IncreasePC();
				return;
				
			}			
			
			int res = pTable->ExitUpdate(exitStatus);
			//machine->WriteRegister(2, res);

			currentThread->FreeSpace();
			currentThread->Finish();
			IncreasePC();
			break; 
				
		}
		default:
			printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);	
			interrupt->Halt();
		}
		break;
	case PageFaultException:
		printf("No valid translation found\n");
		ASSERT(FALSE);	
		interrupt->Halt();
	case ReadOnlyException:
		printf("Write attempted to page marked \"read-only\"\n");
		ASSERT(FALSE);	
		interrupt->Halt();
	case BusErrorException:
		printf("Translation resulted in an invalid physical address\n");
		ASSERT(FALSE);	
		interrupt->Halt();
	case AddressErrorException:
		printf("Unaligned reference or one that was beyond the end of the address space\n");
		ASSERT(FALSE);
		interrupt->Halt();
	case OverflowException:
		printf("Integer overflow in add or sub\n");
		ASSERT(FALSE);	
		interrupt->Halt();
	case IllegalInstrException:
		printf("Unimplemented or reserved instr\n");
		ASSERT(FALSE);	
		interrupt->Halt();
	case NumExceptionTypes:
		printf("NumExceptionTypes\n");
		ASSERT(FALSE);
		interrupt->Halt();
	default:
		printf("Unexpected user mode exception %d %d\n", which, type);
		ASSERT(FALSE);
		interrupt->Halt();
	}
}
