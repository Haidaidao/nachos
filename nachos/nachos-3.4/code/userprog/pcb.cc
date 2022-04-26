#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"

PCB::PCB(int id)
{
    	joinsem= new Semaphore("JoinSem",0);
	exitsem= new Semaphore("ExitSem",0);
	mutex= new Semaphore("Mutex",1);
	pid= id;
	exitcode= 0;
	numwait= 0;
	if(id)
		parentID= currentThread->processID;
	else
		parentID= -1;
	thread= NULL;
	JoinStatus= -1;

}
PCB::~PCB()
{
	
	if(joinsem != NULL)
		delete this->joinsem;
	if(exitsem != NULL)
		delete this->exitsem;
	if(mutex != NULL)
		delete this->mutex;
	if(thread != NULL)
	{		
		thread->FreeSpace();
		thread->Finish();
		
	}
}

//------------------------------------------------------------------
int PCB::GetID()
{
	//return pid;
	return thread->processID;
}

int PCB::GetNumWait()
{
	return numwait;
}

int PCB::GetExitCode()
{
	return exitcode;	
}

void PCB::SetExitCode(int ec)
{
	exitcode= ec;
}

void PCB::IncNumWait()
{
	mutex->P();
	++numwait;
	mutex->V();
}

void PCB::DecNumWait()
{
	mutex->P();
	if(numwait > 0)
		numwait--;
	mutex->V();
}

char* PCB::GetNameThread()
{
	return thread->getName();
}

void PCB::SetPName(char* name)
{ 
	strcpy(pName,name);
}

char* PCB::GetPName() 
{ 
	return this->pName; 
}

//-------------------------------------------------------------------
void PCB::JoinWait()
{
    	JoinStatus= parentID;
	IncNumWait();
	joinsem->P();
}

void PCB::JoinRelease()
{ 
	DecNumWait();
	joinsem->V();
}

void PCB::ExitWait()
{ 
    	exitsem->P();
}


void PCB::ExitRelease() 
{	
    	exitsem->V();
}

//------------------------------------------------------------------
void MyStartProces(int id)
{
    char* filename = pTab->GetProcessName(id);
	OpenFile *excutable = fileSystem->Open(filename);
    AddrSpace *space;
    space = new AddrSpace(excutable);

	if(space == NULL)
	{
		printf("\nPCB::Exec : Can't create AddSpace.");
		return;
	}

    currentThread->space = space;

    space->InitRegisters();		
    space->RestoreState();		

    machine->Run();		
    ASSERT(FALSE);		
}

int PCB::Exec(char* filename, int pID)
{  
	// Goi mutex->P(); de giup tranh tinh trang nap 2 tien trinh cung 1 luc
	mutex->P();

	// Kiem tra thread da khoi tao thanh cong chua, neu chua thi bao loi la khong du bo nho, goi mutex->V() va return       
	thread = new Thread(filename);

	if(thread == NULL){
		printf("\nLoi: Khong tao duoc tien trinh moi !!!\n");
        	mutex->V();
		return -1;
	}

	// Dat processID cua thread nay la id
	thread->processID = pID;
	// Dat parentID cua thread nay la processID cua thread goi thuc thi Exec
	parentID = currentThread->processID;

	// Goi thuc thi Fork(StartProcess_2,id) => Ta cast thread thanh kieu int, sau do khi xu ly ham StartProcess ta cast Thread ve dung kieu cua no
	// StartProcess_2 chinh la ham MyStartProcess
 	thread->Fork(MyStartProces,pID);

    	mutex->V();
	// Tra ve id
	return pID;

}

//***********************************************************************************

