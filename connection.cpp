#include "connection.h"
//test
Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("source2a32");
    db.setUserName("projetcpp2a32");//inserer nom de l'utilisateur
    db.setPassword("mp2a32");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;

    return  test;
}
