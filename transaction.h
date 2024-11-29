#ifndef TRANSACTION_H
#define TRANSACTION_H

<<<<<<< HEAD

=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
#include <QString>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlQueryModel>

<<<<<<< HEAD

=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
class Transaction
{
private:
    int id_transaction;
    QString type_transaction;
    QString date_transaction;
    QString etat_paiement;
    QString mode_paiement;
    double montant_total;


<<<<<<< HEAD

=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
public:
    Transaction();
    Transaction(int id, const QString &type, const QString &date_transaction = "", const QString &etat = "", const QString &mode = "", double montant = 0.0);

    int getid_transaction() const;
    QString gettype_transaction() const;
    QString getdate_transaction() const;
    QString getetat_paiement() const;
    QString getmode_paiement() const;
    double getmontant_total() const;

<<<<<<< HEAD

=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    void setid_transaction(int id);
    void settype_transaction(const QString& type);
    void setdate_transaction(const QString& date);
    void setetat_paiement(const QString& etat);
    void setmode_paiement(const QString& mode);
    void setmontant_total(double montant);




<<<<<<< HEAD

    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id_transaction, QString type_transaction, QString date_transaction , QString mode_paiement ,QString etat_paiement ,float montant_total);
       QSqlQuery afficher();

    double calculerSommeDepenses();


=======
    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id_transaction, QString type_transaction, QString date_transaction , QString mode_paiement ,QString etat_paiement ,float montant_total);
    QSqlQuery afficher();
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8

};

#endif // TRANSACTION_H
