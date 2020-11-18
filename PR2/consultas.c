#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultas.h"
#define ROWSET_SIZE 1
/*PROTOTIPOS PRIVADAS*/
void ConsultasProduct_Stock(SQLHDBC *dbc);
void ConsultasProduct_Find(SQLHDBC *dbc);
void ConsultasOrders_Open(SQLHDBC *dbc);
void ConsultasOrders_Range(SQLHDBC *dbc);
void ConsultasOrders_Details(SQLHDBC *dbc);
void ConsultasCustomers_Find(SQLHDBC *dbc);
void ConsultasCustomers_Balance(SQLHDBC *dbc);
void ConsultasCustomers_ListProducts(SQLHDBC *dbc);
int Consultas_10(SQLHSTMT *stmt, int *pag, int aux, int *pag_aux);
void Cursor_config(SQLHSTMT *stmt);
/*FIN PROTOTIPOS PRIVADAS*/

void ConsultasProduct(int *opt, SQLHDBC *dbc)
{
    switch (*opt)
    {
    case 1:
    { /*ha seleccionado que desea ver el stock*/
        ConsultasProduct_Stock(dbc);
        break;
    }
    case 2:
    { /*ha seleccionado el find*/
        ConsultasProduct_Find(dbc);
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
        ConsultasOrders_Open(dbc);
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

void ConsultasCustomers(int *opt, SQLHDBC *dbc)
{
    switch (*opt)
    {
    case 1: /*Ha seleccionado find*/
    {
        ConsultasCustomers_Find(dbc);
        break;
    }
    case 2: /*Ha seleccionado lista de productos*/
    {
        ConsultasCustomers_ListProducts(dbc);
        break;
    }
    case 3:
    { /*Ha seleccionado balance*/
        ConsultasCustomers_Balance(dbc);
        break;
    }
    default:
        break;
    }
}


void ConsultasOrders_Open(SQLHDBC *dbc)
{
    SQLHSTMT stmt=NULL;
    SQLINTEGER onumber=0;
    SQLLEN size=0;

    if(dbc==NULL) return;
    if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt)))
      return; /*allocate a statement handle*/
    /*aquí no necesita leer nada, va a mostrar los que aun no se han enviado*/
    Cursor_config(stmt);
    if(!SQL_SUCCEEDED(SQLPrepare(stmt, (SQLCHAR *)"SELECT ordernumber from orders where shippeddate is NULL group by ordernumber", SQL_NTS))) { /*prepara la consulta*/
        while(!SQL_SUCCEEDED((SQLFreeHandle(SQL_HANDLE_STMT, stmt))));
        return;
    }
    if(!SQL_SUCCEEDED((SQLExecute(stmt)))){
      while(!SQL_SUCCEEDED((SQLFreeHandle(SQL_HANDLE_STMT, stmt))));
      return;
    }
    size=(SQLLEN)sizeof(onumber);
    if(!SQL_SUCCEEDED(SQLBindCol(stmt, 1, SQL_C_LONG, &onumber, size, NULL))){
      while(!SQL_SUCCEEDED((SQLFreeHandle(SQL_HANDLE_STMT, stmt))));
      return;
    }                                                                      /*ejecutamos la consulta*/
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    {                            /*si encuentra la consulta*/
        printf("%d\n", onumber); /*imprime el valor de la consulta*/
    }
    while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                 /*cerramos el cursor*/
    while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
    if(EOF==puts(QRY)) return;
    return;
}

void ConsultasOrders_Range(SQLHDBC *dbc)
{
    SQLHSTMT stmt=NULL;
    SQLINTEGER onumber=0;
    SQLCHAR odate[11];
    SQLCHAR oshipp[11];
    SQLLEN size;          /*variable auxiliar para los tamanos de las consultas*/
    char buff[25];
    char *tok_1 = NULL;
    char *tok_2 = NULL;
    if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt)))
      return; /*allocate a statement handle*/
    if(printf("Enter dates (YYYY-MM-DD - YYYT-MM-DD) > ")==0){
      while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt)));
      return;
    }
    if(!fgets(buff, 25, stdin)){ /*lees el rango de fechas deseado*/
        /*si hay un error en la lectura*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                 /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    if(!(tok_1 = strtok(buff, " "))){
        /*si hay un error con los toks*/
        if(EOF==puts(QRYERR)) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    if(strtok(NULL, " ")==NULL) return;
    if(!(tok_2 = strtok(NULL, "\n"))){
        /*si hay un con los toks*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    while(!SQL_SUCCEEDED(SQLPrepare(stmt, (SQLCHAR *)"SELECT ordernumber, orderdate, shippeddate FROM orders WHERE orderdate>= ? AND orderdate<= ? group by ordernumber", SQL_NTS)));
    /*SQLPrepare(stmt, (SQLCHAR*) "SELECT ordernumber FROM orders WHERE orderdate>= '2003-01-10' AND orderdate<= '2003-04-21' group by ordernumber", SQL_NTS);*/

    while(!SQL_SUCCEEDED(SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, tok_1, 0, NULL)));
    while(!SQL_SUCCEEDED(SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, tok_2, 0, NULL)));
    while(!SQL_SUCCEEDED(SQLExecute(stmt)));

    size=(SQLLEN)sizeof(onumber);
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 1, SQL_INTEGER, &onumber, size, NULL)));

    size=(SQLLEN)sizeof(odate);
    if(!SQL_SUCCEEDED(SQLBindCol(stmt, 2, SQL_C_CHAR, &odate, size, NULL))|| !SQL_SUCCEEDED(SQLBindCol(stmt, 3, SQL_C_CHAR, &oshipp, size, NULL)))
      return;
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    { /*si encuentra la consulta*/
        if(printf("%d\t%s\t%s\n", onumber, odate, oshipp)==0)
          return;
    }
    while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
    while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
    if(EOF==puts(QRY)) return;
    return;
}

void ConsultasOrders_Details(SQLHDBC *dbc)
{
    SQLHSTMT stmt=NULL;
    char buff[25];
    SQLINTEGER onumber=0;
    SQLINTEGER qordered=0;
    SQLFLOAT peach=0.0;
    SQLCHAR pcode[10];
    SQLCHAR odate[11];
    SQLCHAR status[11];
    SQLLEN size;          /*variable auxiliar para los tamanos de las consultas*/
    if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt)))
      return;
    printf("Enter ordernumber > ");
    if(!(fgets(buff, 25, stdin))){
        /*si hay un error en la lectura*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    onumber = atoi(buff); /*convertimos lo leido a integer*/
    while(!SQL_SUCCEEDED(SQLPrepare(stmt, (SQLCHAR *)"Select O.orderdate, O.status, P.productcode, OD.quantityordered, OD.priceeach FROM orders O, products P, orderdetails OD where OD.ordernumber=O.ordernumber and OD.productcode=P.productcode and O.ordernumber= ? order by OD.orderlinenumber", SQL_NTS)));
    while(!SQL_SUCCEEDED(SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &onumber, 0, NULL)));
    while(!SQL_SUCCEEDED(SQLExecute(stmt)));

    size=(SQLLEN)sizeof(odate);
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 1, SQL_C_CHAR, &odate, size, NULL)));
    size=(SQLLEN)sizeof(status);
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 2, SQL_C_CHAR, &status, size, NULL)));
    size=(SQLLEN)sizeof(pcode);
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 3, SQL_C_CHAR, &pcode, size, NULL)));
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 4, SQL_C_LONG, &qordered, size, NULL)));
    size=(SQLLEN)sizeof(peach);
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 5, SQL_C_DOUBLE, &peach, size, NULL)));
    /*SQLBindCol(stmt, 5, SQL_C_CHAR, &aux, sizeof(aux), NULL);*/
    /*printf("%s\t%s\n", odate, status);*/
    if (SQL_SUCCEEDED(SQLFetch(stmt)))
        if(printf("%s\t%s\n%s\t%d\t%.2f\n", odate, status, pcode, qordered, peach)==0)
          return;
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    { /*si encuentra la consulta*/
        printf("%s\t%d\t%.2f\n", pcode, qordered, peach);
    }
    while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));
    while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt)));
    if(EOF==puts(QRY)) return;
    return;
}

void ConsultasProduct_Stock(SQLHDBC *dbc)
{
    SQLHSTMT stmt=NULL;
    char buff[16]; /*buffer donde guardar el input*/
    char *tok = NULL;
    SQLINTEGER stock=0;
    printf("Enter productcode > ");
    if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt)))
      return;
    if(!(fgets(buff, 16, stdin))){ /*lee el productcode deseado*/
        /*si hay un error en la lectura*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    if(!(tok = strtok(buff, "\n"))){
        /*si hay un error con los tok*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    while(!SQL_SUCCEEDED(SQLPrepare(stmt, (SQLCHAR *)"Select quantityinstock From products where productcode = ?", SQL_NTS))); /*preparamos la consulta*/
    while(!SQL_SUCCEEDED(SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, tok, 0, NULL)));            /*le introducimos el parametro*/
    while(!SQL_SUCCEEDED(SQLExecute(stmt)));;
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 1, SQL_C_SLONG, &stock, (SQLLEN)sizeof(SQLINTEGER), NULL)));

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    {                                                                 /*si encuentra la consulta*/
        printf("%d -> stock for product with id = %s\n", stock, tok); /*imprime el valor de la consulta*/
    }

    while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));
    while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos el statement*/
    if(EOF==puts(QRY)) return;
    return;
}

void ConsultasProduct_Find(SQLHDBC *dbc)
{
    SQLHSTMT stmt=NULL;
    SQLCHAR name[70];
    SQLCHAR code[15];
    char buff[16];
    char *tok = NULL;
    char *aux = NULL;
    printf("Enter productname > ");
    if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt)))
      return; /*allocate a statement handle*/
    if(!(fgets(buff, 16, stdin))){ /*lee el productcode deseado*/
        /*si hay un error en la lectura*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    if(!(tok = strtok(buff, "\n"))){
        /*si hay un error con los tok*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    if (!(aux = (char *)calloc(strlen(tok) + 2, sizeof(char))))
    { /*el auxiliar nos servirá para poder poner %text% como parámetro*/
        printf("Error, memoria insuficiente para la consulta\n");
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    strcat(aux, "%%");
    strcat(aux, tok);
    strcat(aux, "%%");
    while(!SQL_SUCCEEDED(SQLPrepare(stmt, (SQLCHAR *)"SELECT productcode, productname FROM products WHERE productname LIKE ? group by productcode", SQL_NTS)));

    while(!SQL_SUCCEEDED(SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, aux, 0, NULL)));
    while(!SQL_SUCCEEDED(SQLExecute(stmt)));;
    /*SQLExecDirect(stmt, (SQLCHAR*) "SELECT productcode, productname FROM products WHERE productname LIKE '%%Harley%%'", SQL_NTS);*/
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 1, SQL_C_CHAR, &code, (SQLLEN)sizeof(code), NULL))); /*la primera columna es el code*/
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 2, SQL_C_CHAR, &name, (SQLLEN)sizeof(name), NULL))); /*la segunda columna es el name*/

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(SQLFetchScroll(stmt, SQL_FETCH_NEXT, 0)))/*si encuentra la consulta*/
    {
        if(printf("%s\t%s\n", code, name)==0){/*imprime el valor de la consulta*/
          while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));
          while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos el statement*/
          free(aux);
          return;
        }
    }
    while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));
    while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos el statement*/
    free(aux);                            /*liberamos la memoria del auxiliar*/
    if(puts(QRY)==EOF) return;
    return;
}

void ConsultasCustomers_Find(SQLHDBC *dbc)
{
    SQLHSTMT stmt=NULL;
    SQLINTEGER cnumber=0;
    SQLCHAR cname[70];
    SQLCHAR cfname[70];
    SQLCHAR clname[70];
    char buff[16];
    char *tok = NULL;
    char *aux = NULL;
    int pag, pag_aux;
    pag=pag_aux=0;
    printf("Enter customer name > ");
    if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt)))
      return; /*allocate a statement handle*/
    Cursor_config(stmt);
    if(!(fgets(buff, 16, stdin))){ /*lee el nombre deseado*/
        /*si hay un error en la lectura*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    if(!(tok = strtok(buff, "\n"))){
        /*si hay un error con los tok*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    if (!(aux = (char *)calloc(strlen(tok) + 2, sizeof(char))))
    { /*el auxiliar nos servirá para poder poner %text% como parámetro*/
        printf("Error, memoria insuficiente para la consulta\n");
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    strcat(aux, "%%");
    strcat(aux, tok);
    strcat(aux, "%%");
    while(!SQL_SUCCEEDED(SQLPrepare(stmt, (SQLCHAR *)"select c.customernumber, c.customername, c.contactfirstname, c.contactlastname from customers c where c.contactfirstname like ? or c.contactlastname like ? order by c.customernumber;", SQL_NTS)));
    while(!SQL_SUCCEEDED(SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, aux, 0, NULL))); /*bindeas los parametros*/
    while(!SQL_SUCCEEDED(SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, aux, 0, NULL))); /*en este caso los dos son iguales*/
    while(!SQL_SUCCEEDED(SQLExecute(stmt)));; /*ejecutas el stmt*/

    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 1, SQL_C_LONG, &cnumber, (SQLLEN)sizeof(cnumber), NULL))); /*la primera columna es el customer number*/
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 2, SQL_C_CHAR, &cname, (SQLLEN)sizeof(cname), NULL))); /*la segunda columna es el customer name*/
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 3, SQL_C_CHAR, &cfname, (SQLLEN)sizeof(cfname), NULL))); /*la segunda columna es el contact first name*/
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 4, SQL_C_CHAR, &clname, (SQLLEN)sizeof(clname), NULL))); /*la segunda columna es el contact last name*/
    inicio_CF:
    while (SQL_SUCCEEDED(SQLFetchScroll(stmt, SQL_FETCH_NEXT, 0)))
    {
        /*si encuentra la consulta siguiente*/
        printf("%d\t%s\t%s\t%s\n", cnumber, cname, cfname, clname); /*imprime el valor de la consulta*/
        pag_aux++; /*suma 1 al valor de los valores mostrados en esta pag*/
        if(pag_aux==10){ /*si ya has terminado con los 10 de la página*/
            if(Consultas_10(&stmt, &pag, 0, &pag_aux)==1)/*llama a consultas_10 para consultar que acción ejecutar*/
                goto fin_CF; /*si retorna un 1 indica que el usuario quiere salir*/
            if(pag==0){ /*para que no se salte el primer valor*/
                printf("%d\t%s\t%s\t%s\n", cnumber, cname, cfname, clname); /*imprime el valor de la consulta*/
                pag_aux++; /*suma 1 al valor de los valores mostrados en esta pag*/
            }
        }
    }
    /*si ha llegado a este punto es que está en el final*/
    if(Consultas_10(&stmt, &pag, 1, &pag_aux)==0){
        pag_aux=0;
        if(pag==0){
            pag_aux++;
            printf("%d\t%s\t%s\t%s\n", cnumber, cname, cfname, clname); /*imprime el valor de la consulta*/

        }

        goto inicio_CF;
    }
    fin_CF:
    while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));
    while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos el statement*/
    free(aux);                            /*liberamos la memoria del auxiliar*/
    if(puts(QRY)==EOF) return;
    return;
}

void ConsultasCustomers_Balance(SQLHDBC *dbc){
    SQLHSTMT stmt1=NULL;
    SQLHSTMT stmt2=NULL;
    SQLDOUBLE pagos=0.0;
    SQLDOUBLE deudas=0.0;
    char buff[16];
    char *tok = NULL;
    printf("Enter customer number > ");
    if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt1))) /*allocate a statement handle*/
      return;
    if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt2))){ /*allocate a statement handle*/
      while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt1)));
      return;
    }
    if(!(fgets(buff, 16, stdin))){ /*lee el productcode deseado*/
        /*si hay un error en la lectura*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt1)));                 /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt1))); /*liberamos la consulta*/
        return;
    }
    if(!(tok = strtok(buff, "\n"))){
        /*si hay un error con los tok*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt1)));
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt1))); /*liberamos la consulta*/
        return;
    }

    /*SQLPrepare(stmt1, (SQLCHAR *)"select (cnumber_pagos.pagos-cnumber_deudas.deudas) from customers c join (select c.customernumber cnumber, Sum(p.amount) pagos from customers c join payments p on c.customernumber=p.customernumber group by c.customernumber) cnumber_pagos on c.customernumber=cnumber_pagos.cnumber join (select c.customernumber cnumber, SUM(od.quantityordered * od.priceeach) deudas from customers c join orders o on c.customernumber=o.customernumber join orderdetails od on od.ordernumber=o.ordernumber group by c.customernumber) cnumber_deudas on cnumber_deudas.cnumber=cnumber_pagos.cnumber where c.customernumber=?", SQL_NTS);*/
    while(!SQL_SUCCEEDED(SQLPrepare(stmt1, (SQLCHAR *)"select sum(p.amount) from customers c, payments p where c.customernumber=p.customernumber and c.customernumber=? group by p.customernumber;", SQL_NTS))); /*un primer statement para los pagos*/
    while(!SQL_SUCCEEDED(SQLBindParameter(stmt1, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, tok, 0, NULL)));
    while(!SQL_SUCCEEDED(SQLExecute(stmt1)));
    while(!SQL_SUCCEEDED(SQLBindCol(stmt1, 1, SQL_C_DOUBLE, &pagos, (SQLLEN)sizeof(pagos), NULL)));

    /*repetimos para las deudas*/
    while(!SQL_SUCCEEDED(SQLPrepare(stmt2, (SQLCHAR *)"select sum(od.quantityordered * od.priceeach) from customers c, orders o, orderdetails od where c.customernumber=o.customernumber and od.ordernumber=o.ordernumber and c.customernumber=? group by c.customernumber;", SQL_NTS)));
    while(!SQL_SUCCEEDED(SQLBindParameter(stmt2, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, tok, 0, NULL)));
    while(!SQL_SUCCEEDED(SQLExecute(stmt2)));
    while(!SQL_SUCCEEDED(SQLBindCol(stmt2, 1, SQL_C_DOUBLE, &deudas, (SQLLEN)sizeof(deudas), NULL)));

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(SQLFetch(stmt1)) && SQL_SUCCEEDED(SQLFetch(stmt2)))
    {                                   /*si encuentra la consulta*/
        if(printf("Balance = %.2f\n", pagos-deudas)==0) /*imprime el valor de la consulta*/
          return;
    }
    while(!SQL_SUCCEEDED(SQLCloseCursor(stmt1)));
    while(!SQL_SUCCEEDED(SQLCloseCursor(stmt2)));
    while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt1)));
    while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt2)));
    if(puts(QRY)==EOF) return;
    return;
}

void ConsultasCustomers_ListProducts(SQLHDBC *dbc){
    SQLHSTMT stmt=NULL;
    SQLINTEGER cnumber=0;
    SQLINTEGER sum=0;
    SQLCHAR name[70];
    char buff[16];
    char *tok=NULL;

    int pag_aux=0, pag=0;
    printf("Enter customernumber > ");
    if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt)))
      return; /*allocate a statement handle*/
    if(!(fgets(buff, 16, stdin))){ /*lee el productcode deseado*/
        /*si hay un error en la lectura*/
        if(puts(QRYERR)==EOF) return;
        while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));                  /*cerramos el cursor*/
        while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos la consulta*/
        return;
    }
    Cursor_config(stmt); /*configuramos el cursor*/
    /*SQLSetStmtAttr((stmt), SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER) ROWSET_SIZE,0);*/
    tok=strtok(buff, "\n");
    cnumber=atoi(tok);
    while(!SQL_SUCCEEDED(SQLPrepare(stmt, (SQLCHAR *)"select p.productname, sum(od.quantityordered) from customers c join orders o on c.customernumber=o.customernumber join orderdetails od on od.ordernumber=o.ordernumber join products p on p.productcode=od.productcode where c.customernumber=? group by p.productcode, od.productcode order by od.productcode;", SQL_NTS)));
    while(!SQL_SUCCEEDED(SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &cnumber, 0, NULL)));
    while(!SQL_SUCCEEDED(SQLExecute(stmt)));
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 1, SQL_C_CHAR, &name, (SQLLEN)sizeof(name), NULL)));
    while(!SQL_SUCCEEDED(SQLBindCol(stmt, 2, SQL_C_LONG, &sum, (SQLLEN)sizeof(sum), NULL)));
inicio_CLP:
    while (SQL_SUCCEEDED(SQLFetchScroll(stmt, SQL_FETCH_RELATIVE, 1)))
    {
        /*si encuentra la consulta siguiente*/
        printf("%s\t%d\n", name, sum); /*imprime el valor de la consulta*/
        pag_aux++; /*suma 1 al valor de los valores mostrados en esta pag*/
        if(pag_aux==10){ /*si ya has terminado con los 10 de la página*/
            if(Consultas_10(&stmt, &pag, 0, &pag_aux)==1)/*llama a consultas_10 para consultar que acción ejecutar*/
                goto fin_CLP; /*si retorna un 1 indica que el usuario quiere salir*/
            if(pag==0){ /*para que no se salte el primer valor*/
                printf("%s\t%d\n", name, sum); /*imprime el valor de la consulta*/
                pag_aux++; /*suma 1 al valor de los valores mostrados en esta pag*/
            }
        }
    }
    /*si ha llegado a este punto es que está en el final*/
    if(Consultas_10(&stmt, &pag, 1, &pag_aux)==0){
        pag_aux=0;
        if(pag==0){
            pag_aux++;
            printf("%s\t%d\n", name, sum); /*imprime el valor de la consulta*/
        }

        goto inicio_CLP;
    }
    fin_CLP:
    while(!SQL_SUCCEEDED(SQLCloseCursor(stmt)));
    while(!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_STMT, stmt))); /*liberamos el statement*/
    if(puts(QRY)==EOF) return;
    return;
}

void Cursor_config(SQLHSTMT *stmt){
    while(!SQL_SUCCEEDED(SQLSetStmtAttr((stmt), SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER) ROWSET_SIZE,0)));
    while(!SQL_SUCCEEDED(SQLSetStmtAttr((stmt), SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER) 1, 0)));
    while(!SQL_SUCCEEDED(SQLSetStmtAttr((stmt), SQL_ATTR_CURSOR_TYPE, (SQLPOINTER) SQL_CURSOR_STATIC, 0)));
    return;
}

int Consultas_10(SQLHSTMT *stmt, int *pag, int aux, int *pag_aux){
    char ind='\0';
    if(aux==0){ /*si no esta al final*/
        if((*pag)>0){ /*si la pagina no es la primer resultado y aún quedan resultados porque dió succeeded*/
            do{
                printf("\n  press < to go back or press > to continue or s to stop: ");
                ind=(char)getchar();
                if(getchar()==EOF) /*para eliminar el \n*/
                  ind='s';
                if(ind!='<' && ind!='>' && ind!='s'){ /*si no está en ninguna de las opciones posibles*/
                    printf("You have entered an invalid choice. Please try again\n\n\n");
                }
            }while(ind!='<' && ind!='>' && ind!='s');
        }
        /*es la primera pag de resultados, no puede mostrar resultados anteriores*/
        else{
            do{
                printf("\n  press > to continue or s to stop: ");
                ind=(char)getchar();
                if(getchar()==EOF) /*para eliminar el \n*/
                  ind='s';
                if(ind!='>' && ind!='s'){ /*si no está en ninguna de las opciones posibles*/
                    printf(" You have entered an invalid choice. Please try again\n\n\n");
                }
            }while(ind!='>' && ind!='s');
        }
        if(ind=='s')/*si selecciono el stop*/
            return 1;
        if(ind=='<'){ /*si quiere ir para atrás*/
            if(SQL_SUCCEEDED(SQLFetchScroll(*stmt, SQL_FETCH_RELATIVE, (-10-(*pag_aux))))){ /*20 consultas para atras para que muestre las 10 anteriores*/
                (*pag)--;
                (*pag_aux)=0; /*actualizas el valor de pag_aux*/
                return 0;
            }
            /*en caso de error*/
            return 1;
        }
        /*caso contrario quiere seguir*/
        (*pag)++; /*sumamos uno al numero de paginas (principalmente para saber que no estamos en la primera pag)*/
        (*pag_aux)=0; /*actualizas el valor de pag_aux*/
        return 0;
    }
    /*si está al final*/
    else{
        if((*pag)>0){ /*si la pagina no es la primer resultado*/
            do{
                printf("\n  press < to go back or s to stop: ");
                ind=(char)getchar();
                if(getchar()==EOF) /*para eliminar el \n*/
                  ind='s';
                if(ind!='<' && ind!='s'){ /*si no está en ninguna de las opciones posibles*/
                    printf(" You have entered an invalid choice. Please try again\n\n\n");
                }
            }while(ind!='<' && ind!='s');
            if(ind=='s')
                return 1;
            else{
                if(SQL_SUCCEEDED(SQLFetchScroll(*stmt, SQL_FETCH_RELATIVE, -10-(++(*pag_aux))))){ /*20 consultas para atras para que muestre las 10 anteriores*/
                    (*pag)--;
                    (*pag_aux)=0; /*actualizas el valor de pag_aux*/
                    return 0;
                }
                /*en caso de error*/
                return 1;
            }
        }
    }
    return 1;
}
