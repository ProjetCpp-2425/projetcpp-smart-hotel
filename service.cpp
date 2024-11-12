#include "service.h"
#include<QString>
#include <QDebug>
#include <QSqlError>


Service::Service(int id_service, QString nom_service, QString statut_service, float tarif)
{
    this->id_service = id_service;
    this->nom_service = nom_service;
    this->statut_service = statut_service;
    this->tarif = tarif;
    qDebug() << "Service object created with ID:" << id_service
             << ", Nom:" << nom_service << ", Statut:" << statut_service
             << ", Tarif:" << tarif;
}

bool Service::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO service (id_service, nom_service, statut_service, tarif) "
                  "VALUES (:id_service, :nom_service, :statut_service, :tarif)");
    query.bindValue(":id_service", id_service);
    query.bindValue(":nom_service", nom_service);
    query.bindValue(":statut_service", statut_service);
    query.bindValue(":tarif", tarif);

    bool success = query.exec();
    if (success) {
        qDebug() << "Successfully added service with ID:" << id_service;
    } else {
        qDebug() << "Failed to add service with ID:" << id_service;
        qDebug() << "SQL Error:" << query.lastError().text();
    }
    return success;
}

bool Service::modifier(int id_service, QString nom_service, QString statut_service, float tarif)
{
    QSqlQuery query;
    query.prepare("UPDATE service SET nom_service = :nom_service, statut_service = :statut_service, tarif = :tarif "
                  "WHERE id_service = :id_service");
    query.bindValue(":id_service", id_service);
    query.bindValue(":nom_service", nom_service);
    query.bindValue(":statut_service", statut_service);
    query.bindValue(":tarif", tarif);

    bool success = query.exec();
    if (success) {
        qDebug() << "Service with ID" << id_service << "successfully modified.";
    } else {
        qDebug() << "Failed to modify service with ID" << id_service;
        qDebug() << "Error:" << query.lastError().text();
    }
    return success;
}

bool Service::supprimer(int id_service)
{
    QSqlQuery query;
    query.prepare("DELETE FROM service WHERE id_service = :id_service");
    query.bindValue(":id_service", id_service);

    bool success = query.exec();
    if (success) {
        if (query.numRowsAffected() > 0) {
            qDebug() << "Successfully deleted service with ID:" << id_service;
            return true;
        } else {
            qDebug() << "No service found with ID:" << id_service;
            return false;
        }
    } else {
        qDebug() << "Failed to delete service with ID:" << id_service;
        qDebug() << "SQL Error:" << query.lastError().text();
        return false;
    }
}
