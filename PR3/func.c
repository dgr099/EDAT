#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "func.h"

bool createTable ( const char * tab
Name){
    FILE *f=NULL;
    char *aux=NULL;
    int tam=0;
    int i=-1; /*en i guardamos el offset del elemento borrado (-1) porque es una tabla sin datos y sin nada borrado*/
    if(!tableName){ /*comprobacion de parametros*/
        return false;
    }
    if((f=fopen(tableName, "rb"))){ /*si el fichero ya existe no hace nada*/
            fclose(f);
            return true;
        }
    if(!(f=fopen(tableName, "wb"))){ /*si no existe lo crea*/
            return false;
        }
        /*y pones el -1 en la cabecera*/
    if(!(fwrite(&i, sizeof(int), 1, f))){ /*si da error la escritura*/
        fclose(f);/*cerramos el fichero*/
        return false;
    }
    tam=strlen(tableName);
    aux=(char*)calloc(tam, sizeof(char));
    memcpy(aux, tableName, tam-3);
    strcat(aux, "idx");
    createIndex(aux);     
    fclose(f); /*cerramos el fichero*/
    return true;
}

bool createIndex ( const char * indexName){
    FILE *f=NULL;
    int i=-1; /*offset -1 por si no existe el index con dicho name*/
    if(!indexName){
        return false;
    }
    if((f=fopen(indexName, "rb"))){ /*comprueba si existe el fichero*/
        fclose(f);
        return true;
    }
    if(!(f=fopen(indexName, "wb"))){
        fclose(f);
        return false;
    }
    /*escribimos el -1 del puntero al nodo padre*/
    if(!(fwrite(&i, sizeof(int), 1, f))){ /*si da error la escritura*/
        fclose(f);/*cerramos el fichero*/
        return false;
    }
    /*escribimos -1 del puntero al primer nodo borrado*/
    if(!(fwrite(&i, sizeof(int), 1, f))){ /*si da error la escritura*/
        fclose(f);/*cerramos el fichero*/
        return false;
    }
    fclose(f);
    return true;
}

bool findKey ( const char * book_id, const char * indexName, int * nodeIDOrDataOffset){
    FILE *f=NULL;
    int pos=0;
    char key[PK_SIZE]={'0'};
    (*nodeIDOrDataOffset)=-1;
    if(!book_id || !indexName){
        return false;
    }
    if(!(f=fopen(indexName, "rd"))){ /*si no existe la tabla de índices*/
        return false;
    }
    
    fread(&pos, sizeof(int), 1, f); /*leemos el offset de la raiz*/
    /*pos*tamaño_nodo mas los 8 de los dos enteros del inicio*/
    fseek(f, pos*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET); /*posicionas el puntero en la raiz*/
    while(pos!=-1){ /*mientras pueda seguir profundizando*/
        fread(key, sizeof(char), 4, f);/*lees la clave*/
        if(!(strcmp(key, book_id))){
            fseek(f, 3*sizeof(int), SEEK_CUR);
            fread(&pos, sizeof(int), 1, f); /*reutilizamos pos para guardar el offset del fichero de datos*/
            (*nodeIDOrDataOffset)=pos;
            return true;
        }
        if(key>book_id){ /*si es mayor miramos el hijo derecho*/
            fseek(f, 1, SEEK_CUR); /*saltamos el hijo izq*/
        }
        fread(&pos, sizeof(int), 1, f); /*leemos el hijo que toque*/
        fseek(f, pos*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET); /*posicionas el puntero en el hijo izquierdo*/
    }
    /*si has llegado hasta aquí y no lo has encontrado es que no esta*/
    /*retornar id??*/
    return false;
}


bool addTableEntry (Book * book, const char * tableName, const char * indexName){
    FILE *f=NULL;
    int node;
    int pos=-1;
    if(!book || !tableName || !indexName){
        return false;
    }
    if(findKey(book->bookid, indexName, &node)==true){
        printf("Error, la clave introducida ya está en la base de datos\n"); /*mensaje de error*/
        return false;
    }
    if(!(f=fopen(tableName, "rb"))){
        return false; /*si hay errores en la apertura*/
    }
    /*miramos si hay registro borrados*/
    fread(&pos, sizeof(int), 1, f);
    if(pos=-1){ /*si no hay nodos borrados*/
        fclose(f);
        if(!(f=fopen(tableName, "ab"))){
            return false; /*si hay errores en la apertura*/
        }
        fwrite(book->bookid, sizeof(char), 4, f); /*pones su id*/
        fwrite(book->titlelen, sizeof(int), 1, f); /*el tamaño del titulo*/
        fwrite(book->title, sizeof(char), book->titlelen, f); /*el titulo*/
        /*cerramos el fichero tras la insercción*/
        fclose(f);
    }
    /*offset*/
    /*actualizar index*/
    return true;
}
