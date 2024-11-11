#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlQueryModel>

class Transaction
{
private:
    int id_transaction;
    QString type_transaction;
    QString date_transaction;
    QString etat_paiement;
    QString mode_paiement;
    double montant_total;


public:
    Transaction();
    Transaction(int id, const QString &type, const QString &date_transaction = "", const QString &etat = "", const QString &mode = "", double montant = 0.0);

    int getid_transaction() const;
    QString gettype_transaction() const;
    QString getdate_transaction() const;
    QString getetat_paiement() const;
    QString getmode_paiement() const;
    double getmontant_total() const;

    void setid_transaction(int id);
    void settype_transaction(const QString& type);
    void setdate_transaction(const QString& date);
    void setetat_paiement(const QString& etat);
    void setmode_paiement(const QString& mode);
    void setmontant_total(double montant);




    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id_transaction, QString type_transaction, QString date_transaction , QString mode_paiement ,QString etat_paiement ,float montant_total);
    QSqlQuery afficher();

};

#endif // TRANSACTION_H
