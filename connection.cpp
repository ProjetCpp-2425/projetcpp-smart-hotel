#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
<<<<<<< HEAD

=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
db.setDatabaseName("source2a32");//inserer le nom de la source de données
db.setUserName("projetcpp2a32");//inserer nom de l'utilisateur
db.setPassword("mp2a32");//inserer mot de passe de cet utilisateur

<<<<<<< HEAD



=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
if (db.open())
test=true;





    return  test;
}
