
#include "quash.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>



void initQuash() { //define quash
    printf("Welcome to Quash!\n"); 
}

void Quash() { //main function of Quash
     char input[1024]; //buffer for holding the input command
     while (1) { //create loop for the running shell
         printf("[QUASH]$ "); //print the prompt
        if (fgets(input, sizeof(input), stdin) == NULL) { //in case the input is empty or there is an error while reading input
            perror("fgets"); //print error message
            break; //then break the loop
        }
        ParsedCommand command; //parse the input command
        if (parseCmd(input, &command)) {
            execCmd(&command); //execute the command if parsing was successful
            continue;
        }

        free_parsedCmd(&command); //free the command after executing

     }
     return;
}

void quashCleanup() { //free all resources befor exiting
    printf("Exit...\n");
}

int main() {
    initQuash();
    Quash();
    quashCleanup();
    return 0;
}
