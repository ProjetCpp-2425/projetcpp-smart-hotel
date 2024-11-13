#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

class Client {
public:
    Client();
    Client(const QString &nom, const QString &prenom, const QString &dateNaissance, const QString &email, const QString &telephone, const QString &cin);

    QString getNom() const;
    QString getPrenom() const;
    QString getDateNaissance() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getCin() const;

    void setNom(const QString &nom);
    void setPrenom(const QString &prenom);
    void setDateNaissance(const QString &dateNaissance);
    void setEmail(const QString &email);
    void setTelephone(const QString &telephone);
    void setCin(const QString &cin);

private:
    QString nom;
    QString prenom;
    QString dateNaissance;
    QString email;
    QString telephone;
    QString cin;
};

#endif // CLIENT_H
