#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QCryptographicHash>
#include <QFile>

class Client {
public:
    Client();
    Client(const QString &nom, const QString &prenom, const QString &dateNaissance, const QString &email, const QString &telephone, const QString &cin,int);

    QString getNom() const;
    QString getPrenom() const;
    QString getDateNaissance() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getCin() const;
    int getVisite() const;


    void setNom(const QString &nom);
    void setPrenom(const QString &prenom);
    void setDateNaissance(const QString &dateNaissance);
    void setEmail(const QString &email);
    void setTelephone(const QString &telephone);
    void setCin(const QString &cin);
    void setVisite(int visite);


    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(QString c);
    bool modifier(QString c, QString n, QString p, QString dn, QString e, QString t);
    bool exists(QString cin);
    QList<QString> getAllPhoneNumbers();
    QSqlQueryModel* sortTable(const QString &sortColumn, bool ascending);
    bool incrementVisite(QString c);
    Client trouverClientParCin(const QString &cin);
    static QList<QPair<QString, int>> getHistoriqueVisites();
private:
    QString nom;
    QString prenom;
    QString dateNaissance;
    QString email;
    QString telephone;
    QString cin;
    int visite;
};

#endif // CLIENT_H
