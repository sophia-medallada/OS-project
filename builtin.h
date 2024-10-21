#ifndef BUILTIN_H
#define BUILTIN_H

#include "parsing_interface.h"
#include <stdbool.h>

bool cd(const ParsedCommand* command); //change directory command
bool pwd(const ParsedCommand* command); //check current working directory
bool echo(const ParsedCommand* command); //print stdout
bool export(const ParsedCommand* command); //set up enviroment variables
bool jobsCmd(const ParsedCommand* command); //list of background jobs

//check if a command is a builtin command
bool isBuiltin(const ParsedCommand* command);

//execute the builtin command
bool execBuiltin(const ParsedCommand* command);

#endif
