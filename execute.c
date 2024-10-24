#include "execute.h"
#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // POSIX operating system API
#include <sys/types.h> // Data types
#include <sys/wait.h> // waitpid function
#include <fcntl.h> // For open()


// execute parsed command
void execCmd(const ParsedCommand* command) {
    if (quitCmd(command)) {
        exit(0);
    }

    pid_t pid = fork(); // fork a new process
    // child process
    if (pid == 0) { 
        if (command->infile != NULL) {
            int inputFile = open(command->infile, O_RDONLY);
            if (inputFile < 0) {
                perror("Cannot open input file");
                exit(1);
            }
            dup2(inputFile, STDIN_FILENO); // redirect standard input to inputFile
            close(inputFile);
        }
        if (command->outfile != NULL) {
            FILE *file; // handle the output redirection, declare a pointer for the output file
            if (command->append) {
                file = fopen(command->outfile, "append"); // open file for append
            } else {
                file = fopen(command->outfile, "write"); // open file for writing
            }
            if (file == NULL) { // in case the file is empty or failed opening, print error message
                perror("File opening failure"); 
                exit(1); // then terminate the child process
            }
            dup2(fileno(file), STDOUT_FILENO); // redirect standard output to file descriptor
            fclose(file); // after file redirection, close file pointer
        }
        // execute the whole command from user input
        execvp(command->args[0], command->args); // replace current process with a new process
        perror("execvp fail"); // print error message if execvp fails
        exit(1); // exit child process
    } 
    else if (pid < 0) {
        perror("Fork failed"); // print fork error message
    } 
    else { //Sophia's code
        // Parent process
        if (command->background) {
            // Background execution: do not wait
            if (job_count < MAX_JOBS) {
                jobs[job_count].job_id = job_count + 1;
                jobs[job_count].pid = pid;
                strncpy(jobs[job_count].command, command->args[0], sizeof(jobs[job_count].command) - 1);
                jobs[job_count].command[sizeof(jobs[job_count].command) - 1] = '\0'; // ensure null-termination
                printf("Background job started: [%d] %d %s &\n", jobs[job_count].job_id, jobs[job_count].pid, jobs[job_count].command);
                job_count++;
            } else {
                fprintf(stderr, "Maximum number of background jobs reached.\n");
            }
        } else {
            waitpid(pid, NULL, 0); // wait until the child process is finished
        }
    }
}
//sophia's code
// Handle completed background jobs
void handleCompletedJobs() {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // Find the job in the jobs array
        for (int i = 0; i < job_count; i++) {
            if (jobs[i].pid == pid) {
                printf("Completed: [%d] %d %s &\n", jobs[i].job_id, jobs[i].pid, jobs[i].command);
                // Remove the job from the array by shifting others
                for (int j = i; j < job_count - 1; j++) {
                    jobs[j] = jobs[j + 1];
                }
                job_count--;
                break;
            }
        }
    }
}
