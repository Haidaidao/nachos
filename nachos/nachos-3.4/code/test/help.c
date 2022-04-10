#include "syscall.h"
#include "copyright.h"

int main()
{
        
	PrintString("-Group of 4 people:\n");
       
        PrintString("+Dao Dai Hai-20127016\n");
	
        PrintString("+Nguyen Le Hoang Thong-20127078\n");
        
        PrintString("+Lai Minh Thong-20127635\n");
       
        PrintString("+Tran Anh Kiet-20127545\n");
        
        PrintString("======================================\n");
       
        PrintString("-Ideas for printing ASCII codes:\n");
       
        PrintString("+Use the function void Print Char(character) written in sentence g to print each character in the ascii encoding and combine it with a for loop.\n");
       
        PrintString("======================================\n");
        
        PrintString("-Ideas for sort codes:\n");
        
        PrintString("+Step 1, use the readInt function to enter the value n is the size of the array, If <= 100 print the string re-enter.\n");
       
        PrintString("+Step 2, next, allocate a dynamic array called A of size n\n");
        
        PrintString("+Step 3, we use bubble sort to sort the elements ascending\n");
        
        PrintString("+Step 4, use printArray function to output array after sorting\n");
       
        PrintString("+Step 5, frees up memory for pointer A\n");
       
	Halt();
	return 0;
}
