#include "connection.h"
#include <QSqlError>
#include <QDebug>

Connection::Connection() {}

bool Connection::createconnect() {
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("source2a32"); // Replace with your database name
    db.setUserName("projetcpp2a32");  // Replace with your username
    db.setPassword("mp2a32");         // Replace with your password

    // Attempt to open the database connection
    if (!db.open()) {
        qDebug() << "Database Connection Error:" << db.lastError().text();
        return false;
    }

    // Connection succeeded
    qDebug() << "Database connection established successfully.";
    return true;
}
