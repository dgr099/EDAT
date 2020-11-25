#include <stdio.h>
#include <stdbool.h>

#define NO_DELETED_REGISTERS −1
#define INDEX_HEADER_SIZE 8
#define DATA_HEADER_SIZE 4
/* primary key size */
#define PK_SIZE 4
/* Our table is going to contain a string ( title ) and
an alph anume ric primary key ( bookid )
*/
typedef struct _book {
char bookid [PK_SIZE] ; /* primary key */
size_t titlelen; /* titlelength */
char* title ; /* string to be saved in the database */
}Book;
/* Note that in general a struct cannot be used to
handle tables in databases since the table structure
is unknown at compilation time .
*/
typedef struct _node {
char bookid [PK_SIZE];
int left, right, parent, offset; /*hijo derecho, izquierdo, padre y offset de la información*/
}Node;

/* Function prototypes .
see function descriptions in the follow ingsections
All function return true if success or false if failed
except findKey which return true if register found
and false otherwise .
*/
bool createTable ( const char * tableName);
bool createIndex ( const char * indexName);
bool findKey ( const char * book_id, const char * indexName, int * nodeIDOrDataOffset) ;
bool addTableEntry (Book * book, const char * tableName, const char * indexName);
bool addIndexEntry ( char * bookid , int bookOffset, const char * indexName ) ;
void printTree(size_t level , const char * indexName) ;