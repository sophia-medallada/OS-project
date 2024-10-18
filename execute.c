#include "parsing_interface.c"
#include "execute.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //POSIX operating system API
#include <sys/types.h> //data types
#include <sys/wait.h> //waitpid function


//execute parsed command
void execCmd(const ParsedCommand* command) {
    pid_t pid = fork(); //fork a new process
    //child process
    if (pid == 0) { 
        if (command->outfile != NULL) {
            FILE *file; //handle the output redirection, declare a pointer for the output file
            if (command->append) {
                file = fopen(command->outfile, "append"); //open file for append
            } else {
                file = fopen(command->outfile, "write"); //open file for writing
            }
            if (file == NULL) { //in case the file is empty or fail opening the file, print error message
                perror("File opening failure"); 
                exit(1); //then terminate the child process
            }
            dup2(fileno(file), STDOUT_FILENO); //redirect standard output to file descriptor
            fclose(file); //after file redirecting, close file pointer
        }
        //execute the whole command from the user input
        execvp(command->args[0], command->args); //replace current process wirh a new process
        perror("Cannot execute command"); //print error message if execvp fails
        exit(1); //exit child process
        } else if (pid < 0) {
            perror("Fork failed"); //print out fork error message
        } else {
            waitpid(pid, NULL, 0); //wait until the child process is finished
        }
}
