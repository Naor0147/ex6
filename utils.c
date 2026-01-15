/***********
 ID:329218416
 NAME:Naor Biton
 ASSIGNMENT:ex6
***********/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int getInt(const char *prompt)
{
    printf("%s", prompt);
    int value = -1;
    scanf("%d", &value);
    clearBuffer();
    return value;
}

char *getString(const char *prompt)
{
    printf("%s", prompt);
    return getStringScanf();
}

// get string from buffer
char *getStringScanf()
{
    int stringSize = 1; // start from 1 to save \0
    char *string = (char *)malloc(stringSize * (sizeof(char)));
    char CurrentChar = getchar(); // gets the first char
    int CurrentChar = getchar(); // gets the first char

    // if the char is not new line keep getting chars
    while (CurrentChar != '\n')
    while (CurrentChar != '\n' && CurrentChar != EOF)
    {
        string[stringSize - 1] = CurrentChar;                  // save char
        string[stringSize - 1] = (char)CurrentChar;            // save char
        stringSize++;                                          // increase size
        string = realloc(string, stringSize * (sizeof(char))); // make space for new char
        CurrentChar = getchar();                               // get the next char
    }
    string[stringSize - 1] = 0; // end of file
    return string;              // return the pointer to the string
}

void clearBuffer()
{
    int leftOverChar;

    while (1)
    {
        leftOverChar = getchar();

        // If we hit a newline or the end of the input stops the program
        if (leftOverChar == '\n' || leftOverChar == EOF)
        {
            return;
        }
    }
}