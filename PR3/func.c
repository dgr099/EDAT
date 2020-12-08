#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/stat.h>
#include "func.h"

bool createTable ( const char * tableName){
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
    /*reservamos memoria para el nombre */
    aux=(char*)calloc(tam, sizeof(char));
    memcpy(aux, tableName, tam-3);
    strcat(aux, "idx");
    createIndex(aux);  /*creamos el indice*/   
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

void printTree_rec(size_t *level, FILE *f, size_t *aux, int *lado, int *id){
    Node nodo;
    size_t aux_1;
    if(!f || !level || !aux || (*id)==-1){
        (*aux)--;
        return;
    }
    /*leemos el nodo*/
    fread(&nodo.bookid, sizeof(char), 4, f);
    fread(&nodo.left, sizeof(nodo.left), 1, f);
    fread(&nodo.right, sizeof(nodo.right), 1, f);
    fread(&nodo.parent, sizeof(nodo.parent), 1, f);
    fread(&nodo.offset, sizeof(nodo.offset), 1, f);
    /*imprimimos el nodo*/
    for(aux_1=0; (*aux)!=aux_1; aux_1++){
        printf( "\t"); /*le ponemos tabuladores*/
    }
    if((*lado)==1)   /*fprintf(stdin, "r ");*/
        printf("r "); /*primero indicamos si es hijo izq o derecho*/
    if((*lado)==-1)   /*fprintf(stdin, "l ");*/
        printf("l ");
    /*imprimimos clave, id y su offset*/
    /*fprintf(stdin, "%s ", nodo.bookid);
    fprintf(stdin, "(%d): ", (*id));
    fprintf(stdin, "%d\n", nodo.offset);*/
    for(aux_1=0; aux_1!=4; aux_1++)
        printf("%c", nodo.bookid[aux_1]);
    printf(" (%d): ", (*id));
    printf("%d\n", nodo.offset);
    if((*aux) == (*level)){ /*si hemos llegado a la profundidad máxima dejamos de profundizar*/
        ((*aux)--); /*bajamos uno al level parcial*/
        return;
    }
    /*si aun no hemos llegado al level*/
    (*aux)++; /*sumamos uno al level parcial porque vamos a profundizar*/
    /*vamos a por el hijo izq*/
    fseek(f, nodo.left*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET);
    (*lado)=-1;
    printTree_rec(level, f, aux, lado, &nodo.left);


    /*vamos a por el hijo der*/
    (*aux)++;
    fseek(f, nodo.right*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET);
    (*lado)=1;
    printTree_rec(level, f, aux, lado, &nodo.right);
    (*aux)--;

}

void printTree(size_t level , const char * indexName){
    FILE *f=NULL;
    int pos=-1;
    int lado=0;
    size_t aux=0; /*servirá para comprobar la condición de parada*/
    if(!indexName){
        return;
    }
    if(!(f=fopen(indexName, "rb"))) /*si falla la apertura del fichero*/
        return;
    /*imprime en profundidad, función recursiva*/
    /*primero saltamos a la raíz*/
    fread(&pos, sizeof(int), 1, f);
    if(pos==-1){ /*si no tienen ningún nodo salimos directamente*/
        return;
    }
    fseek(f, pos*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET); /*hacemos seek a la raíz*/
    /*reutilizamos post para guardar el valor del id del nodo*/
    printTree_rec(&level, f, &aux, &lado, &pos);
    fclose(f);
    return;
}

bool findKey ( const char * book_id, const char * indexName, int * nodeIDOrDataOffset){
    FILE *f=NULL;
    int pos=-1, pos_aux=-1; /*en pos_aux guardamos la ultima posicion válida*/
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
        pos_aux=pos;
        fread(key, sizeof(char), 4, f);/*lees la clave*/
        if(!(strcmp(key, book_id))){ /*si hemos encontrado el nodo en cuestión*/
            fseek(f, 3*sizeof(int), SEEK_CUR);/*saltamos  leer el offset omitiendo padre e hijos*/
            fread(&pos, sizeof(int), 1, f); /*reutilizamos pos para guardar el offset del fichero de datos*/
            (*nodeIDOrDataOffset)=pos; /*guardamos en nodeIDorDataOffset el offset del registro de datos*/
            return true; /*devolvemos true al haberlo encontrado*/
        }
        if(key>book_id){ /*si es mayor miramos el hijo derecho*/
            fseek(f, 1, SEEK_CUR); /*saltamos el hijo izq*/
        }
        fread(&pos, sizeof(int), 1, f); /*leemos el hijo que toque*/
        fseek(f, pos*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET); /*posicionas el puntero en el hijo que toque*/
    }
    /*si has llegado hasta aquí y no lo has encontrado es que no esta*/
    (*nodeIDOrDataOffset)=pos_aux; /*guardamos el id del último nodo visitado con éxito*/
    return false;
}



/*duda offset*/
bool addTableEntry (Book * book, const char * tableName, const char * indexName){
    FILE *f=NULL;
    int node=0;
    int pos=-1;
    int offset=0;
    struct stat st;
    if(!book || !tableName || !indexName){
        return false;
    }
    if(findKey(book->bookid, indexName, &node)==true){ /*si la clave ya está dentro*/
        printf("Error, la clave introducida ya está en la base de datos\n"); /*mensaje de error*/
        return false;
    }
    /*caso contrario la clave no está dentro*/
    if(!(f=fopen(tableName, "rb"))){ /*comprobamos sí existe esa tabla de datos*/
        return false; /*si hay errores en la apertura*/
    }
    /*miramos si hay registro borrados*/
    fread(&pos, sizeof(int), 1, f);
    if(pos==-1){ /*si no hay nodos borrados*/
        fclose(f); /*cerramos para abrir en modo escritura*/
        stat(tableName, &st); offset=st.st_size; /*guardas el tamaño del fichero antes de introducir los nuevos datos*/
        /*que será el offset*/
        if(!(f=fopen(tableName, "ab"))){
            return false; /*si hay errores en la apertura en modo escritura*/
        }
        /*guardas dentro del fichero de datos la info del nuevo nodo*/
        fwrite(&book->bookid, sizeof(char), 4, f); /*pones su identificador*/
        fwrite(&book->titlelen, sizeof(int), 1, f); /*el tamaño del titulo que equivalen a los bytes que ocupa ya que cada char es equivalente a un byte*/
        fwrite(book->title, sizeof(char), book->titlelen, f); /*finalmente esqcribes el titulo*/
        /*cerramos el fichero tras la insercción*/
        fclose(f);
        /*aquí es necesario actualizar el índice*/
    }
    /*falta actualizar indíce*/
    /*actualizar index*/
    return true;
}
