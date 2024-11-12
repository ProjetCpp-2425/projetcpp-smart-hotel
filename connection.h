#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Connection {
private:
    QSqlDatabase db; // Attribut pour la connexion à la base de données

public:
    Connection(); // Constructeur
    bool createconnect(); // Déclaration de la méthode
};

#endif // CONNECTION_H
