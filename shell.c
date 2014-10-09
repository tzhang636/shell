/* 
 * Machine Problem #2: Simple Shell 
 * CS 241, Spring 2011
 *
 * Main program for interactive shell.
 */

/* LIBRARY SECTION */
#include <ctype.h>              /* Character types                       */
#include <stdio.h>              /* Standard buffered input/output        */
#include <stdlib.h>             /* Standard library functions            */
#include <string.h>             /* String operations                     */
#include <sys/types.h>          /* Data types                            */
#include <sys/wait.h>           /* Declarations for waiting              */
#include <unistd.h>             /* Standard symbolic constants and types */
#include "vector.h"             /* Queue library */

/* CONSTANTS AND GLOBAL VARIABLES */
static const int SHELL_BUFFER_SIZE = 256;   /* Size of the Shell input buffer */

/* FUNCTION DECLARATIONS */
void select_command(char *input, vector_t *history, int *counter);

void cd(char *input, char *cpy_input, char *token2, vector_t *history, 
	int *counter);

void exit_function(char *input, char *cpy_input, vector_t *history, 
		   int *counter);

void print_command(char *input, char *cpy_input, char *token1, 
		   vector_t *history, int *counter);

void execute_command(char *input, char *cpy_input, char *token1, 
		     vector_t *history, int *counter);

void non_builtin(char *input, char *cpy_input, vector_t *history, int *counter);

/* MAIN PROCEDURE SECTION */
int main(int argc, char **argv)
{
   // initialize vector
   vector_t *history = (vector_t*) malloc(sizeof(vector_t));
   vector_init(history);

   // counter for command script
   int *counter = (int*) malloc(sizeof(int));	
   *counter = 1;

   // read-eval loop
   loop:
   while(1){
      pid_t myid = getpid();
      printf("Shell(pid=%d)%d} ", myid, *counter);
      
      // get user input command
      char *input = (char*) malloc(sizeof(char)*300);
      fgets(input, 270, stdin);
      	    
      // check for blank line
      if (strcmp(input, "\n") == 0){
	 free(input);
	 goto loop;
      }

      // check for overflow
      if ((int)strlen(input) > SHELL_BUFFER_SIZE){
	 printf("Commands should be within the size of 256 characters.\n");

         // clears stdin
	 int ch;
	 do
	    ch = getchar();
	 while (ch != EOF && ch != '\n');
	 clearerr(stdin);
	 free(input);
	 counter++;
      }

      // otherwise, select command
      else	 
         select_command(input, history, counter); 
   }

   return 0;

} /* end main() */

void select_command(char *input, vector_t *history, int *counter)
{
      
      // copies input into cpy_input
      char *cpy_input = (char*)malloc(sizeof(char)*300);
      strcpy(cpy_input, input);

      // parses input into two tokens
      char *token1 = strtok(input, " \n");
      char *token2 = strtok(NULL, " \n");

      // checks if the command contains just whitespaces
      if (token1 == NULL)
	 return;

      // Built-in Commands
      // cd xxx
      if (strcmp(token1, "cd") == 0)
	 cd(input, cpy_input, token2, history, counter);

      // exit
      else if (strcmp(token1, "exit") == 0)
	 exit_function(input, cpy_input, history, counter);
      
      // !#N
      else if (token1[0] == '!' && token1[1] == '#' && 
	       token1 == strstr(token1, "!#"))
	 print_command(input, cpy_input, token1, history, counter);
	 
      // !N
      else if (token1[0] == '!' && token1 == strstr(token1, "!"))
	 execute_command(input, cpy_input, token1, history, counter);
        
      // Non Built-in Commands
      else
	 non_builtin(input, cpy_input, history, counter);
}

void cd(char *input, char *cpy_input, char *token2, vector_t *history, 
	int *counter)
{
   vector_append(history, cpy_input);
   (*counter)++;

   if (token2 == NULL){
      chdir("..");
      free(input);
   }

   else{
      int ret_val = chdir(token2);
      if (ret_val == -1)
         printf("Not valid\n");
      free(input);
   }	
}

void exit_function(char *input, char *cpy_input, vector_t *history, 
		   int *counter)
{
   int i;
   for (i=0; i<(int)vector_size(history); i++)
      free(vector_at(history, i));
 
   vector_destroy(history);
   free(input);
   free(cpy_input);
   free(history);
   free(counter);
   exit(0);
}

void print_command(char *input, char *cpy_input, char *token1, 
		   vector_t *history, int *counter)
{
   // we know that *token1 == "!#..."

   int N = atoi(&token1[2]);

   if (strlen(token1) == 2){
      vector_append(history, cpy_input);
      int i;
      for (i=1; i<(int)vector_size(history); i++)
         printf("!%d: %s", i, (char*)vector_at(history, i-1));
      (*counter)++;
      free(input);
   }

   else if (N == 0){
      vector_append(history, cpy_input);
      (*counter)++;
      free(input);
   }

   else if (N > 0 && N <= (int)vector_size(history)){
      vector_append(history, cpy_input);
      int i;
      for (i=(int)vector_size(history)-N; i<(int)vector_size(history); i++)
	 printf("!%d: %s", i, (char*)vector_at(history, i-1));
      (*counter)++;
      free(input);
   }

   else{
      printf("Not valid\n");
      free(input);
      free(cpy_input);
   }
}

void execute_command(char *input, char *cpy_input, char *token1, 
		     vector_t *history, int *counter)
{
   // we know that *token1 == "!..."

   int N = atoi(&token1[1]);
   int orig_size = (int)vector_size(history);

   if (strlen(token1) == 1){
      printf("Not valid\n");
      free(input);
      free(cpy_input);
   }

   else if (N >= 0 && N <= (int)vector_size(history)){
      int i = orig_size-N;
      while (i<orig_size){
	 char *temp = (char*)malloc(sizeof(char)*300);
	 strcpy(temp, (char*)vector_at(history, i));
         printf("%s", temp);
	 select_command(temp, history, counter);
         
	 i++;
	 fflush(stdout);
      }
      free(input);
      free(cpy_input);
   }

   else{
      printf("Not valid\n");
      free(input);
      free(cpy_input);
   }
 
}

void non_builtin(char *input, char *cpy_input, vector_t *history, int *counter)
{
   vector_append(history, cpy_input);
   (*counter)++;
   system(cpy_input);
   free(input);
}


