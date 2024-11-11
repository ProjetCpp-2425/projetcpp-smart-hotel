#ifndef RESERVATION_H
#define RESERVATION_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QObject>
#include <QSqlQuery>

class Reservation {
private:
    int num_reservation;
    QDate date_arrivee;
    QDate date_depart;
    QString statut_reservation;
    float montant;
    int cin;
    QString type;


public:
    Reservation();
    Reservation(int num_reservation, QDate date_arrivee, QDate date_depart, QString statut_reservation, double montant, int cin, QString type);

    int getNumReservation() ;
    QDate getDateArrivee() ;
    QDate getDateDepart() ;
    QString getStatutReservation() ;
    QString getType() ;
    double getMontant() ;
    int getCin() ;

    void setNumReservation(int num);
    void setDateArrivee( QDate& date);
    void setDateDepart( QDate& date);
    void setStatutReservation( QString& statut);
    void setType( QString& type);
    void setMontant(float montant);
    void setCin(int cin);

    QSqlQueryModel* afficher();
    Reservation recherche(int id);
    QSqlQueryModel* rechercher(QString critere);


};

#endif // RESERVATION_H
