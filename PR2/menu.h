#ifndef MENU_H
#define MENU_H

/*Macros para diferenciar texto por colores*/
#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"

#define OUT -1 /*OUT => salir del menu*/

/*strings que se usan repetidamente*/
#define SELECT "\x1B[33m Select any option:\x1B[0m"
#define CHOICE "\x1B[34m Enter a number that corresponds to your choice > \x1B[0m\b"
#define ERR "Wrong Input. Please try again\n"

int ShowMainMenu();
int ShowSubMenu(int *opt);
#endif
