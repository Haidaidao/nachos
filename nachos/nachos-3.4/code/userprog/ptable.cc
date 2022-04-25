#include "ptable.h"
#include "system.h"

#define For(i,a,b) for (int i = (a); i < b; ++i)

PTable::PTable(int size)
{
	int i = 0;
	psize = size;
	bm = new BitMap(size);
	bmsem = new Semaphore("BMsem",1);
		
	for(i = 0 ; i < MAX_PROCESS ; ++i)
		pcb[i] = NULL;
	bm->Mark(0);

	pcb[0] = new PCB(0);
	pcb[0]->SetPName("./test/scheduler");
	pcb[0]->parentID = -1;
}

PTable::~PTable()
{
	int i=0;
	if(bm!=NULL)
		delete bm;
	if(bmsem!=NULL)
		delete bmsem;
	for(i=0; i<MAX_PROCESS; i++)
		if(pcb[i]!=NULL)
			delete pcb[i];
}

//--------------------------------------------------------------------

int PTable::ExecUpdate(char* filename)
{
        //Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
	bmsem->P();			//chi nap 1 tien trinh vao mot thoi diem


//Kiem tra file co ton tai tren may khong
// Kiểm tra tính hợp lệ của chương trình “name”.
	if(filename == NULL)
	{
		printf("\nPTable::Exec : Can't not execute name is NULL.\n");
		bmsem->V();
		return -1;
	}
// Kiểm tra sự tồn tại của chương trình “name” bằng cách gọi phương thức Open của lớp fileSystem.
	OpenFile *executable = fileSystem->Open(filename);
	if (executable == NULL) 
	{
		printf("\nUnable to open file %s\n", filename);
		bmsem->V();
		return -1;
    	}
	delete executable;			// close file
////////////////////////////////////////////////////////////
	
	// So sánh tên chương trình và tên của currentThread để chắc chắn rằng chương trình này không gọi thực thi chính nó.
//Kiem tra chuong trinh duoc goi co la chinh no khong
	if(!strcmp(filename,currentThread->getName()))
	{
		printf("\nLoi: khong duoc phep goi exce chinh no !!!\n");
		bmsem->V();
		return -1;
	}
////////////////////////////////////////////////////////////
//Kiem tra con slot trong khong
	int ID= GetFreeSlot();
	if(ID==-1)
	{
		printf("\nLoi: Da vuot qua 10 tien trinh !!!\n");
		bmsem->V();
		return -1;
	}
////////////////////////////////////////////////////////////
//Nếu có slot trống thì khởi tạo một PCB mới với processID chính là index của slot này
	pcb[ID]= new PCB(ID);
	pcb[ID]->SetPName(filename); 
	
// parrentID là processID của currentThread
    	pcb[ID]->parentID = currentThread->processID;

	// bm->Mark(ID); GetFreeSlot call bm->Find() actually also marked the new slot
	int pID= pcb[ID]->Exec(filename,ID);

// Gọi bmsem->V()
	bmsem->V();
// Trả về kết quả thực thi của PCB->Exec.
	return pID;
}


int PTable::ExitUpdate(int ec)
{             
//Kiem tra pID co ton tai khong
	int pID= currentThread->processID;
	if(!IsExist(pID))
	{
		printf("\nLoi: Tien trinh khong ton tai !!!\n");
		return -1;
	} 

//////////////////////////////////////////////////////////////

//Neu la main process thi Halt()
	if(pID==0)
	{
		currentThread->FreeSpace();	
		interrupt->Halt();
		return 0;
	}

        if(!IsExist(pID))
	{
		printf("\nLoi: Tien trinh khong ton tai !!!\n");
		return -1;
	}

	pcb[pID]->SetExitCode(ec);
	pcb[pcb[pID]->parentID]->DecNumWait();

	if(pcb[pID]->JoinStatus != -1)
	{
		pcb[pID]->JoinRelease();
		pcb[pID]->ExitWait();
		Remove(pID);	
	}
	else
		Remove(pID);
	
	return ec;
}

int PTable::JoinUpdate(int pID)
{
	if(pID <= 0 || pID > 9)
	{
		printf("\nLoi: Khong ton tai process: id = %d\n",pID);
		return -1;
	}

	if (pcb[pID] == NULL)
	{
		printf("Loi: Khong ton tai process id nay!");
		return -1;
	}

	if(currentThread->processID != pcb[pID]->parentID)
	{
		printf("\nLoi: Ko duoc phep join vao tien trinh khong phai cha cua no !!!\n");
		return -1;
	}

/////////////////////////////////////////////////////////////////////////////////////////////
	pcb[pcb[pID]->parentID]->IncNumWait();

	pcb[pID]->JoinWait(); 	//doi den khi tien trinh con ket thuc

	int ec = pcb[pID]->GetExitCode();

	if(ec != 0)
	{
		printf("\nProcess exit with exitcode EC = %d ",ec);
		return -1;
	}

	pcb[pID]->ExitRelease();	//cho phep tien trinh con ket thuc
	
	return 0;
}

void PTable::Remove(int pID)
{
	if(pID<0 || pID>9)
		return;
	if(bm->Test(pID))
	{
		bm->Clear(pID);
		if(pcb[pID] != 0) delete pcb[pID];
	}
}

//----------------------------------------------------------------------------------------------
// Find free slot in order to save the new process infom
int PTable::GetFreeSlot()
{
	return bm->Find();
}

// Check if Process ID is Exist
bool PTable::IsExist(int pID)
{
	if(pID<0 || pID>9)
		return 0;
	return bm->Test(pID);
}

char* PTable::GetProcessName(int pID)
{
	if(pID>=0 && pID<10 && bm->Test(pID))
		return (pcb[pID]->GetPName());
}

