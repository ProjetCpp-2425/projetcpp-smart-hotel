#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("source2a32");
db.setUserName("projetcpp2a32");
db.setPassword("mp2a32");

if (db.open())
test=true;
    return  test;
}
void Connection::closeConnect (){db.close();}
