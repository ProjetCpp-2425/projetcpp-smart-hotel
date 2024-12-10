#include "client.h"
#include <QDebug>
// Constructors
Client::Client() : visite(0) {} // Default value for VISITE is 0

Client::Client(const QString &nom, const QString &prenom, const QString &dateNaissance, const QString &email, const QString &telephone, const QString &cin, int visite)
    : nom(nom), prenom(prenom), dateNaissance(dateNaissance), email(email), telephone(telephone), cin(cin), visite(visite) {}

// Getters
QString Client::getNom() const { return nom; }
QString Client::getPrenom() const { return prenom; }
QString Client::getDateNaissance() const { return dateNaissance; }
QString Client::getEmail() const { return email; }
QString Client::getTelephone() const { return telephone; }
QString Client::getCin() const { return cin; }
int Client::getVisite() const { return visite; }

// Setters
void Client::setNom(const QString &nom) { this->nom = nom; }
void Client::setPrenom(const QString &prenom) { this->prenom = prenom; }
void Client::setDateNaissance(const QString &dateNaissance) { this->dateNaissance = dateNaissance; }
void Client::setEmail(const QString &email) { this->email = email; }
void Client::setTelephone(const QString &telephone) { this->telephone = telephone; }
void Client::setCin(const QString &cin) { this->cin = cin; }
void Client::setVisite(int visite) { this->visite = visite; }


// Add a client to the database
bool Client::ajouter() {
    QSqlQuery query;
        query.prepare("INSERT INTO client (NOM, PRENOM, DATENAISSANCE, EMAIL, TELEPHONE, CIN, VISITE) "
                      "VALUES (:nom, :prenom, :dateNaissance, :email, :telephone, :cin, :visite)");
        query.bindValue(":nom", nom);
        query.bindValue(":prenom", prenom);
        query.bindValue(":dateNaissance", dateNaissance);
        query.bindValue(":email", email);
        query.bindValue(":telephone", telephone);
        query.bindValue(":cin", cin);
        query.bindValue(":visite", visite);
        return query.exec();
}

QSqlQueryModel* Client::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT CIN, NOM, PRENOM, DATENAISSANCE, EMAIL, TELEPHONE, VISITE FROM client");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date de Naissance"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Visite"));
    return model;
}

// Delete a client by CIN
bool Client::supprimer(QString c) {
    QSqlQuery query;
    query.prepare("DELETE FROM client WHERE CIN = :cin");
    query.bindValue(":cin", c);
    return query.exec();
}

// Modify a client's information
bool Client::modifier(QString c, QString n, QString p, QString dn, QString e, QString t) {
    QSqlQuery query;
    query.prepare("UPDATE client SET NOM = :nom, PRENOM = :prenom, DATENAISSANCE = :dateNaissance, "
                  "EMAIL = :email, TELEPHONE = :telephone WHERE CIN = :cin");
    query.bindValue(":cin", c);
    query.bindValue(":nom", n);
    query.bindValue(":prenom", p);
    query.bindValue(":dateNaissance", dn);
    query.bindValue(":email", e);
    query.bindValue(":telephone", t);
    return query.exec();
}

// Check if a client exists
bool Client::exists(QString cin) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM client WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    return query.exec() && query.next() && query.value(0).toInt() > 0;
}

// Retrieve all phone numbers
QList<QString> Client::getAllPhoneNumbers() {
    QList<QString> phones;
    QSqlQuery query("SELECT TELEPHONE FROM client");
    while (query.next()) {
        phones.append(query.value(0).toString());
    }
    return phones;
}

// Sort clients
QSqlQueryModel* Client::sortTable(const QString &sortColumn, bool ascending) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT CIN, NOM, PRENOM, DATENAISSANCE, EMAIL, TELEPHONE FROM client";
    if (!sortColumn.isEmpty()) {
        queryStr += " ORDER BY " + sortColumn + (ascending ? " ASC" : " DESC");
    }
    model->setQuery(queryStr);
    return model;
}

bool Client::incrementVisite(QString c) {
    QSqlQuery query;
    query.prepare("UPDATE client SET VISITE = VISITE + 1 WHERE CIN = :cin");
    query.bindValue(":cin", c);
    return query.exec();
}


Client Client::trouverClientParCin(const QString &cin)
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, DATENAISSANCE, EMAIL, TELEPHONE, VISITE FROM client WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    // Execute the query and handle errors
    if (!query.exec()) {
        qDebug() << "Error executing query";
        return Client();  // Return an empty client object if query fails
    }

    // Check if we found the client
    if (query.next()) {
        // Create a Client object and set its properties from the database result
        Client client;
        client.setNom(query.value(0).toString());
        client.setCin(cin);
        client.setPrenom(query.value(1).toString());
        client.setDateNaissance(query.value(2).toString());
        client.setEmail(query.value(3).toString());
        client.setTelephone(query.value(4).toString());
        client.setVisite(query.value(5).toInt());
        return client;  // Return the found client
    } else {
        // If no client was found, log a message and return an empty Client
        qDebug() << "No client found with CIN: " << cin;
        return Client();  // Return an empty client object if not found
    }
}
