#include "client.h"

Client::Client() {}

Client::Client(const QString &nom, const QString &prenom, const QString &dateNaissance, const QString &email, const QString &telephone, const QString &cin)
    : nom(nom), prenom(prenom), dateNaissance(dateNaissance), email(email), telephone(telephone), cin(cin) {}

QString Client::getNom() const { return nom; }
QString Client::getPrenom() const { return prenom; }
QString Client::getDateNaissance() const { return dateNaissance; }
QString Client::getEmail() const { return email; }
QString Client::getTelephone() const { return telephone; }
QString Client::getCin() const { return cin; }

void Client::setNom(const QString &nom) { this->nom = nom; }
void Client::setPrenom(const QString &prenom) { this->prenom = prenom; }
void Client::setDateNaissance(const QString &dateNaissance) { this->dateNaissance = dateNaissance; }
void Client::setEmail(const QString &email) { this->email = email; }
void Client::setTelephone(const QString &telephone) { this->telephone = telephone; }
void Client::setCin(const QString &cin) { this->cin = cin; }
