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
    	// Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
	mutex->P();

    	// Kiểm tra thread đã khởi tạo thành công chưa, nếu chưa thì báo lỗi là không đủ bộ nhớ, gọi mutex->V() và return.             
	thread = new Thread(filename);

	if(thread == NULL){
		printf("\nLoi: Khong tao duoc tien trinh moi !!!\n");
        	mutex->V();
		return -1;
	}

	//  Đặt processID của thread này là id.
	thread->processID = pID;
	// Đặt parrentID của thread này là processID của thread gọi thực thi Exec
	parentID = currentThread->processID;
	// Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó.
 	thread->Fork(MyStartProces,pID);

    	mutex->V();
	// Trả về id.
	return pID;

}

//***********************************************************************************

