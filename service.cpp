#include "service.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

Service::Service()
    : id_service(0), service_demande(""), numero(0), tarif(0.0f) {}

Service::Service(int id_service, const QString &service_demande, int numero, const QDate &datee, float tarif)
    : id_service(id_service), service_demande(service_demande), numero(numero), datee(datee), tarif(tarif) {}

bool Service::ajouter() {
    QSqlQuery query;

    // Prepare the query to insert into the "services" table
    query.prepare("INSERT INTO services (ID_SERVICE, SERVICE_DEMANDE, NUMERO, DATEE, TARIF) "
                  "VALUES (:id_service, :service_demande, :numero, :datee, :tarif)");

    // Bind the values to the placeholders
    query.bindValue(":id_service", id_service);         // ID of the service
    query.bindValue(":service_demande", service_demande); // Type of service requested
    query.bindValue(":numero", numero);                 // Identifier or phone number
    query.bindValue(":datee", datee);                   // Date of the service
    query.bindValue(":tarif", tarif);                   // Cost of the service

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "Error adding service:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Service::modifier(int id_service, const QString &service_demande, int numero, const QDate &datee, float tarif) {
    QSqlQuery query;
    query.prepare("UPDATE services SET SERVICE_DEMANDE = :service_demande, NUMERO = :numero, DATEE = :datee, TARIF = :tarif "
                  "WHERE ID_SERVICE = :id_service");

    query.bindValue(":id_service", id_service);
    query.bindValue(":service_demande", service_demande);
    query.bindValue(":numero", numero);
    query.bindValue(":datee", datee);
    query.bindValue(":tarif", tarif);

    if (!query.exec()) {
        qDebug() << "Error modifying service:" << query.lastError().text();
        return false;
    }

    return true;
}
QSqlQueryModel* Service::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel;

    // Requête SQL pour récupérer les services
    QSqlQuery query("SELECT ID_SERVICE, SERVICE_DEMANDE, NUMERO, DATEE, TARIF FROM services");

    // Exécuter la requête et vérifier s'il y a une erreur
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
        return nullptr;
    }

    // Définir le modèle avec les résultats de la requête
    model->setQuery(query);

    return model;
}

// Getters and setters
int Service::getIdService() const { return id_service; }
void Service::setIdService(int id_service) { this->id_service = id_service; }

QString Service::getServiceDemande() const { return service_demande; }
void Service::setServiceDemande(const QString &service_demande) { this->service_demande = service_demande; }

int Service::getNumero() const { return numero; }
void Service::setNumero(int numero) { this->numero = numero; }

QDate Service::getDatee() const { return datee; }
void Service::setDatee(const QDate &datee) { this->datee = datee; }

float Service::getTarif() const { return tarif; }
void Service::setTarif(float tarif) { this->tarif = tarif; }
