#include "parsing_interface.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//count number of tokens in the input
static int countToken(const char* input) {
    int count = 0; //define count of token
    char *copy = strdup(input); //define a copy of input that can be modify later
    char *token = strtok(copy, " \t\n"); //tokenize the input string
    while (token != NULL) { //in case token is not empty, create a loop through each token
        count++; //update token count
        token = strtok(NULL, " \t\n"); //keep tokenizing the string remained
    }
    free(copy); //free the string copy
    return count;
}

ParsedCommand* parseCmd(const char* input) { 
    ParsedCommand *command = malloc(sizeof(ParsedCommand)); //allocate memory for ParsedCommand
    command->args = NULL; //locate args to NULL
    command->infile = NULL; //locate input file to NULL
    command->outfile = NULL; //locate output file to NULL
    command->append = false; //locate append flag to false
    command->background = false; //locate backgroung flag to false
    int cntToken = countToken(input);
    if(cntToken == 0) {
        free(command); //in case there is not token, free the command structure
        return NULL; }
    command->args = malloc(sizeof(char*) * (cntToken + 1)); //allocate memory for args (+1 for NULL termination)
    char *copy = strdup(input); 
    char *token = strtok(copy, " \t\n");
    int argIndex = 0; //index of arguments in args

    while (token != NULL) {
        if (strcmp(token, "<") == 0) { //in case token is '<' (specifies an input file), the next token become the input file
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                command->infile = strdup(token); //store the input file in ParsedCommand
                if (command->infile == NULL) { //memory allocation failure handler
                    free_parsedCmd(command);
                    free(copy);
                    return NULL;
                }
            }
        } else if (strcmp(token, ">") == 0) { //in case token is '>' (specifies an output file), the next token become the output file
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                command->outfile = strdup(token); //store the output file in ParsedCommand
                if (command->outfile == NULL) { //memory allocation failure handler
                    free_parsedCmd(command);
                    free(copy);
                    return NULL;
                }
                command->append = false; //set append mode to false
            }
        } else if (strcmp(token, ">>") == 0) { //in case the token is '>>' (append to the file), the next token become the output file
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                command->outfile = strdup(token); //store the output file
                if (command->outfile == NULL) { //memory allocation failure handler
                    free_parsedCmd(command);
                    free(copy);
                    return NULL;
                }
                command->append = true; //set append mode to true
            }
        } else if (strcmp(token, "&") == 0) { //in case the token is '&' (execute background), set background flag to true
            command->background = true; 
        } else { //otherwise, define as a regular argument
            command->args[argIndex++] = strdup(token); //store the argument in args
            if (command->args[argIndex - 1] == NULL) { //memory allocation failure handler
                    free_parsedCmd(command);
                    free(copy);
                    return NULL;
                }
        }
        token = strtok(NULL, " \t\n"); //continue with the next token
    }
    command->args[argIndex] = NULL; 
    free(copy); //free input string copy
    return command; //return parsed command
}

//free the memory for ParsedCommand
void free_parsedCmd(ParsedCommand* command) {
    //in case the args is not empty, free each argument in the args array
    if (command->args != NULL) {
        for (int i = 0; command->args[i] != NULL; i++) {
            free(command->args[i]); //free each argument 
        }
        free(command->args); //free the args
    }
    if (command->infile != NULL) { //in case the input file is not empty, free the input redirection
        free(command->infile);
    }
    if (command->outfile != NULL) { //in case the output file is not empty, free the output redirection
        free(command->outfile);
    }
    free(command); //free the parsed command
}

//check the quit or exit command
bool quitCmd(const ParsedCommand* command) {
    if (command == NULL || command->args == NULL || command->args[0] == NULL) { //in case there is no arguments, this is not a quit command
        return false;
    }
    //if the argument is "quit" or "exit", return true
    return strcmp(command->args[0], "quit") == 0 || strcmp(command->args[0], "exit") == 0;
}
