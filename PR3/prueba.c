#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "func.h"
int main(){
    FILE *f=NULL;
    int i=-1;
    char pkey[PK_SIZE+1];
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
    /*findKey(pkey, "try.idx", &pr);*/
    /*createTable("prueba.dat");*/
    return 0;
}




int main_1(){
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
