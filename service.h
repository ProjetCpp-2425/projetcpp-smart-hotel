#ifndef SERVICE_H
#define SERVICE_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>


class Service
{
    int id_service;
    QString nom_service;
    QString statut_service;
    float tarif;

public:
    Service() {}
    Service(int, QString, QString, float);

    int getIdService() { return id_service; }
    QString getNomService() { return nom_service; }
    QString getStatutService() { return statut_service; }
    float getTarif() { return tarif; }

    void setIdService(int id) { id_service = id; }
    void setNomService(QString nom) { nom_service = nom; }
    void setStatutService(QString statut) { statut_service = statut; }
    void setTarif(float tarif) { this->tarif = tarif; }

    bool ajouter();
    bool modifier(int id_service, QString nom_service, QString statut_service, float tarif);
    bool supprimer(int);
};


#endif // SERVICE_H
