#ifndef EXECUTE_H
#define EXECUTE_H
#include <stdbool.h>
#include "parsing_interface.h"

void execCmd(const ParsedCommand* command); //executes a parsed command
void execBuiltin(const ParsedCommand* command); //executes a built-in command
bool backgroundJob(const ParsedCommand* command); //check background job command

#endif
