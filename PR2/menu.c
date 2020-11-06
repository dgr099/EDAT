#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
/*prototipos de privadas*/
int ShowProductsMenu();
int ShowOrdersMenu();
int ShowCustomersMenu();
/*fin de prototipos de privadas*/

int ShowMainMenu() {
    int nSelected = 0;
    char buf[16]; /* buffer for reading user input
                     in theory we only need 1 char
                     but a large buffer will handle
                     more gracefully wrong inputs
                     as 'qwerty' */

    do {
        printf("\n%sMAIN MENU:\n",YEL);
        puts(SELECT);
        printf("  1. Products.\n  2. Orders.\n  3. Customers.\n  %s4. Exit.\n",RED);
        puts(CHOICE);

        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) { /*si no esta entre las opciones*/
            puts(ERR);
        }
    } while ((nSelected < 1) || (nSelected > 4)); /*mientrás que sea un numero incorrecto*/

    if( nSelected == 4)
      return OUT;
    else
      return nSelected;
}

int ShowSubMenu(int *opt){
    switch (*opt)
    {
    case 1:
        return ShowProductsMenu();
        break;

    case 2:
        return ShowOrdersMenu();
        break;
    case 3:
        return ShowCustomersMenu();
        break;
    }
    return 0; /*opt incorrecta, no se puede dar el caso porque ShowMainMenu ya tiene comprobación pero por si acaso*/
}

int ShowProductsMenu() {
    int nSelected = 0;
    char buf[16]; /* buffer for reading user input
                     in theory we only need 1 char
                     but a large buffer will handle
                     more gracefully wrong inputs
                     as 'qwerty' */

    do {
        printf("\n%sPRODUCTS MENU:\n",YEL);
        puts(SELECT);
        printf("  1. Stock.\n  2. Find.\n%s  3. Back.\n",RED);
        puts(CHOICE);

        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 3)) { /*si no esta entre las opciones o si no ha leido nada el fgets*/
            puts(ERR);
        }
    } while ((nSelected < 1) || (nSelected > 3)); /*mientrás que sea un numero incorrecto*/

    if( nSelected == 3)
      return OUT;
    else
      return nSelected;
}

int ShowOrdersMenu() {
    int nSelected = 0;
    char buf[16]; /* buffer for reading user input
                     in theory we only need 1 char
                     but a large buffer will handle
                     more gracefully wrong inputs
                     as 'qwerty' */

    do {
        printf("\n%sORDERS MENU:\n",YEL);
        puts(SELECT);
        printf("  1. Open.\n  2. Range.\n  3. Detail.\n  %s4. Back.\n",RED);
        puts(CHOICE);

        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) { /*si no esta entre las opciones o si no ha leido nada el fgets*/
            puts(ERR);
        }
    } while ((nSelected < 1) || (nSelected > 4)); /*mientrás que sea un numero incorrecto*/

    if( nSelected == 4)
      return OUT;
    else
      return nSelected;
}

int ShowCustomersMenu() {
    int nSelected = 0;
    char buf[16]; /* buffer for reading user input
                     in theory we only need 1 char
                     but a large buffer will handle
                     more gracefully wrong inputs
                     as 'qwerty' */

    do {
        printf("\n%sCUSTOMERS MENU:\n",YEL);
        puts(SELECT);
        printf("  1. Find.\n  2. List Products.\n  3. Balance.\n  %s4. Back.\n",RED);
        puts(CHOICE);

        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) { /*si no esta entre las opciones o si no ha leido nada el fgets*/
            puts(ERR);
        }
    } while ((nSelected < 1) || (nSelected > 4)); /*mientrás que sea un numero incorrecto*/

    if( nSelected == 4)
      return OUT;
    else
      return nSelected;
}
