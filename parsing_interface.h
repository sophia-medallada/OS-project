#ifndef PARSING_INTERFACE_H
#define PARSING_INTERFACE_H
#include <stdbool.h>

typedef struct {
    char *args; //define array of arguments
    char *infile; //input redirection
    char *outfile; //output redirection
    bool append; //flag indicating
    bool background; //flag indicating
} ParsedCommand;

//Function prototypes
ParsedCommand* parseCmd(const char* input); //parse command input
void free_parsedCmd(ParsedCommand* command); //free parse command
void quitCmd(const ParsedCommand* command); //check quit or exit command


#endif
