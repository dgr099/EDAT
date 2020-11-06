#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultas.h"

/*PROTOTIPOS PRIVADAS*/
void ConsultasProduct_stock(SQLHDBC *dbc);
void ConsultasProduct_find(SQLHDBC *dbc);
void ConsultasOrders_open(SQLHDBC *dbc);
void ConsultasOrders_Range(SQLHDBC *dbc);
void ConsultasOrders_Details(SQLHDBC *dbc);
/*FIN PROTOTIPOS PRIVADAS*/

void ConsultasProduct(int *opt, SQLHDBC *dbc)
{
    switch (*opt)
    {
    case 1:
    { /*ha seleccionado que desea ver el stock*/
        ConsultasProduct_stock(dbc);
        break;
    }
    case 2:
    { /*ha seleccionado el find*/
        ConsultasProduct_find(dbc);
        break;
    }
    default:
    { /*Ha seleccionado exit*/
        break;
    }
    }
    return;
}

void ConsultasOrders(int *opt, SQLHDBC *dbc)
{
    switch (*opt)
    {
    case 1: /*Ha seleccionado Open*/
    {
        ConsultasOrders_open(dbc);
        break;
    }
    case 2: /*Ha seleccionado Range*/
    {
        ConsultasOrders_Range(dbc);
        break;
    }
    case 3:
    { /*Ha seleccionado Details*/
        ConsultasOrders_Details(dbc);
        break;
    }
    default:
        break;
    }
}

void ConsultasOrders_open(SQLHDBC *dbc)
{
    SQLHSTMT stmt;
    SQLINTEGER onumber;
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt); /*allocate a statement handle*/
    /*aquí no necesita leer nada, va a mostrar los que aun no se han enviado*/
    SQLPrepare(stmt, (SQLCHAR *)"SELECT ordernumber from orders where shippeddate is NULL group by ordernumber", SQL_NTS); /*prepara la consulta*/
    SQLExecute(stmt);                                                                                                      /*ejecutamos la consulta*/
    SQLBindCol(stmt, 1, SQL_C_LONG, &onumber, sizeof(onumber), NULL);                                                      /*solo hay una columna resultante de la consulta que es el code*/
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    {                            /*si encuentra la consulta*/
        printf("%d\n", onumber); /*imprime el valor de la consulta*/
    }
    SQLCloseCursor(stmt);                 /*cerramos el cursor*/
    SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
    puts(QRY);
    return;
}

void ConsultasOrders_Range(SQLHDBC *dbc)
{
    SQLHSTMT stmt;
    SQLINTEGER onumber;
    SQLCHAR odate[11];
    SQLCHAR oshipp[11];
    char buff[25];
    char *tok_1 = NULL;
    char *tok_2 = NULL;
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt); /*allocate a statement handle*/
    printf("Enter dates (YYYY-MM-DD - YYYT-MM-DD) > ");
    if(!fgets(buff, 25, stdin)){ /*lees el rango de fechas deseado*/
        /*si hay un error en la lectura*/
        puts(QRYERR);
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    if(!(tok_1 = strtok(buff, " "))){
        /*si hay un error con los toks*/
        puts(QRYERR);
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    strtok(NULL, " ");
    if(!(tok_2 = strtok(NULL, "\n"))){
        puts(QRYERR);
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    SQLPrepare(stmt, (SQLCHAR *)"SELECT ordernumber, orderdate, shippeddate FROM orders WHERE orderdate>= ? AND orderdate<= ? group by ordernumber", SQL_NTS);
    /*SQLPrepare(stmt, (SQLCHAR*) "SELECT ordernumber FROM orders WHERE orderdate>= '2003-01-10' AND orderdate<= '2003-04-21' group by ordernumber", SQL_NTS);*/

    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, tok_1, 0, NULL);
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, tok_2, 0, NULL);
    SQLExecute(stmt);

    SQLBindCol(stmt, 1, SQL_INTEGER, &onumber, sizeof(onumber), NULL);
    SQLBindCol(stmt, 2, SQL_C_CHAR, &odate, sizeof(odate), NULL);
    SQLBindCol(stmt, 3, SQL_C_CHAR, &oshipp, sizeof(odate), NULL);
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    { /*si encuentra la consulta*/
        printf("%d\t%s\t%s\n", onumber, odate, oshipp);
    }
    SQLCloseCursor(stmt);                 /*cerramos el cursor*/
    SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
    puts(QRY);
    return;
}

void ConsultasOrders_Details(SQLHDBC *dbc)
{
    SQLHSTMT stmt;
    char buff[25];
    SQLINTEGER onumber;
    SQLINTEGER qordered;
    SQLFLOAT peach;
    SQLCHAR pcode[10];
    SQLCHAR odate[11];
    SQLCHAR status[11];
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt);
    printf("Enter ordernumber > ");
    if(!(fgets(buff, 25, stdin))){
        /*si hay un error en la lectura*/
        puts(QRYERR);
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    onumber = atoi(buff); /*convertimos lo leido a integer*/
    SQLPrepare(stmt, (SQLCHAR *)"Select O.orderdate, O.status, P.productcode, OD.quantityordered, OD.priceeach FROM orders O, products P, orderdetails OD where OD.ordernumber=O.ordernumber and OD.productcode=P.productcode and O.ordernumber= ? order by OD.orderlinenumber", SQL_NTS);
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &onumber, 0, NULL);
    SQLExecute(stmt);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &odate, sizeof(odate), NULL);
    SQLBindCol(stmt, 2, SQL_C_CHAR, &status, sizeof(status), NULL);
    SQLBindCol(stmt, 3, SQL_C_CHAR, &pcode, sizeof(pcode), NULL);
    SQLBindCol(stmt, 4, SQL_C_LONG, &qordered, sizeof(pcode), NULL);
    SQLBindCol(stmt, 5, SQL_C_DOUBLE, &peach, sizeof(peach), NULL);
    /*SQLBindCol(stmt, 5, SQL_C_CHAR, &aux, sizeof(aux), NULL);*/
    /*printf("%s\t%s\n", odate, status);*/
    if (SQL_SUCCEEDED(SQLFetch(stmt)))
        printf("%s\t%s\n%s\t%d\t%.2f\n", odate, status, pcode, qordered, peach);
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    { /*si encuentra la consulta*/
        printf("%s\t%d\t%.2f\n", pcode, qordered, peach);
    }
    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    puts(QRY);
    return;
}

void ConsultasProduct_stock(SQLHDBC *dbc)
{
    SQLHSTMT stmt;
    char buff[16]; /*buffer donde guardar el input*/
    char *tok = NULL;
    SQLINTEGER stock;
    printf("Enter productcode > ");
    puts("\n");
    if(!(fgets(buff, 16, stdin))){ /*lee el productcode deseado*/
        /*si hay un error en la lectura*/
        puts(QRYERR);
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    if(!(tok = strtok(buff, "\n"))){
        /*si hay un error con los tok*/
        puts(QRYERR);
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt);                                                       /*allocate a statement handle*/
    SQLPrepare(stmt, (SQLCHAR *)"Select quantityinstock From products where productcode = ?", SQL_NTS); /*preparamos la consulta*/
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, tok, 0, NULL);            /*le introducimos el parametro*/
    SQLExecute(stmt);
    SQLBindCol(stmt, 1, SQL_C_SLONG, &stock, sizeof(SQLINTEGER), NULL);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    {                                                                 /*si encuentra la consulta*/
        printf("%d -> stock for product with id = %s\n", stock, tok); /*imprime el valor de la consulta*/
    }

    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos el statement*/
    puts(QRY);
    return;
}

void ConsultasProduct_find(SQLHDBC *dbc)
{
    SQLHSTMT stmt;
    SQLCHAR name[70];
    SQLCHAR code[15];
    char buff[16];
    char *tok = NULL;
    char *aux = NULL;
    printf("Enter productname > ");
    if(!(fgets(buff, 16, stdin))){ /*lee el productcode deseado*/
        /*si hay un error en la lectura*/
        puts(QRYERR);
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    if(!(tok = strtok(buff, "\n"))){
        /*si hay un error con los tok*/
        puts(QRYERR);
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    if (!(aux = (char *)calloc(strlen(tok) + 2, sizeof(char))))
    { /*el auxiliar nos servirá para poder poner %text% como parámetro*/
        printf("Error, memoria insuficiente para la consulta\n");
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    strcat(aux, "%%");
    strcat(aux, tok);
    strcat(aux, "%%");
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt); /*allocate a statement handle*/
    SQLPrepare(stmt, (SQLCHAR *)"SELECT productcode, productname FROM products WHERE productname LIKE ? group by productcode", SQL_NTS);

    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, aux, 0, NULL);
    SQLExecute(stmt);
    /*SQLExecDirect(stmt, (SQLCHAR*) "SELECT productcode, productname FROM products WHERE productname LIKE '%%Harley%%'", SQL_NTS);*/
    SQLBindCol(stmt, 1, SQL_C_CHAR, &code, sizeof(code), NULL); /*la primera columna es el code*/
    SQLBindCol(stmt, 2, SQL_C_CHAR, &name, sizeof(name), NULL); /*la segunda columna es el name*/

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    {                                   /*si encuentra la consulta*/
        printf("%s\t%s\n", code, name); /*imprime el valor de la consulta*/
    }
    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos el statement*/
    free(aux);                            /*liberamos la memoria del auxiliar*/
    puts(QRY);
    return;
}
