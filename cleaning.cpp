#include "cleaning.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

Cleaning::Cleaning() : nomClient(""), idChambre(0), date(QDate::currentDate()), chambreDisponible(false) {}

QString Cleaning::getNomClient() const { return nomClient; }
void Cleaning::setNomClient(const QString &nom) { nomClient = nom; }

int Cleaning::getIdChambre() const { return idChambre; }
void Cleaning::setIdChambre(int id) { idChambre = id; }

QDate Cleaning::getDate() const { return date; }
void Cleaning::setDate(const QDate &d) { date = d; }

QString Cleaning::getFrequence() const { return frequence; }
void Cleaning::setFrequence(const QString &freq) { frequence = freq; }

QStringList Cleaning::getServices() const { return services; }
void Cleaning::setServices(const QStringList &srv) { services = srv; }

QStringList Cleaning::getHeurePreferee() const { return heurePreferee; }
void Cleaning::setHeurePreferee(const QStringList &heures) { heurePreferee = heures; }

bool Cleaning::isChambreDisponible() const { return chambreDisponible; }
void Cleaning::setChambreDisponible(bool disponible) { chambreDisponible = disponible; }

bool Cleaning::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO cleaning (nom_client, id_chambre, date, frequence, services, heure_preferee, disponible) "
                  "VALUES (:nom_client, :id_chambre, :date, :frequence, :services, :heure_preferee, :disponible)");
    query.bindValue(":nom_client", nomClient);
    query.bindValue(":id_chambre", idChambre);
    query.bindValue(":date", date);
    query.bindValue(":frequence", frequence);
    query.bindValue(":services", services.join(","));
    query.bindValue(":heure_preferee", heurePreferee.join(","));
    query.bindValue(":disponible", chambreDisponible);

    if(!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de nettoyage:" << query.lastError();
        return false;
    }
    return true;
}

bool Cleaning::modifier(int idChambre) {
    QSqlQuery query;
    query.prepare("UPDATE cleaning SET nom_client=:nom_client, date=:date, frequence=:frequence, "
                  "services=:services, heure_preferee=:heure_preferee, disponible=:disponible "
                  "WHERE id_chambre=:id_chambre");
    query.bindValue(":nom_client", nomClient);
    query.bindValue(":id_chambre", idChambre);
    query.bindValue(":date", date);
    query.bindValue(":frequence", frequence);
    query.bindValue(":services", services.join(","));
    query.bindValue(":heure_preferee", heurePreferee.join(","));
    query.bindValue(":disponible", chambreDisponible);

    if(!query.exec()) {
        qDebug() << "Erreur lors de la modification de nettoyage:" << query.lastError();
        return false;
    }
    return true;
}

bool Cleaning::supprimer(int idChambre) {
    QSqlQuery query;
    query.prepare("DELETE FROM cleaning WHERE id_chambre = :id_chambre");
    query.bindValue(":id_chambre", idChambre);

    if(!query.exec()) {
        qDebug() << "Erreur lors de la suppression de nettoyage:" << query.lastError();
        return false;
    }
    return true;
}
