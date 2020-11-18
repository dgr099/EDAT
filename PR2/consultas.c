#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*PROTOTIPOS PRIVADAS*/
void ConsultasProduct_Stock(SQLHDBC *dbc);
void ConsultasProduct_Find(SQLHDBC *dbc);
void ConsultasOrders_Open(SQLHDBC *dbc);
void ConsultasOrders_Range(SQLHDBC *dbc);
void ConsultasOrders_Details(SQLHDBC *dbc);
void ConsultasCustomers_Find(SQLHDBC *dbc);
void ConsultasOrders_Balance(SQLHDBC *dbc);
void ConsultasCustomers_ListProducts(SQLHDBC *dbc);
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

void ConsultasOrders(i*opt, SQLHDBC *dbc)
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
        ConsultasOrders_Balance(dbc);
        break;
    }
    default:
        break;
    }
}


void ConsultasOrders_Open(SQLHDBC *dbc)
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
        printf("Error en la introducción de datos\n");
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    if(!(tok_1 = strtok(buff, " "))){
        /*si hay un error con los toks*/
        printf("Error en la introducción de datos\n");
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    strtok(NULL, " ");
    if(!(tok_2 = strtok(NULL, "\n"))){
        /*si hay un con los toks*/
        printf("Error en la introducción de datos\n");
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
        printf("Error en la introducción de datos\n");
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
}

void ConsultasProduct_Stock(SQLHDBC *dbc)
{
    SQLHSTMT stmt;
    char buff[16]; /*buffer donde guardar el input*/
    char *tok = NULL;
    SQLINTEGER stock;
    printf("Enter productcode > ");
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt); 
    if(!(fgets(buff, 16, stdin))){ /*lee el productcode deseado*/
        /*si hay un error en la lectura*/
        printf("Error en la introducción de datos\n");
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    if(!(tok = strtok(buff, "\n"))){
        /*si hay un error con los tok*/
        printf("Error en la introducción de datos\n");
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return; 
    }
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
    return;
}

void ConsultasProduct_Find(SQLHDBC *dbc)
{
    SQLHSTMT stmt;
    SQLCHAR name[70];
    SQLCHAR code[15];
    char buff[16];
    char *tok = NULL;
    char *aux = NULL;
    printf("Enter productname > ");
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt); /*allocate a statement handle*/
    if(!(fgets(buff, 16, stdin))){ /*lee el productcode deseado*/
        /*si hay un error en la lectura*/
        printf("Error en la introducción de datos\n");
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    if(!(tok = strtok(buff, "\n"))){
        /*si hay un error con los tok*/
        printf("Error en la introducción de datos\n");
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
    return;
}

void ConsultasCustomers_Find(SQLHDBC *dbc)
{
    SQLHSTMT stmt;
    SQLINTEGER cnumber;
    SQLCHAR cname[70];
    SQLCHAR cfname[70];
    SQLCHAR clname[70];
    char buff[16];
    char *tok = NULL;
    char *aux = NULL;
    printf("Enter customer name > ");
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt); /*allocate a statement handle*/
    if(!(fgets(buff, 16, stdin))){ /*lee el nombre deseado*/
        /*si hay un error en la lectura*/
        printf("Error en la introducción de datos\n");
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    if(!(tok = strtok(buff, "\n"))){
        /*si hay un error con los tok*/
        printf("Error en la introducción de datos\n");
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
    SQLPrepare(stmt, (SQLCHAR *)"select c.customernumber, c.customername, c.contactfirstname, c.contactlastname from customers c where c.contactfirstname like ? or c.contactlastname like ? order by c.customernumber;", SQL_NTS);

    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, aux, 0, NULL); /*bindeas los parametros*/
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, aux, 0, NULL); /*en este caso los dos son iguales*/
    SQLExecute(stmt); /*ejecutas el stmt*/

    SQLBindCol(stmt, 1, SQL_C_LONG, &cnumber, sizeof(cnumber), NULL); /*la primera columna es el customer number*/
    SQLBindCol(stmt, 2, SQL_C_CHAR, &cname, sizeof(cname), NULL); /*la segunda columna es el customer name*/
    SQLBindCol(stmt, 3, SQL_C_CHAR, &cfname, sizeof(cfname), NULL); /*la segunda columna es el contact first name*/
    SQLBindCol(stmt, 4, SQL_C_CHAR, &clname, sizeof(clname), NULL); /*la segunda columna es el contact last name*/


    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    {                                   /*si encuentra la consulta*/
        printf("%d\t%s\t%s\t%s\n", cnumber, cname, cfname, clname); /*imprime el valor de la consulta*/
    }
    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos el statement*/
    free(aux);                            /*liberamos la memoria del auxiliar*/
    return;
}

void ConsultasOrders_Balance(SQLHDBC *dbc){
    SQLHSTMT stmt1;
    SQLHSTMT stmt2;
    SQLDOUBLE pagos;
    SQLDOUBLE deudas;
    char buff[16];
    char *tok = NULL;
    printf("Enter customer number > ");
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt1); /*allocate a statement handle*/
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt2); /*allocate a statement handle*/
    if(!(fgets(buff, 16, stdin))){ /*lee el productcode deseado*/
        /*si hay un error en la lectura*/
        printf("Error en la introducción de datos\n");
        SQLCloseCursor(stmt1);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt1); /*liberamos la consulta*/
        return;
    }
    if(!(tok = strtok(buff, "\n"))){
        /*si hay un error con los tok*/
        printf("Error en la introducción de datos\n");
        SQLCloseCursor(stmt1);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt1); /*liberamos la consulta*/
        return;
    }

    /*SQLPrepare(stmt1, (SQLCHAR *)"select (cnumber_pagos.pagos-cnumber_deudas.deudas) from customers c join (select c.customernumber cnumber, Sum(p.amount) pagos from customers c join payments p on c.customernumber=p.customernumber group by c.customernumber) cnumber_pagos on c.customernumber=cnumber_pagos.cnumber join (select c.customernumber cnumber, SUM(od.quantityordered * od.priceeach) deudas from customers c join orders o on c.customernumber=o.customernumber join orderdetails od on od.ordernumber=o.ordernumber group by c.customernumber) cnumber_deudas on cnumber_deudas.cnumber=cnumber_pagos.cnumber where c.customernumber=?", SQL_NTS);*/
    SQLPrepare(stmt1, (SQLCHAR *)"select sum(p.amount) from customers c, payments p where c.customernumber=p.customernumber and c.customernumber=? group by p.customernumber;", SQL_NTS); /*un primer statement para los pagos*/
    SQLBindParameter(stmt1, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, tok, 0, NULL);
    SQLExecute(stmt1);
    SQLBindCol(stmt1, 1, SQL_C_DOUBLE, &pagos, sizeof(pagos), NULL);

    SQLPrepare(stmt2, (SQLCHAR *)"select sum(od.quantityordered * od.priceeach) from customers c, orders o, orderdetails od where c.customernumber=o.customernumber and od.ordernumber=o.ordernumber and c.customernumber=? group by c.customernumber;", SQL_NTS); /*un segundo statement para las deudas*/
    SQLBindParameter(stmt2, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, tok, 0, NULL);
    SQLExecute(stmt2);
    SQLBindCol(stmt2, 1, SQL_C_DOUBLE, &deudas, sizeof(deudas), NULL);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(SQLFetch(stmt1)) && SQL_SUCCEEDED(SQLFetch(stmt2)))
    {                                   /*si encuentra la consulta*/
        printf("Balance = %.2f\n", pagos-deudas); /*imprime el valor de la consulta*/
    }
    SQLCloseCursor(stmt1);
    SQLCloseCursor(stmt2);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt1);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
    return;    
}
void ConsultasCustomers_ListProducts(SQLHDBC *dbc){
    SQLHSTMT stmt;
    SQLINTEGER cnumber;
    SQLINTEGER sum;
    SQLCHAR name[70];
    char buff[16];
    char *tok=NULL;
    printf("Enter customernumber > ");
    SQLAllocHandle(SQL_HANDLE_STMT, *dbc, &stmt);
    if(!(fgets(buff, 16, stdin))){ /*lee el productcode deseado*/
        /*si hay un error en la lectura*/
        printf("Error en la introducción de datos\n");
        SQLCloseCursor(stmt);                 /*cerramos el cursor*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos la consulta*/
        return;
    }
    tok=strtok(buff, "\n");
    cnumber=atoi(tok);
    SQLPrepare(stmt, (SQLCHAR *)"select p.productname, sum(od.quantityordered) from customers c join orders o on c.customernumber=o.customernumber join orderdetails od on od.ordernumber=o.ordernumber join products p on p.productcode=od.productcode where c.customernumber=? group by p.productcode, od.productcode order by od.productcode;", SQL_NTS);
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &cnumber, 0, NULL);
    SQLExecute(stmt);
    SQLBindCol(stmt, 1, SQL_C_CHAR, &name, sizeof(name), NULL); 
    SQLBindCol(stmt, 2, SQL_C_LONG, &sum, sizeof(sum), NULL); 
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    {                                   /*si encuentra la consulta*/
        printf("%s\t%d\n", name, sum); /*imprime el valor de la consulta*/
    }
    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt); /*liberamos el statement*/
    return; 
}
/*
        if(pag_aux==10){ /*si ya has terminado con los 10 de la página
            if(pag>0){ /*si la pagina no es la primer
                printf("press < to go back 10 results or press > to show more results or s to stop: ");
                ind=(char)fgetc(stdin);
                if(ind!='<' && ind!='>' && ind!='s'){ /*si no está en ninguna de las opciones posibles

                }
            }
            else
                printf("press > to show more results: ");
            ind=(char)fgetc(stdin);
        }
        */
