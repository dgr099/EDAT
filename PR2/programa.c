#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include "menu.h"
#include "odbc.h"
#include "consultas.h"

int main(){
    int opt=0, sub_opt=0; /*aquí se guarda la opción y la sub opción de la opción, por ejemplo products y stock*/
    SQLHENV env;
    SQLHDBC dbc;
    SQLRETURN ret;
    printf("Connecting...\n");
    if(!SQL_SUCCEEDED(ret=odbc_connect(&env, &dbc))){ /*si ocurre un error al conectar con la base de datos*/
        fprintf(stderr, "Failed to connect\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }
     
    
    do{
        opt=ShowMainMenu(); /*guarda la opción del menu*/
        sub_opt=ShowSubMenu(&opt); /*guarda la opción del submenu*/
        /*no es necesario comprobar sub_opt ni opt porque el propio ShowSubMenu y ShowMainMenu tienen comprobación de errores*/
        switch (opt){ /*en función de la opción del menu*/
            case 1: /*si es 1 llama a las consultas de product*/
                ConsultasProduct(&sub_opt, &dbc); /*con la opción del submenu*/
                break;
            case 2: /*si es 2 llama a las consultas de orders*/
                ConsultasOrders(&sub_opt, &dbc);
                break;
            case 3:
                ConsultasCustomers(&sub_opt, &dbc);
            default:
                break;
        }   
    }while(opt!=4); /*mientras que no quieras salir del programa pedirá instrucciones*/
    printf("Disconnecting...\n");
    /* DISCONNECT */
    if (!SQL_SUCCEEDED(odbc_disconnect(env, dbc))) {
        return EXIT_FAILURE;
    }    
    return 0;
}