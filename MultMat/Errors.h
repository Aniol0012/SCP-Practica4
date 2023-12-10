//
// Created by Fernando Cores Prado on 4/12/23.
//

#ifndef MULTMAT_SEC3_ERRORS_H
#define MULTMAT_SEC3_ERRORS_H

#define DEBUG 0

extern char *COLOR_BLACK;
extern char *COLOR_RED;
extern char *COLOR_RED_B;
extern char *COLOR_GREEN;
extern char *COLOR_GREEN_B;
extern char *COLOR_YELLOW;
extern char *COLOR_BLUE;
extern char *COLOR_MAGENTA;
extern char *COLOR_CYAN;
extern char *COLOR_CYAN_B;
extern char *COLOR_RESET;

// Functions Prototypes
void Error(char *msg);
void printMessage(char *msg, char *color);
void print(char *msg);

#endif //MULTMAT_SEC3_ERRORS_H
