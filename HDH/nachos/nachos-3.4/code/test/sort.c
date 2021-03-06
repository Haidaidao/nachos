/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"
#define N 100


int main()
{
	int i, j, tmp, n;
	int A[N];

    	/* first input the array, in reverse sorted order */
	PrintString("Enter the number of elements: ");
	n = ReadInt();
	
	for (i = 0; i < n; i++)		
	{
		PrintString("Enter A[");
		PrintInt(i);
		PrintString("] = ");
		A[i] = ReadInt();
	}

	
	PrintString("Array: ");
	for (i = 0; i < n; i++)		
   	{
		PrintChar(' ');
		PrintInt(A[i]);	
	}
	
	PrintString("\nSorting... \n");

    	/* then sort! */
		 for(i=0;i<n;i++){
				for(j=i+1;j<n;j++){
				    if(A[i] > A[j]){
				        tmp = A[i];
				        A[i] = A[j];
				        A[j] = tmp;
				    }
				}
			}
	
	PrintChar('\n');
	
	PrintString("Array: ");
	for (i = 0; i < n; i++)		
   	{
		PrintChar(' ');
		PrintInt(A[i]);	
	}
	

	//delete[]A;
 	/* and then we're done -- should be 0! */
	Halt();
	return 0;
}
