#ifndef CONSULTAS_H
#define CONSULTAS_H

#define QRY "\nQuery done. Going back...\n"
#define QRYERR "\nWrong input (unable to make this query). Going back...\n"
void ConsultasProduct(int *opt, SQLHDBC *dbc);
void ConsultasOrders(int *opt, SQLHDBC *dbc);
#endif
