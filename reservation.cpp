#include "reservation.h"


Reservation::Reservation()
{
}

Reservation::Reservation(int num_reservation, QDate date_arrivee, QDate date_depart, QString statut_reservation, double montant, int cin, QString type)
    : num_reservation(num_reservation), date_arrivee(date_arrivee), date_depart(date_depart), statut_reservation(statut_reservation), montant(montant), cin(cin), type(type)
{
}


int Reservation::getNumReservation()  {
    return num_reservation;
}

QDate Reservation::getDateArrivee()  {
    return date_arrivee;
}

QDate Reservation::getDateDepart()  {
    return date_depart;
}

QString Reservation::getStatutReservation()  {
    return statut_reservation;
}
QString Reservation::getType()  {
    return type;
}

double Reservation::getMontant()  {
    return montant;
}

int Reservation::getCin()  {
    return cin;
}


void Reservation::setNumReservation(int num) {
    num_reservation = num;
}

void Reservation::setDateArrivee( QDate& date) {
    date_arrivee = date;
}

void Reservation::setDateDepart( QDate& date) {
    date_depart = date;
}

void Reservation::setStatutReservation( QString& statut) {
    statut_reservation = statut;
}

void Reservation::setType( QString& type) {
    this->type = type;
}

void Reservation::setMontant(float montant) {
    this->montant = montant;
}

void Reservation::setCin(int cin) {
    this->cin = cin;
}


QSqlQueryModel* Reservation::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(R"(
        SELECT num_reservation, date_arrivee, date_depart, statut_reservation, montant, cin, type
        FROM reservation
    )");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Num Reservation"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Arrivée"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Départ"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut Reservation"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Montant"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));

    return model;
}


Reservation Reservation::recherche(int id)
{
    QSqlQuery query;
    query.prepare("SELECT num_reservation, date_arrivee, date_depart, statut_reservation, montant, cin, type "
                  "FROM reservation WHERE num_reservation = :id");
    query.bindValue(":id", id);

    if (query.exec() == false || query.next() == false) {
        return Reservation();
    }

    int numReservation = query.value("num_reservation").toInt();
    QDate dateArrivee = query.value("date_arrivee").toDate();
    QDate dateDepart = query.value("date_depart").toDate();
    QString statutReservation = query.value("statut_reservation").toString();
    float montant = query.value("montant").toFloat();
    int cin = query.value("cin").toInt();
    QString type = query.value("type").toString();

    Reservation tmp(numReservation, dateArrivee, dateDepart, statutReservation, montant, cin,type);

    return tmp;
}

QSqlQueryModel* Reservation::   rechercher(QString donnee) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare(
           "SELECT NUM_RESERVATION, DATE_ARRIVEE, DATE_DEPART, STATUT_RESERVATION, MONTANT, CIN, TYPE "
           "FROM RESERVATION "
           "WHERE NUM_RESERVATION LIKE :donnee OR "
           "DATE_ARRIVEE LIKE :donnee OR "
           "DATE_DEPART LIKE :donnee OR "
           "STATUT_RESERVATION LIKE :donnee OR "
           "MONTANT LIKE :donnee OR "
           "CIN LIKE :donnee OR "
           "TYPE LIKE :donnee"
       );

    // Bind the search criteria with wildcards for a flexible search
    query.bindValue(":donnee", "%" + donnee + "%");

    // Execute the query and set the model
    if (query.exec()) {
        model->setQuery(query);
        // Set headers for each column
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Num Reservation"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Arrivee"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Depart"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut Reservation"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Montant"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("CIN"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));
        return model;
    }

    // Clean up if the query fails
    delete model;
    return nullptr;
}



QSqlQueryModel* Reservation::afficherAvecTri(const QString &critereDeTri){

    QSqlQueryModel *model = new QSqlQueryModel();

    if(critereDeTri == "NumReservationASC"){
        model->setQuery(R"(
            SELECT num_reservation, date_arrivee, date_depart, statut_reservation, montant, cin, type
            FROM reservation ORDER BY num_reservation ASC
        )");
    }
    else if(critereDeTri == "NumReservationDESC"){
        model->setQuery(R"(
            SELECT num_reservation, date_arrivee, date_depart, statut_reservation, montant, cin, type
            FROM reservation ORDER BY num_reservation DESC
        )");
    }
    else if (critereDeTri == "DateReservationASC"){
        model->setQuery(R"(
            SELECT num_reservation, date_arrivee, date_depart, statut_reservation, montant, cin, type
            FROM reservation ORDER BY date_depart ASC
        )");
    }
    else if (critereDeTri == "DateReservationDESC"){
        model->setQuery(R"(
            SELECT num_reservation, date_arrivee, date_depart, statut_reservation, montant, cin, type
            FROM reservation ORDER BY date_depart DESC
        )");
    }
    else{
        model->setQuery(R"(
            SELECT num_reservation, date_arrivee, date_depart, statut_reservation, montant, cin, type
            FROM reservation
        )");
    }




    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Num Reservation"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Arrivée"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Départ"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut Reservation"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Montant"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));

    return model;

}






