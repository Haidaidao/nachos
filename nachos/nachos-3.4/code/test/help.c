#include "syscall.h"
#include "copyright.h"

int main()
{
        
	PrintString("-Group of 4 people:");
        PrintChar('\n');
        PrintString("+Dao Dai Hai-20127016");
	PrintChar('\n');
        PrintString("+Nguyen Le Hoang Thong-20127078");
        PrintChar('\n');
        PrintString("+Lai Minh Thong-20127635");
        PrintChar('\n');
        PrintString("+Tran Anh Kiet-20127545");
        PrintChar('\n');
        PrintString("======================================");
        PrintChar('\n');
        PrintString("-Ideas for printing ASCII codes:");
        PrintChar('\n');
        PrintString("+Use the function void Print Char(character) written in sentence g to print each character in the ascii encoding and combine it with a for loop.");
        PrintChar('\n');
        PrintString("======================================");
        PrintChar('\n');
        PrintString("-Ideas for sort codes:");
        PrintChar('\n');
        PrintString("+Step 1, use the readInt function to enter the value n is the size of the array, If <= 100 print the string re-enter.");
        PrintChar('\n');
        PrintString("+Step 2, next, allocate a dynamic array called A of size n");
        PrintChar('\n');
        PrintString("+Step 3, we use bubble sort to sort the elements ascending");
        PrintChar('\n');
        PrintString("+Step 4, use printArray function to output array after sorting");
        PrintChar('\n');
        PrintString("+Step 5, frees up memory for pointer A");
       
	Halt();
	return 0;
}