#include "transaction.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
<<<<<<< HEAD
#include <iostream>
#include <vector>
=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8

Transaction::Transaction()
    : id_transaction(0), type_transaction(""), date_transaction(""), etat_paiement("impayé"),
      mode_paiement("espèce"), montant_total(0.0) {}

Transaction::Transaction(int id_transaction, const QString& type_transaction, const QString& date_transaction,
                         const QString& etat_paiement, const QString& mode_paiement, double montant_total)
    : id_transaction(id_transaction), type_transaction(type_transaction), date_transaction(date_transaction),
      etat_paiement(etat_paiement), mode_paiement(mode_paiement), montant_total(montant_total) {}

int Transaction::getid_transaction() const { return id_transaction; }
QString Transaction::gettype_transaction() const { return type_transaction; }
QString Transaction::getdate_transaction() const { return date_transaction; }
QString Transaction::getetat_paiement() const { return etat_paiement; }
QString Transaction::getmode_paiement() const { return mode_paiement; }
double Transaction::getmontant_total() const { return montant_total; }

<<<<<<< HEAD

=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
void Transaction::setid_transaction(int id) { id_transaction = id; }
void Transaction::settype_transaction(const QString& type) { type_transaction = type; }
void Transaction::setdate_transaction(const QString& date) { date_transaction = date; }
void Transaction::setetat_paiement(const QString& etat) { etat_paiement = etat; }
void Transaction::setmode_paiement(const QString& mode) { mode_paiement = mode; }
void Transaction::setmontant_total(double montant) { montant_total = montant; }

bool Transaction::ajouter()
{
    QSqlQuery query;
<<<<<<< HEAD
    query.prepare("INSERT INTO transaction (id_transaction, type_transaction, date_transaction, etat_paiement, mode_paiement, montant_total) "
                  "VALUES (:id_transaction, :type_transaction, TO_DATE(:date_transaction, 'YYYY-MM-DD'), :etat_paiement, :mode_paiement, :montant_total)");
    query.bindValue(":id_transaction", id_transaction);
    query.bindValue(":type_transaction", type_transaction);
    query.bindValue(":date_transaction", date_transaction);
    query.bindValue(":etat_paiement", etat_paiement);
     query.bindValue(":mode_paiement", mode_paiement);
=======
    query.prepare("INSERT INTO transaction (id_transaction, type_transaction, date_transaction, mode_paiement, etat_paiement, montant_total) "
                  "VALUES (:id_transaction, :type_transaction, TO_DATE(:date_transaction, 'YYYY-MM-DD'), :mode_paiement, :etat_paiement, :montant_total)");
    query.bindValue(":id_transaction", id_transaction);
    query.bindValue(":type_transaction", type_transaction);
    query.bindValue(":date_transaction", date_transaction);
    query.bindValue(":mode_paiement", mode_paiement);
    query.bindValue(":etat_paiement", etat_paiement);
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    query.bindValue(":montant_total", montant_total);

    return query.exec();
}

bool Transaction::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM transaction WHERE id_transaction = :id_transaction");
    query.bindValue(":id_transaction", id);
    return query.exec() && query.numRowsAffected() > 0;
}

<<<<<<< HEAD
bool Transaction::modifier(int id_transaction, QString type_transaction, QString date_transaction,QString etat_paiement,
                           QString mode_paiement, float montant_total)
{
    QSqlQuery query;
    query.prepare("UPDATE transaction SET type_transaction = :type_transaction, date_transaction = TO_DATE(:date_transaction, 'YYYY-MM-DD'), "
                  "etat_paiement = :etat_paiement,mode_paiement = :mode_paiement,  montant_total = :montant_total "
=======
bool Transaction::modifier(int id_transaction, QString type_transaction, QString date_transaction,
                           QString mode_paiement, QString etat_paiement, float montant_total)
{
    QSqlQuery query;
    query.prepare("UPDATE transaction SET type_transaction = :type_transaction, date_transaction = TO_DATE(:date_transaction, 'YYYY-MM-DD'), "
                  "mode_paiement = :mode_paiement, etat_paiement = :etat_paiement, montant_total = :montant_total "
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
                  "WHERE id_transaction = :id_transaction");
    query.bindValue(":id_transaction", id_transaction);
    query.bindValue(":type_transaction", type_transaction);
    query.bindValue(":date_transaction", date_transaction);
<<<<<<< HEAD

    query.bindValue(":etat_paiement", etat_paiement);
      query.bindValue(":mode_paiement", mode_paiement);
=======
    query.bindValue(":mode_paiement", mode_paiement);
    query.bindValue(":etat_paiement", etat_paiement);
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    query.bindValue(":montant_total", montant_total);

    return query.exec();
}

<<<<<<< HEAD
double Transaction::calculerSommeDepenses()
{
    QSqlQuery query;
    query.prepare("SELECT SUM(montant_total) FROM transaction WHERE type_transaction = 'depense'");

    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toDouble();
        }
    }
    return 0.0;
}
=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8


