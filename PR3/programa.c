#include "menu.h"
int main(){
    int cod;
    short use=0;
    char tabla[MAX_NOMBRE];
    do{
        cod=showmenu(tabla, &use);
    }while(cod==0);
    if(cod==4)
        return 0;
    return 1;
}