#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/stat.h>
#include "utils.h"

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
    aux=(char*)calloc(tam+1, sizeof(char));
    memcpy(aux, tableName, tam-3);
    strcat(aux, "idx");
    createIndex(aux);  /*creamos el indice*/ 
    free(aux);  
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
    /*si no existe lo creas*/
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
    fread(&nodo.book_id, sizeof(char), 4, f);
    fread(&nodo.left, sizeof(nodo.left), 1, f);
    fread(&nodo.right, sizeof(nodo.right), 1, f);
    fread(&nodo.parent, sizeof(nodo.parent), 1, f);
    fread(&nodo.offset, sizeof(nodo.offset), 1, f);
    /*imprimimos el nodo*/
    for(aux_1=0; (*aux)!=aux_1; aux_1++){
        printf( "\t"); /*le ponemos tabuladores*/
    }
    if((*lado)==1) 
        printf("r "); /*primero indicamos si es hijo izq o derecho*/
    if((*lado)==-1)  
        printf("l ");

    for(aux_1=0; aux_1!=4; aux_1++)
        printf("%c", nodo.book_id[aux_1]);
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
    short aux=0;
    char key[PK_SIZE]={'0'};
    (*nodeIDOrDataOffset)=-1;
    if(!book_id || !indexName){
        return false;
    }
    if(!(f=fopen(indexName, "rb"))){ /*si no existe la tabla de índices*/
        return false;
    }
    
    fread(&pos, sizeof(int), 1, f); /*leemos el offset de la raiz*/
    /*pos*tamaño_nodo mas los 8 de los dos enteros del inicio*/
    fseek(f, pos*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET); /*posicionas el puntero en la raiz*/
    while(pos!=-1){ /*mientras pueda seguir profundizando*/
        pos_aux=pos;
        fread(key, sizeof(char), 4, f);/*lees la clave*/
        if((aux=strncmp(book_id, key, PK_SIZE))==0){ /*si hemos encontrado el nodo en cuestión*/
            fseek(f, 3*sizeof(int), SEEK_CUR);/*saltamos  leer el offset omitiendo padre e hijos*/
            fread(&pos, sizeof(int), 1, f); /*reutilizamos pos para guardar el offset del fichero de datos*/
            (*nodeIDOrDataOffset)=pos; /*guardamos en nodeIDorDataOffset el offset del registro de datos*/
            return true; /*devolvemos true al haberlo encontrado*/
        }
        if(aux > 0){ /*si es mayor miramos el hijo derecho*/
            fseek(f, 1*sizeof(int), SEEK_CUR); /*saltamos el hijo izq*/
        }
        fread(&pos, sizeof(int), 1, f); /*leemos el hijo que toque*/
        fseek(f, pos*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET); /*posicionas el puntero en el hijo que toque*/
    }
    /*si has llegado hasta aquí y no lo has encontrado es que no esta*/
    (*nodeIDOrDataOffset)=pos_aux; /*guardamos el id del último nodo visitado con éxito*/
    fclose(f);
    return false;
}

bool addTableEntry (Book * book, const char * tableName, const char * indexName){
    FILE *f=NULL;
    int node=0;
    int pos=-1;
    int offset=0;
    struct stat st;
    if(!book || !tableName || !indexName){
        return false;
    }
    if(findKey(book->book_id, indexName, &node)==true){ /*si la clave ya está dentro*/
        printf("Error, la clave introducida ya está en la base de datos\n"); /*mensaje de error*/
        return false;
    }
    /*caso contrario la clave no está dentro*/
    if(!(f=fopen(tableName, "rb+"))){ /*comprobamos sí existe esa tabla de datos*/
        return false; /*si hay errores en la apertura*/
    }
    /*miramos si hay registro borrados*/
    fread(&pos, sizeof(int), 1, f);
    /*si no hay registros borrados*/
    if(pos!=NO_DELETED_REGISTERS){ /*si hay nodos borrados*/
        fseek(f, pos + 4*sizeof(char), SEEK_SET); /*nos movemos para leer el tamaño*/
        fread(&offset, sizeof(int), 1, f); /*lo leemos*/
        if(offset>=(int)book->title_len){ /*si entra en el registro*/
            fseek(f, 0, SEEK_SET); /*nos movemos al inicio para modificar la cadena de reg borrados*/
            offset=-1; /*utilizo el offset como auziliar*/
            fwrite(&offset, sizeof(int), 1, f); /*ponemos a -1 los registros borrados*/
            offset=pos; /*el offset es el offset del reg a reescribir*/
            fseek(f, offset, SEEK_SET); /*te vas al offset para sobreescribir*/  
        }
        else{ /*si no cabe escribimos al final*/
            goto NO_REG_V;
        }
    }
    /*si no hay registros borrados o no cabe*/
    else{
        NO_REG_V:/*no hay registro borrao o no es valido porque no cabe*/
        stat(tableName, &st); offset=st.st_size; /*guardas el tamaño del fichero antes de introducir los nuevos datos, para conocer el offset*/
        fseek(f, 0, SEEK_END); /*nos colocamos para escribir al final*/
    }
    /*guardas los datos en la posición que toque*/
    fwrite(&book->book_id, sizeof(char), 4, f); /*pones su identificador*/
    fwrite(&book->title_len, sizeof(int), 1, f); /*el tamaño del titulo que equivalen a los bytes que ocupa ya que cada char es equivalente a un byte*/
    fwrite(book->title, sizeof(char), book->title_len, f); /*finalmente esqcribes el titulo*/
    /*actualizas el valor del índice*/
    addIndexEntry(book->book_id, offset, indexName);
    fclose(f);
    return true;
}

bool addIndexEntry(char * book_id, int bookOffset, const char * indexName){
    struct stat st;
    FILE *f=NULL;
    int node=-1;
    int aux;
    int nnr; /*numero de registro relativo*/
    char pkey[4];
    if(!book_id || bookOffset<4 || !indexName){
        return false;
    }
    if(findKey(book_id, indexName, &node)==true)
        return false;
    
    if((f=fopen(indexName, "rb+"))==NULL){ /*comprobamos que exista el fichero y lo abrimos*/
        return false;
    }
    stat(indexName, &st); nnr=(st.st_size-INDEX_HEADER_SIZE)/sizeof(Node); /*como los registros son fijos, para saber el numero de 
                                                        registro basta con tomar el tamaño total, restarle el 
                                                        tamaño del headder y dibidirlo por el tamaño
                                                        de un registro*/
    /****************************/
    /*comprobar registro borrado*/
    /****************************/
    fseek(f, sizeof(int), SEEK_SET); /*saltamos la raiz*/
    fread(&aux, sizeof(int), 1, f); /*leemos si hay registros borrados*/
    if(aux==-1){
        fseek(f, 0, SEEK_END);/*si no hay registros borrados escribes al final*/
    }
    else{
        /*te colocas para leer el hijo izq*/
        fseek(f, aux*sizeof(Node)+INDEX_HEADER_SIZE+4*sizeof(char), SEEK_SET);
        fread(&aux, sizeof(int), 1, f); /*lees el hijo izq*/
        fseek(f, sizeof(int), SEEK_SET); /*vuelves al inicio para poner el nuevo nodo borrado*/
        fwrite(&aux, sizeof(int), 1, f);
        fseek(f, aux*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET); /*si hay registros borrados sobre escribimos el borrado*/
        nnr=aux; /*el numero de registro relativo ya no es el final*/
    }
    /*escribes la info donde toque despues de posicionarte antes*/
    fwrite(book_id, sizeof(char), 4, f); /*guardas la id*/
    aux=-1;   fwrite(&aux, sizeof(int), 1, f); /*hijo izquierdo, debe ser una hoja del árbol*/
    aux=-1;   fwrite(&aux, sizeof(int), 1, f); /*hijo derecho, debe ser una hoja del árbol*/
    fwrite(&node, sizeof(int), 1, f); /*padre (último nodo visitado en find)*/
    fwrite(&bookOffset, sizeof(int), 1, f); /*guardamos su offset*/
    if(node<0) /*si es la raiz*/{
        fseek(f, 0, SEEK_SET); /*escribimos al inicio*/
        fwrite(&nnr, sizeof(int), 1, f);
        fclose(f);
        return true;
    }
    fseek(f, node*sizeof(Node)+INDEX_HEADER_SIZE, SEEK_SET); /*vamos al padre a actualizar la info de sus hijos*/
    fread(pkey, sizeof(char), 4, f);
    if(strncmp(book_id, pkey, PK_SIZE)<0){ /*si la clave es mayor que el padre, hijo izq*/
        fwrite(&nnr, sizeof(int), 1, f); /*escribes el hijo izq*/
        fclose(f);
        return true;
    }
    fseek(f, sizeof(int), SEEK_CUR); /*nos saltamos el hijo izq*/
    fwrite(&nnr, sizeof(int), 1, f); /*escribes el hijo derecho*/
    fclose(f);
    return true;

}