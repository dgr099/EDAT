#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils.h"
int main(){
    FILE *f=NULL;
    int i=-1;
    char pkey[PK_SIZE+1];
    Book book;
    f=fopen("try.idx", "wb");
    /*para probar los índices como vienen en el pdf*/
    i=2;    fwrite(&i, sizeof(int), 1, f);
    i=0;   fwrite(&i, sizeof(int), 1, f);
    /****/
    strcpy(pkey, "BAR1");       fwrite(pkey, sizeof(char), 4, f);
    i=10;   fwrite(&i, sizeof(int), 1, f); /*siguiente borrado es una prueba da igual*/
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=2;   fwrite(&i, sizeof(int), 1, f);
    i=4;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "GAR1");       fwrite(pkey, sizeof(char), 4, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=2;   fwrite(&i, sizeof(int), 1, f);
    i=34;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "BAR2");       fwrite(pkey, sizeof(char), 4, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=1;   fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=63;   fwrite(&i, sizeof(int), 1, f);
    /****/
    fclose(f);
    printTree(4, "try.idx");
    strcpy(pkey, "BAR1");
    /*findKey(pkey, "try.idx", &pr);*/
    strcpy(book.book_id, "ABC1");
    book.title=(char*)calloc(4, sizeof(char));
    strcpy(book.title, "Hola");
    book.title_len=4;
    addIndexEntry(book.book_id, 10, "try.idx");
    printTree(4, "try.idx");
    free(book.title);
    /*createTable("prueba.dat");*/
    return 0;
}

int main_1(){
    FILE *f=NULL;
    Book prueba;
    int i=4;
    createTable("try.dat");
    f=fopen("try.dat", "wb");
    /*offset del borrado*/
    fwrite(&i, sizeof(int), 1, f);
    /*nodo borrado*/
    strcpy(prueba.book_id, "BAR1");
    fwrite(prueba.book_id, sizeof(char), 4, f);
    i=22;
    fwrite(&i, sizeof(int), 1, f);
    prueba.title=(char*)calloc(22, sizeof(char));
    strcpy(prueba.title, "Zalacain el aventurero");
    fwrite(prueba.title, sizeof(char), 22, f);
    fclose(f);
    /*llamas a la funcion de insertar para ver si aprovecha la memoria*/
    free(prueba.title);
    strcpy(prueba.book_id, "GAR1");
    prueba.title=(char*)calloc(23, sizeof(char));
    strcpy(prueba.title, "Poema del cante jondo-_");
    prueba.title_len=23;
    addTableEntry(&prueba, "try.dat", "try.idx");
    free(prueba.title);
    return 0;
}
int main_2(){
    FILE *f=NULL;
    int i=-1;
    char pkey[PK_SIZE+1];
    createTable("try.dat");
    f=fopen("try.idx", "wb");
    /*para probar los índices como vienen en el pdf*/
    i=0;    fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    /****/
    strcpy(pkey, "MAR2");       fwrite(pkey, sizeof(char), 4, f);
    i=1;   fwrite(&i, sizeof(int), 1, f);
    i=2;   fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=4;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "MAR0");       fwrite(pkey, sizeof(char), 4, f);
    i=3;   fwrite(&i, sizeof(int), 1, f);
    i=4;   fwrite(&i, sizeof(int), 1, f);
    i=0;   fwrite(&i, sizeof(int), 1, f);
    i=37;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "VAR1");       fwrite(pkey, sizeof(char), 4, f);
    i=5;   fwrite(&i, sizeof(int), 1, f);
    i=6;   fwrite(&i, sizeof(int), 1, f);
    i=0;   fwrite(&i, sizeof(int), 1, f);
    i=53;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "BAR0");       fwrite(pkey, sizeof(char), 4, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=7;   fwrite(&i, sizeof(int), 1, f);
    i=1;   fwrite(&i, sizeof(int), 1, f);
    i=88;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "MAR1");       fwrite(pkey, sizeof(char), 4, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=1;   fwrite(&i, sizeof(int), 1, f);
    i=105;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "PAR2");       fwrite(pkey, sizeof(char), 4, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=8;   fwrite(&i, sizeof(int), 1, f);
    i=2;   fwrite(&i, sizeof(int), 1, f);
    i=121;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "WAR3");       fwrite(pkey, sizeof(char), 4, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=2;   fwrite(&i, sizeof(int), 1, f);
    i=21;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "CAR5");       fwrite(pkey, sizeof(char), 4, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=3;   fwrite(&i, sizeof(int), 1, f);
    i=157;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "RAR2");       fwrite(pkey, sizeof(char), 4, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=5;   fwrite(&i, sizeof(int), 1, f);
    i=174;   fwrite(&i, sizeof(int), 1, f);
    /****/

    fclose(f);
    printTree(4, "try.idx");
    strcpy(pkey, "BAR1");
    i=450;
    addIndexEntry(pkey, i, "try.idx");
    printf("\n\n");
    printTree(5, "try.idx");
    /*createTable("prueba.dat");*/
    return 0;
}




int main_3(){
    FILE *f=NULL;
    int i=-1;
    char pkey[PK_SIZE+1];
    f=fopen("try.idx", "wb");
    /*para probar los índices como vienen en el pdf*/
    i=2;    fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    /****/
    strcpy(pkey, "BAR1");       fwrite(pkey, sizeof(char), 4, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=2;   fwrite(&i, sizeof(int), 1, f);
    i=4;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "GAR1");       fwrite(pkey, sizeof(char), 4, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=2;   fwrite(&i, sizeof(int), 1, f);
    i=34;   fwrite(&i, sizeof(int), 1, f);
    /****/

    /****/
    strcpy(pkey, "BAR2");       fwrite(pkey, sizeof(char), 4, f);
    i=0;   fwrite(&i, sizeof(int), 1, f);
    i=1;   fwrite(&i, sizeof(int), 1, f);
    i=-1;   fwrite(&i, sizeof(int), 1, f);
    i=63;   fwrite(&i, sizeof(int), 1, f);
    /****/
    fclose(f);
    printTree(4, "try.idx");
    strcpy(pkey, "BAR1");
    /*findKey(pkey, "try.idx", &pr);*/
    /*createTable("prueba.dat");*/
    return 0;
}
