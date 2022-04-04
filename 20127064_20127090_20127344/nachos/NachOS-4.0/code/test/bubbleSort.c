#include "syscall.h"

int main()
{
  int n; int j; int i; int choose; int temp; int a[101];
  // input and check
  do
  {
    PrintString("Input n: ");
    n = ReadNum();

    if (n > 100 || n < 1)
    {
      PrintString("ERROR: n <= 100\n");
    }
  } while (n > 100 || n < 1);
  //input a[i]
  for (i = 0; i < n; i++)
  {
    PrintString("a[");
    PrintNum(i);
    PrintString("] = ");
    a[i] = ReadNum();
  }
  //choose mode sort
  PrintString("1: Decrease\n");
  PrintString("2: Increase\n");

  PrintString("Choose sort order you want: ");
  choose = ReadNum();
  //choose and check
  while (choose < 1 || choose > 2)
  {
    PrintString("Choose sort order you want: ");
    choose = ReadNum();

    if (choose < 1 || choose > 2)
    {
      PrintString("ERROR: Just choose 1 or 2.\n");
    }
  }
  //Decrease sort with bubbleSort
  if (choose == 1)
  {
    for (i = 0; i < n; i++)
    {
      for (j = i + 1; j < n; j++)
      {
        if (a[i] < a[j])
        {
          temp = a[i];
          a[i] = a[j];
          a[j] = temp;
        }
      }
    }
  }
  //Increase sort with bubbleSort
  else if (choose == 2)
  {
    for (i = 0; i < n; i++)
    {
      for (j = i + 1; j < n; j++)
      {
        if (a[i] > a[j])
        {
          temp = a[i];
          a[i] = a[j];
          a[j] = temp;
        }
      }
    }
  }
  //Print
  for (i = 0; i < n; i++)
  {
    PrintNum(a[i]);
    PrintChar(' ');
  }
  PrintChar('\n');
  Halt();
}