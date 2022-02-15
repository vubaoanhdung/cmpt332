#include <stdio.h> // printf
#include <string.h> // strcmp strtok strcpy
#include <stdlib.h> // malloc

#include "mylib.h"

void whiteSpaceRemover(char *command)
{
   if (command[strlen(command) - 1] == ' ' || command[strlen(command) - 1] == '\n')
   {
      command[strlen(command) - 1] = '\0';
   }
   if (command[0] == ' ' || command[0] == '\n')
   {
      memmove(command, command + 1, strlen(command));
   }
}

void clearTerminal()
{
   printf("\e[1;1H\e[2J");
}

char *duplicateCharacterCMPT(char output[], int numBytes)
{
   char *result;
   int i;
   int j = 0; // keep track of the index of the result
   result = (char *)malloc(4096);

   for (i = 0; i < numBytes; i++)
   {
      result[j] = output[i];
      j++;
      if (output[i] == 'c' || output[i] == 'm' || output[i] == 'p' || output[i] == 't')
      {
         result[j] = output[i];
         j++;
      }
   }

   result[j] = '\0'; // set the last character in the result string to \0
   memset(output, 0, strlen(output)); // clean up the output mem
   
   return (char *)result;
}

void reverseOrder(char **array, int numberOfItems)
{
   char *tempArray[numberOfItems];
   int i;
   for (i = 0; i < numberOfItems; i++)
   {
      // Note: the difference between number of items and the index
      // is 1. Therefore, subtract 1 to get the actual index in the new array
      tempArray[i] = array[numberOfItems - 1 - i];
   }

   // replace the memory for given array by the new created array
   // i.e. array = tempArray
   memcpy(array, tempArray, sizeof(char *) * numberOfItems);
}

void tokenize(char **listOfCommands, char *user_input, char *delimeter, int *numberOfCommands)
{
   char *token;
   int i = 0;

   token = strtok(user_input, delimeter);
   while (token != NULL)
   {
      listOfCommands[i] = malloc(sizeof(token) + 1);
      strcpy(listOfCommands[i], token);     // copy the command to the list of commands
      whiteSpaceRemover(listOfCommands[i]); // remove white spaces in the command
      token = strtok(NULL, delimeter);      // get a new token
      i++;
   }

   listOfCommands[i] = NULL; // put NULL at the end of the list
   *numberOfCommands = i;    // store how many number of commands are there
}
