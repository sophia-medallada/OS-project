#ifndef EXECUTE_H
#define EXECUTE_H
#include <stdbool.h>
#include "parsing_interface.h"

#define MAX_JOBS 1024

typedef struct {
    int job_id;
    pid_t pid;
    char command[1024];
} BackgroundJob;

void execCmd(const ParsedCommand* command); //executes a parsed command
void handleCompletedJobs(); // Handle completed background jobs

extern int job_count; // Current number of background jobs
extern BackgroundJob jobs[MAX_JOBS];

#endif
