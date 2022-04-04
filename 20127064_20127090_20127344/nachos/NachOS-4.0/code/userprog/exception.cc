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
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include <vector>
#include "ksyscall.h"
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
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------
vector<int> openning;
void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");
	
    switch (which) {
	
	case NoException:
		return;

	case PageFaultException:
		DEBUG(dbgSys, "No valid translation found.\n");
		printf("\n\nNo valid translation found.");
		SysHalt();
		ASSERTNOTREACHED();
		break;

	case ReadOnlyException:
		DEBUG(dbgSys, "Write attempted to page marked read-only.\n");
		printf("\n\nWrite attempted to page marked read-only.");
		SysHalt();
		ASSERTNOTREACHED();
		break;

	case BusErrorException:
		DEBUG(dbgSys, "Translation resulted in an invalid physical address.\n");
		printf("\n\nTranslation resulted in an invalid physical address.");
		SysHalt();
		ASSERTNOTREACHED();
		break;

	case AddressErrorException:
		DEBUG(dbgSys, "Unaligned reference or one that was beyond the end of the address space.\n");
		printf("\n\nUnaligned reference or one that was beyond the end of the address space.");
		SysHalt();

		ASSERTNOTREACHED();
		break;

	case OverflowException:
		DEBUG(dbgSys, "Integer overflow in add or sub..\n");
		printf("\n\nnteger overflow in add or sub..");
		SysHalt();
		ASSERTNOTREACHED();
		break;

	case IllegalInstrException:
		DEBUG(dbgSys, "Unimplemented or reserved instr.\n");
		printf("\n\nUnimplemented or reserved instr.");
		SysHalt();

		ASSERTNOTREACHED();
		break;

	case NumExceptionTypes:
		DEBUG(dbgSys, "Number exception types.\n");
		printf("\n\nNumber exception types.");
		SysHalt();

		ASSERTNOTREACHED();
		break;

    case SyscallException:
      	switch(type) {
		case SC_Halt:{
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
			printf("\nShutdown, initiated by user program.\n");
			SysHalt();

			ASSERTNOTREACHED();
		}

 		case SC_Add: {
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();

			return;

			ASSERTNOTREACHED();
		}
		
		case SC_ReadNum: {
			//input: number; return : number , if number == char  => 0
			//Use kernel->synchConsoleIn->GetChar() to read string and convert it to number, return number in register 2
			int result1;

			result = SysReadNum(); //call sysReadNum in ksyscall.h to read number

			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);
			DEBUG(dbgSys, "sysReadNum returning with " << result << "\n");
			IncreasePC();

			return;

			ASSERTNOTREACHED();
		}
			
		case SC_ReadChar: {
			//input: char; return char system read
			//Use kernel->synchConsoleIn->GetChar() to read char and return it in register 2
			char ch;
				
			ch = SysReadChar(); //call sysReadChar in ksyscall.h to read char
			kernel->machine->WriteRegister(2, (char)ch);
			if (ch != '\0') {
				DEBUG(dbgSys, "sysReadChar returning with " << ch << "\n");
			}
			IncreasePC();

			return;

			ASSERTNOTREACHED();
		}
			
		case SC_Open: {
			// Input: bufAddr: Address of string 'filename'
			// Output: return OpenFileID if success, -1 if fail
			// Use Open in filesys.h open to read and write file 
	 		int bufAddr = kernel->machine->ReadRegister(4); //read buf address
			char *buf;
			
			if (kernel->fileSystem->index > 10) {//check the number of open files 
				DEBUG(dbgSys, "Can't open file"<< "\n");
				printf("Can't open file\n");
				kernel->machine->WriteRegister(2, -1);
				delete[] buf;
				IncreasePC();
				return;
				ASSERTNOTREACHED();
			}
				
			
			buf = User2System(bufAddr, MaxFileLength + 1); // get file name by buf address
			if (strcmp(buf, "stdin") == 0) {//open with consoleInput
				DEBUG(dbgSys, "Open file with stdin mode"<< "\n");
				printf("Open file with stdin mode\n");

				kernel->machine->WriteRegister(2, 0);
				delete[] buf;
				IncreasePC();
				return;
				ASSERTNOTREACHED();
			}
			
			if (strcmp(buf, "stdout") == 0) {//open with consoleOutput
				DEBUG(dbgSys, "Open file with stdout mode"<< "\n");
				printf("Open file with stdout mode\n");

				kernel->machine->WriteRegister(2, 1);
				delete[] buf;
				IncreasePC();

				return;

				ASSERTNOTREACHED();
			}
			
			//open buf and assign that value to openf
			//if openf not null => succes, else fail
			if ((kernel->fileSystem->openf[kernel->fileSystem->index] = kernel->fileSystem->Open(buf)) != NULL) {
				DEBUG(dbgSys, "Open file success"<< "\n");
				
				kernel->machine->WriteRegister(2, kernel->fileSystem->index);
				openning.push_back(kernel->fileSystem->index);
				kernel->fileSystem->openf[kernel->fileSystem->index] = kernel->fileSystem->Open(buf);
			}
			else {
				DEBUG(dbgSys, "Can't open file"<< "\n");
				kernel->machine->WriteRegister(2, -1);
			}
			delete[] buf;
			IncreasePC();

			return;

			ASSERTNOTREACHED();

		}

		case SC_Close: {
			// Input: id of file(OpenFileID)
			// Output: 0: if success, -1 if fail
			int id = kernel->machine->ReadRegister(4);
			int i = kernel->fileSystem->index; 
			int k, position;
			char * filename;
			
			if (i < id) {
				DEBUG(dbgSys, "Close file failed" << "\n");
				printf("Close file failed \n");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
				break;
			}
			
			//get file name from address
			filename = User2System(id, MaxFileLength + 1);
			if(kernel->fileSystem->openf[id] != NULL){ //file exist
				DEBUG(dbgSys, "Close file success"<< "\n");
				delete kernel->fileSystem->openf[id];
				kernel->fileSystem->openf[id] == NULL;
			}
			else{
				DEBUG(dbgSys, "Close file failed" << "\n");
				printf("Close file failed \n");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			kernel->machine->WriteRegister(2, 0);
			//DEBUG(dbgSys, "Openning size is  : " << openning.size() << "\n");
			for(k = 0 ; k < openning.size();k++){
				if(id == openning[k]){
					position = k;
					break;
				}
			}
			
			openning.erase(openning.begin() + position);
			IncreasePC();
			return;

		}

		case SC_Seek: {
			// Input: position, id of file(OpenFileID)
			// Output: postion: success, -1: fail
			// Seek an arbitrary position in a file
			int pos = kernel->machine->ReadRegister(4);
			int id = kernel->machine->ReadRegister(5);
			if (id < 1 || id > kernel->fileSystem->index || kernel->fileSystem->openf[id] == NULL) {//check file is exist
				DEBUG(dbgSys, "Seek file failed" << "\n");
				printf("Seek file failed\n");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}

			int len = kernel->fileSystem->openf[id]->Length();	// file size

			if (pos > len) {//pos is outside size of string
				DEBUG(dbgSys, "Seek out size file" << "\n");
				printf("Seek out size file\n");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}

			if (pos == -1)	// move ptr to end of file
				pos = len;
			DEBUG(dbgSys, "Seek to "<<pos<<" success" << "\n");
			printf("Seek to %d success\n",pos);
			kernel->fileSystem->openf[id]->Seek(pos);
			kernel->machine->WriteRegister(2, pos);
			IncreasePC();
			return;
		}

		case SC_Read:
		{
			// Input: Address of variable(char*) want to store the read value , the number of characters want to read(int), id file(OpenFileID)
			// Output: number of bytes read: success, -1: fail
			// Read file and store it to variable(char*) and return number of bytes read
			int virtAddr = kernel->machine->ReadRegister(4);
			int charcount = kernel->machine->ReadRegister(5);
			int id = kernel->machine->ReadRegister(6);
			int i = kernel->fileSystem->index; // 2
			char *buf;
			if (id > i || id < 0) {// check file is exist{						 	
				DEBUG(dbgSys, "Try to open invalib file" << "\n");
				printf("Try to open invalib file");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}

			if (kernel->fileSystem->openf[id] == NULL) {//check file is exist
				DEBUG(dbgSys, "Try to open invalib file" << "\n");
				printf("Try to open invalib file");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}


			if(id == 1){ //id =1 => file open with stdout mode => can't read
				SysPrintString("OpenID is invalid for read !.\n");

				DEBUG(dbgSys, "OpenID is invalid for read !."<< "\n");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			if (id == 0) {//id = 1 => file open with stdin mode => read with console
				buf = new char[charcount];
				SysReadString(buf, charcount); // call sysReadString to read content user input
				System2User(virtAddr, strlen(buf), buf);///push buf to the address of variable(char*)
				DEBUG(dbgSys, "SysReadString return: "<<buf<< "\n");

				kernel->machine->WriteRegister(2, strlen(buf) + 1); // return length of content
				delete[] buf;
				IncreasePC();
				return;
			}
			buf = User2System(virtAddr, MaxFileLength + 1);
			
			int before = kernel->fileSystem->openf[id]->GetCurrentPos();//get content want to write to the file
			if ((kernel->fileSystem->openf[id]->Read(buf, charcount)) > 0)
			{
				DEBUG(dbgSys, "Read file return: "<<buf<< "\n");
				
				int after = kernel->fileSystem->openf[id]->GetCurrentPos();//get current ptr in file after write
				System2User(virtAddr, charcount, buf);//push buf to the address of variable(char*)
				kernel->machine->WriteRegister(2, after - before + 1);	
				//printf("Buffer is : %s" , buf);
			} 
			else {
				kernel->machine->WriteRegister(2, -1);
			}
			delete[] buf;
			IncreasePC();
			return;
		}
		case SC_Write:
		{
			// Input: Address of variable(char*) want to write , maximum number of characters want to write, id file(OpenFileID)
			// Output: number of bytes write: success, -1: fail
			// Write to file
			int virtAddr = kernel->machine->ReadRegister(4);
			int charcount = kernel->machine->ReadRegister(5);
			int id = kernel->machine->ReadRegister(6);
			int i = kernel->fileSystem->index;
		
			if (id > i || id < 0) //check file is exist
			{
				DEBUG(dbgSys, "Try to open invalib file" << "\n");
				printf("Try to open invalib file");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			if (kernel->fileSystem->openf[id] == NULL) //check file is exist
			{
				DEBUG(dbgSys, "Try to open invalib file" << "\n");
				printf("Try to open invalib file");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			
			if(id == 0){//id = 0 => open file with stdin mode => can't write
				DEBUG(dbgSys, "OpenID is invalid for write !." << "\n");
				SysPrintString("OpenID is invalid for write !.\n");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			char *buf =  User2System(virtAddr, charcount);
			if(id == 1){// id = 1 => open file with stdout mode => write to console
				SysPrintString(buf);// call sysPrintString to print string to console
				System2User(virtAddr, strlen(buf) , buf); //push buf to the address of variable(char*)
				kernel->machine->WriteRegister(2, strlen(buf) + 1);
				delete[] buf;
				IncreasePC();
				return;
			}
			int countC=strlen(buf);
			buf[countC]='\0';
			int before = kernel->fileSystem->openf[id]->GetCurrentPos();//get current ptr in file
			if ((kernel->fileSystem->openf[id]->Write(buf, countC)) != 0)
			{
				int after = kernel->fileSystem->openf[id]->GetCurrentPos(); //get current ptr in file after write
				System2User(virtAddr, after - before, buf);//push buf to the address of variable(char*)
				kernel->machine->WriteRegister(2, after - before + 1);
				delete[] buf;
				IncreasePC();
				return;
			}
			else{
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
		}

			case SC_ReadString:
			{
				// Input: address of buffer(char*), the maximum characters want to read(int)
				// Output: non
				// read string and store to buffer(char*)
				int address = kernel->machine->ReadRegister(4);
				int limit = kernel->machine->ReadRegister(5);

				char *buff = new char[limit + 1];
				memset(buff, 0, limit + 1); // set all buff is '-'
				int countR = SysReadString(buff, limit + 1); //call sysReadString to read string
				int countS2U = System2User(address, limit + 1, buff);//push buf to the address of buffer(char*)

				DEBUG(dbgSys, "Number of entered characters " << countR << "\n");

				IncreasePC();

				return;

				ASSERTNOTREACHED();
			}
			case SC_RandomNum:
			{
				// Input: non
				// Output: random number

				int result2;
				result = SysRandomNum();// call SysRandomNum to get random number
				DEBUG(dbgSys, "RandomNum returning with " << result << "\n");
				/* Prepare Result */
				kernel->machine->WriteRegister(2, (int)result);
				IncreasePC();
				return;
				ASSERTNOTREACHED();
		  	}
			

			case SC_Create:
			{
				// Input: address of name(char*)
				// Output: 0: success, -1: fail
				int virtAddr;
				char* filename;
				virtAddr = kernel->machine->ReadRegister(4); 
				
				filename = User2System(virtAddr, MaxFileLength + 1);// get file name from address
				if (strlen(filename) == 0)// check file name is exist
				{
					DEBUG('a', "\n File name is not valid");
					kernel->machine->WriteRegister(2, -1); 
					IncreasePC();
					return;
				}
				
				if (filename == NULL)  // check file name is exist
				{
					DEBUG('a', "\n Not enough memory in system");
					kernel->machine->WriteRegister(2, -1);
					delete[] filename;
					IncreasePC();
					return;
				}
				//Use Create in filesys.h to create file
				if (kernel->fileSystem->Create(filename) == -1)  //create fail
				{
					DEBUG(dbgSys, "Error create file '" << filename << "'\n")
					kernel->machine->WriteRegister(2, -1);
					delete[] filename;
					IncreasePC();
					return;
				}
				
				DEBUG(dbgSys, "Create file '" << filename << "' successfully\n");
				kernel->machine->WriteRegister(2, 0);
				delete[] filename;
				
				IncreasePC();
				return;
				ASSERTNOTREACHED();
			}
			case SC_PrintNum:
			{
				//Input: number
				//Output: non
				
				int num = kernel->machine->ReadRegister(4);

				SysPrintNum(num);//call SysPrintNum to print number

				IncreasePC();
				return;
				ASSERTNOTREACHED();
		  	}
			case SC_PrintChar:
				{
				//Input: address of character(char)
				//Output: non
				char ch = kernel->machine->ReadRegister(4);
				SysPrintChar(ch);//call SysPrintChar to print number
				IncreasePC();
				return;
				ASSERTNOTREACHED();
				}
			case SC_PrintString:
			{
				//Input: address of buffer(char*)
				//Output: non
				int address = kernel->machine->ReadRegister(4);
				char *str = User2System(address, 255);// get string from address of buffer
				SysPrintString(str);//call SysPrintString to print number
				if (str != NULL)
					delete[] str;
				IncreasePC();
				return;
				ASSERTNOTREACHED();
			}
			case SC_Remove:
                                // Input: address of name(char*)
                                // Output: 0 : success, -1: fail
                                int virtAddr1;
                                char* filename1;
                                int temp;
                                virtAddr1 = kernel->machine->ReadRegister(4);
                               
                                filename1 = User2System(virtAddr1, MaxFileLength + 1);//get filename from address of name(char*)
                                if (strlen(filename1) == 0) //check file name is exist
                                {
                                        DEBUG('a', "\n File name is not valid");
                                        kernel->machine->WriteRegister(2, -1); 
                                        IncreasePC();
                                        return;
                                }
                               
                                if (filename1 == NULL)  // check file name is exist
                                {
                                        DEBUG('a', "\n Not enough memory in system");
                                        kernel->machine->WriteRegister(2, -1); 
                                        delete filename1;
                                        IncreasePC();
                                        return;
                                }
                                temp = check(openning, kernel->fileSystem->index); //check file name is exist in folder
                                if(temp == -1){
                                        DEBUG(dbgSys, "Error remove file '" << filename1 << "'\n");
                                        kernel->machine->WriteRegister(2, -1);
                                        delete filename1;
                                        IncreasePC();
                                        return;
                                }
                                kernel->fileSystem->Remove(filename1);// Use Remove in filesys.h to remove file
                                DEBUG(dbgSys, "Remove file '" << filename1 << "' successfully\n");
                                kernel->machine->WriteRegister(2, 0);
                                delete filename1;
                                IncreasePC();
                                return;
                                ASSERTNOTREACHED();
                                break;
                        default:
                                cerr << "Unexpected user mode exception" << (int)which << "\n";
                                break;
                        break;
                  }
      default:
                cerr << "Unexpected user mode exception" << (int)which << "\n";
                break;
        }
    ASSERTNOTREACHED();
}
