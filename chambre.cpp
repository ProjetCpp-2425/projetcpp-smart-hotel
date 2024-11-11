#include "chambre.h"
#include <QDebug>
#include <QSqlError>

Chambre::Chambre(int Id_chambre, QString categorie, QString type, QString statut_chambre, float tarif)
{
    this->Id_chambre = Id_chambre;
    this->categorie = categorie;
    this->type = type;
    this->statut_chambre = statut_chambre;
    this->tarif = tarif;
    qDebug() << "Chambre object created with ID:" << Id_chambre
             << ", Categorie:" << categorie << ", Type:" << type
             << ", Statut:" << statut_chambre << ", Tarif:" << tarif;
}

bool Chambre::ajouter()
{
    QSqlQuery query;

    QString idStr = QString::number(Id_chambre);
    QString tarifStr = QString::number(tarif);
    QString categorieStr = categorie;
    QString typeStr = type;
    QString statutStr = statut_chambre;

    query.prepare("INSERT INTO chambre (Id_chambre, categorie, type, statut_chambre, tarif) "
                  "VALUES (:Id_chambre, :categorie, :type, :statut_chambre, :tarif)");

    // Bind values to the placeholders
    query.bindValue(":Id_chambre", idStr);
    query.bindValue(":categorie", categorieStr);
    query.bindValue(":type", typeStr);
    query.bindValue(":statut_chambre", statutStr);
    query.bindValue(":tarif", tarifStr);

    bool success = query.exec();

    if (success) {
        qDebug() << "Successfully added room with ID:" << Id_chambre;
    } else {
        qDebug() << "Failed to add room with ID:" << Id_chambre;
        qDebug() << "SQL Error:" << query.lastError().text();
    }

    return success;
}


bool Chambre::modifier(int Id_chambre, QString categorie, QString type, QString statut_chambre, float tarif)
{
    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM chambre WHERE Id_chambre = :Id_chambre");
    query.bindValue(":Id_chambre", Id_chambre);
    if (!query.exec()) {
        qDebug() << "Error checking room existence:" << query.lastError().text();
        return false;
    }

    query.next();
    if (query.value(0).toInt() == 0) {
        qDebug() << "Room with ID" << Id_chambre << "does not exist.";
        return false;
    }

    query.prepare("UPDATE chambre SET categorie = :categorie, type = :type, statut_chambre = :statut_chambre, tarif = :tarif "
                  "WHERE Id_chambre = :Id_chambre");
    query.bindValue(":Id_chambre", Id_chambre);
    query.bindValue(":categorie", categorie);
    query.bindValue(":type", type);
    query.bindValue(":statut_chambre", statut_chambre);
    query.bindValue(":tarif", tarif);

    bool success = query.exec();
    if (success) {
        qDebug() << "Room with ID" << Id_chambre << "successfully modified.";
    } else {
        qDebug() << "Failed to modify room with ID" << Id_chambre;
        qDebug() << "Error:" << query.lastError().text();
    }

    return success;
}

bool Chambre::supprimer(int id_chambre)
{
    QSqlQuery query;
    query.prepare("DELETE FROM chambre WHERE Id_chambre = :id_chambre");
    query.bindValue(":id_chambre", id_chambre);

    bool success = query.exec();

    if (success) {
        if (query.numRowsAffected() > 0) {
            qDebug() << "Successfully deleted room with ID:" << id_chambre;
            return true;
        } else {
            qDebug() << "No room found with ID:" << id_chambre;
            return false;
        }
    } else {
        qDebug() << "Failed to delete room with ID:" << id_chambre;
        qDebug() << "SQL Error:" << query.lastError().text();
        return false;
    }
}
QSqlQueryModel* Chambre::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT Id_chambre, categorie, type, statut_chambre, tarif FROM chambre");

    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        delete model;
        return nullptr;
    }

    model->setQuery(query);
    return model;
}

