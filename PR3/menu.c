#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "utils.h"
/*funciones privadas*/
int submenu_use(char *nombre_tabla, short* use);
int submenu_insert(char *nombre_tabla, short* use);
int submenu_print(char *nombre_tabla, short* use);
/*funciones privadas*/

int showmenu(char *table_name, short *use){
    int opt=-1;
    printf("Escoga entre los distintos menu:\n");
    printf("1.  Use\n");
    printf("2.  Insertar\n");
    printf("3.  Print\n");
    printf("4.  Exit\n");
    printf("Elecion: ");
    fscanf(stdin, "%i", &opt); /*switch de las opciones*/
    switch (opt)
    {
    case 1:
        submenu_use(table_name, use);/*llama a la rutina use*/
        break;
    case 2:
        submenu_insert(table_name, use);
        break;
    case 3:
        submenu_print(table_name, use);
        break;
    case 4:
        printf("Saliendo...\n");
        return 4;
    default:
        printf("Error, no se ha entendido la petición\n");
        break;
    }
    return 0;
}

int submenu_use(char *nombre_tabla, short*use){
    printf("Introduzca el nombre de la tabla: ");
    fscanf(stdin, "%s", nombre_tabla);
    if(createTable(nombre_tabla)==false){
        printf("\nError en la apertura de la tabla\n");
        return 2;
    }
    *use=1;
    return 0; 
}

int submenu_insert(char *nombre_tabla, short* use){
    Book b;
    char *indice;
    int tam=0;
    if((*use)==0){
        printf("No hay ninguna tabla seleccionada\n");
        return 1;
    }
    tam=strlen(nombre_tabla);
    /*reservamos memoria para el nombre */
    indice=(char*)calloc(tam+1, sizeof(char));
    memcpy(indice, nombre_tabla, tam-3);
    strcat(indice, "idx");

    printf("Introduzca el valor de la clave a introducir: ");
    fscanf(stdin, " %s", b.book_id);
    b.title=(char*)calloc(MAX_NOMBRE, sizeof(char));
    printf("Introduzca el título a almacenar: ");
    fscanf(stdin, " %[^\n]s", b.title);
    /*fgets(&b.title[0], MAX_NOMBRE, stdin);*/
    b.title_len=strlen(b.title);
    addTableEntry(&b, nombre_tabla, indice);
    free(indice);
    free(b.title);
    return 0;
}

int submenu_print(char *nombre_tabla, short* use){
    char *indice=NULL;
    int tam;
    if(*use==0){
        printf("No hay ninguna tabla seleccionada\n");
        return 1;
    }
    tam=strlen(nombre_tabla);
    /*reservamos memoria para el nombre */
    indice=(char*)calloc(tam+1, sizeof(char));
    memcpy(indice, nombre_tabla, tam-3);
    strcat(indice, "idx");
    /*Qué level mando??*/
    printTree(-1, indice);
    free(indice);
    return 0;
}