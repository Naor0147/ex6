#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"



char* getString(const char* prompt)
{
    printf("%s\n",prompt);
    return getStringScanf;
}


// get string from buffer 
char *getStringScanf()
{
    int stringSize = 1; // start from 1 to save \0
    char *string = (char *)malloc(stringSize * (sizeof(char)));
    char CurrentChar = getchar(); // gets the first char

    // if the char is not new line keep getting chars
    while (CurrentChar != '\n')
    {
        string[stringSize - 1] = CurrentChar;                  // save char
        stringSize++;                                          // increase size
        string = realloc(string, stringSize * (sizeof(char))); // make space for new char
        CurrentChar = getchar();                               // get the next char
    }
    string[stringSize - 1] = 0; // end of file
    return string;              // return the pointer to the string
}