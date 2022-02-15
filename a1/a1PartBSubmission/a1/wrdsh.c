// Damon Vu
// 11261393
// bav965

#include <stdlib.h>
#include <stdio.h> // printf, fprintf
#include <unistd.h> // fork
#include <sys/wait.h> // wait
#include <string.h> // strcpy, strcat, ctrcmp, ctrtok, ctrchr
#include <fcntl.h> // read, write 

#include "mylib.h" // whiteSpaceRemover(), tokenize(), reverseOrder(), clearTerminal(), duplicateCharacterCMPT()

void performExecution(char *user_input, int numberOfCommands, int isRedirection, char *redirectDestination)
{
   char *path = "/bin/";
   char command[20];
   char *argv[100];
   int fd[2];

   pipe(fd);
   tokenize(argv, user_input, " ", &numberOfCommands);
   strcpy(command, path);
   strcat(command, argv[0]);

   int pid = fork();
   if (pid < 0)
   {
      fprintf(stderr, "fork failed\n");
   }
   else if (pid == 0)
   {
      // Child
      close(fd[0]);
      dup2(fd[1], STDOUT_FILENO);
      close(fd[1]);

      // execute the command and check for error
      if (execvp(command, argv) < 0)
      {
         printf("Unknown command! Please try again!\n");
         exit(1);
      }
   }
   else
   {
      // Parent
      char buffer[4096]; // NOTE: limitation -> if the output is too long, it will be cut off
      close(fd[1]);
      int numBytes = read(fd[0], buffer, sizeof(buffer));
      close(fd[0]);
      char *result = duplicateCharacterCMPT(buffer, numBytes);

      if (isRedirection)
      {
         int file = open(redirectDestination, O_WRONLY | O_CREAT, 0777);
         write(file, result, strlen(result));
         close(file);
      }
      else
      {
         write(STDOUT_FILENO, result, strlen(result));
      }
      wait(NULL);
   }
}

void performExecuteWithPipes(char **listOfCommands, int numberOfCommands, int isRedirection, char *redirectDestination)
{
   int fd[numberOfCommands][2]; // 2D array, each entry is a fd[2]
   int index;                   // keep track of the index in the 2D array

   char *path = "/bin/";
   char command[20];
   char *argv[100];
   int parameterCount; // count of the numbers of parameters in EACH command

   reverseOrder(listOfCommands, numberOfCommands); // reverse the order of commands

   for (index = 0; index < numberOfCommands; index++)
   {
      tokenize(argv, listOfCommands[index], " ", &parameterCount);
      pipe(fd[index]);

      int rc = fork();
      if (rc < 0)
      {
         fprintf(stderr, "fork failed!!!\n");
         exit(1);
      }
      else if (rc == 0)
      {
         // Child

         // if the command is NOT the last command
         if (index != numberOfCommands - 1)
         {
            close(fd[index][0]);               // close read end of pipe
            dup2(fd[index][1], STDOUT_FILENO); // copy the write end to STDOUT_FILENO
            close(fd[index][1]);               // close the write end
         }

         // if the command is NOT the first command
         if (index != 0)
         {
            close(fd[index - 1][1]); // close the write end of the previous one
            dup2(fd[index - 1][0], STDIN_FILENO);
            close(fd[index - 1][0]); // close the read end of the previous one

            // if the command is the last command
            // dup it to STDOUT_FILENO so that we can catch it in the parent
            if (index == numberOfCommands - 1)
            {
               close(fd[index][0]);
               dup2(fd[index][1], STDOUT_FILENO);
               close(fd[index][1]);
            }
         }

         strcpy(command, path);
         strcpy(command, argv[0]);
         if (execvp(command, argv) < 0)
         {
            printf("Execution failed\n");
            exit(1);
         };
      }
      else
      {
         // Parent
         if (index != 0)
         {
            close(fd[index - 1][1]);
            close(fd[index - 1][0]);
         }

         // catch the last command output and modify it
         if (index == numberOfCommands - 1)
         {
            char buffer[4096];
            close(fd[index][1]);
            int numBytes = read(fd[index][0], buffer, sizeof(buffer));
            close(fd[index][0]);
            char *result = duplicateCharacterCMPT(buffer, numBytes);

            if (isRedirection)
            {
               int file = open(redirectDestination, O_WRONLY | O_CREAT, 0777);
               write(file, result, strlen(result));
               close(file);
            }
            else
            {
               write(STDOUT_FILENO, result, strlen(result));
            }
         }
         wait(NULL);
      }
   }
}

int main()
{
   char user_input[1024]; // store user input
   char *listOfCommands[100];
   int numberOfCommands = 0;
   int firstTimeStartUp = 1;
   char redirectDestination[1024]; // for output redirection if '<' exists
   int isRedirection = 0;

   while (1)
   {
      if (firstTimeStartUp)
      {
         clearTerminal();
         firstTimeStartUp = 0;
      }
      printf("wrdsh> ");
      if (!fgets(user_input, 1024, stdin))
      {
         break;
      }

      //remove the \n character at the end
      size_t length = strlen(user_input);
      if (user_input[length - 1] == '\n')
      {
         user_input[length - 1] = '\0';
      }

      whiteSpaceRemover(user_input);
      // exit the program if user enter 'exit'
      if (strcmp(user_input, "exit") == 0)
      {
         break;
      }

      // if user hit enter without any input from keyboard -> do nothing -> continue the loop
      else if (strcmp(user_input, "") == 0)
      {
         continue;
      }

      // Redirection case
      else if (strchr(user_input, '<'))
      {
         isRedirection = 1;
         int i;
         char *user_input_pointer = user_input; // point to user_input after cut off the redirection info

         for (i = 0; i < strlen(user_input); i++)
         {
            if (user_input[i] == '<')
            {
               break;
            }
            redirectDestination[i] = user_input[i];
         }
         redirectDestination[i] = '\0';
         memmove(user_input_pointer, user_input_pointer + i + 1, strlen(user_input_pointer));

         // check pipes
         if (strchr(user_input_pointer, '|'))
         {
            tokenize(listOfCommands, user_input_pointer, "|", &numberOfCommands);
            performExecuteWithPipes(listOfCommands, numberOfCommands, isRedirection, redirectDestination);
         }
         else
         {
            performExecution(user_input_pointer, numberOfCommands, isRedirection, redirectDestination);
         }
         isRedirection = 0;
      }

      // if pipes exist in the user_input
      // go this route
      else if (strchr(user_input, '|'))
      {
         tokenize(listOfCommands, user_input, "|", &numberOfCommands);
         performExecuteWithPipes(listOfCommands, numberOfCommands, isRedirection, redirectDestination);
      }

      // otherwise, go this route
      // this is a normal route with a single call
      else
      {
         performExecution(user_input, numberOfCommands, isRedirection, redirectDestination);
      }
   }
   return 0;
}
