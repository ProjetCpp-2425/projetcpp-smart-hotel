#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

class Service {
public:
    Service();
    Service(int id_service, const QString &service_demande, int numero, const QDate &datee, float tarif);

    // CRUD Operations
    bool ajouter();
    bool supprimer(int id_service);
    bool modifier(int id_service, const QString &service_demande, int numero, const QDate &datee, float tarif);

    QSqlQueryModel *afficher();
    QSqlQueryModel *rechercher(const QString &critere);

    // Getters and Setters
    int getIdService() const;
    void setIdService(int id_service);

    QString getServiceDemande() const;
    void setServiceDemande(const QString &service_demande);

    int getNumero() const;
    void setNumero(int numero);

    QDate getDatee() const;
    void setDatee(const QDate &datee);

    float getTarif() const;
    void setTarif(float tarif);

private:
    int id_service;
    QString service_demande;
    int numero;
    QDate datee;
    float tarif;
};

#endif // SERVICE_H
