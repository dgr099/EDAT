#include <stdio.h>
#include <string.h>
#include "func.h"
int main(){
    FILE *f=NULL;
    int i=-1;
    int pr;
    char pkey[PK_SIZE+1];
    f=fopen("try.idx", "wb");
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
    strcpy(pkey, "BAR1");
    findKey(pkey, "try.idx", &pr);
    /*createTable("prueba.dat");*/
    return 0;
}