#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect() {
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("source2a32"); // Remplacez par le nom de votre base
    db.setUserName("projetcpp2a32");        // Remplacez par votre nom d'utilisateur
    db.setPassword("mp2a32");       // Remplacez par votre mot de passe

    if (!db.open()) {
        // Affichage du message d'erreur si la connexion échoue
        qDebug() << "Erreur lors de l'ouverture de la base de données : " << db.lastError().text();
        return false;
    }
    return true; // La connexion a réussi
}
