//
// Created by Fernando Cores Prado on 4/12/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "Errors.h"

// Constants
char *COLOR_BLACK   = "\033[0;30m";
char *COLOR_RED     = "\033[0;31m";
char *COLOR_RED_B   = "\033[1;31m";
char *COLOR_GREEN   = "\033[0;32m";
char *COLOR_GREEN_B = "\033[1;32m";
char *COLOR_YELLOW  = "\033[0;33m";
char *COLOR_BLUE    = "\033[0;34m";
char *COLOR_MAGENTA = "\033[0;35m";
char *COLOR_CYAN    = "\033[0;36m";
char *COLOR_CYAN_B  = "\033[1;36m";
char *COLOR_RESET   = "\033[0m";

/*
* Show Error message and finish application
*/
void Error(char *msg)
{
    char infoColorError[200];

    sprintf(infoColorError, "%s%s: %s%s\n", COLOR_RED_B, msg, strerror(errno), COLOR_RESET);
    write(STDERR_FILENO, infoColorError, strlen(infoColorError));
    exit(errno);
}

/*
* Show Error message and finish application
*/
void printMessage(char *msg, char *color)
{
    char infoColorMsg[200];

    sprintf(infoColorMsg, "%s%s%s", color, msg, COLOR_RESET);
    write(STDOUT_FILENO, infoColorMsg, strlen(infoColorMsg));
}


/*
* Show Error message and finish application
*/
void print(char *msg)
{
    printMessage(msg, COLOR_BLUE);
}

