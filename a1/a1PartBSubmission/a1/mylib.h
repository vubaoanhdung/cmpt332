#ifndef MY_LIB_H
#define MY_LIB_H

/*
   Given a string
   Return a new string with characters 'c' 'm' 'p' 't' duplicated
*/
char *duplicateCharacterCMPT(char output[], int numBytes);

/*
   Reversing the order of items inside the array
   Use this function to reverse the order of commands inside listOfCommands
   For example: 'grep -o Mak | ls' will become 'ls | grep -o Mak'
*/
void reverseOrder(char **array, int numberOfItems);

/*
   Using strtok to tokenize user_input and put all the commands from user_input 
   into listOfCommands. The number of commands will be stored under numberOfCommands variable
*/
void tokenize(char **listOfCommands, char *user_input, char *delimeter, int *numberOfCommands);

/*
   Removing the white space at the beginning or the end of an command
*/
void whiteSpaceRemover(char *command);

/*
   Clear the terminal
   Use at the start of the program
*/
void clearTerminal();

#endif