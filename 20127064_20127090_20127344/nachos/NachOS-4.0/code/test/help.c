#include"syscall.h"


int main(){
    //use PrintString to print about project
    PrintString("\t-----> INFOMATION <-----\t\n");
    PrintString("Le Thanh Tu  ----- 201270790 \n");
    PrintString("Vo Hien Hai Thuan ----- 20127344\n");
    PrintString("Nguyen Tran Mai Phuong   ----- 20127064\n\n");

    PrintString("\t----  SORT  ----\t\n");
    PrintString("\t---Version: BUBBLE SORT---\t\n");
    PrintString("Bubble sort, sometimes referred to as sinking sort\n");
    PrintString("Bubble Sort is the simplest sorting algorithm\n");
    PrintString("Works by repeatedly swapping the adjacent elements if they are in wrong order.\n");

    PrintString("\t----  ASCII ----\t\n");
    PrintString("Abbreviated from American Standard Code for Information Interchange\n");
    PrintString("ASCII codes represent text in computers\n");
    PrintString("These include upper and lowercase English letters, numbers, and punctuation symbols\n\n");

    PrintString("\t---- CREATE FILE ----\t\n");
    PrintString("\tInput: file name\t\n");
    PrintString("\tSystem will create a new file with the file name being the data entered by user\t\n");

    PrintString("\t---- REMOVE FILE ----\t\n");
    PrintString("\tInput: fileName\t\n");
    PrintString("\tSystem will check and remove 'fileName' if that file exists \t\n");

    PrintString("\t---- CAT ---- \t\n");
    PrintString("\tInput: fileName\t\n");
    PrintString("\tSystem will display the content contained in 'fileName' \t\n");

    PrintString("\t---- COPY ----\t\n");
    PrintString("\tInput: srcName , destName\t\n");
    PrintString("\tSystem will copy the data contained in file 'srcName' and paste those data into file 'destName'\t\n");

    Halt();
}

  /* not reached */
