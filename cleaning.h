#ifndef CLEANING_H
#define CLEANING_H

#include <QString>
#include <QDate>
#include <QStringList>
#include <QSqlDatabase>

class Cleaning {
public:
    Cleaning();

    // Getters et Setters
    QString getNomClient() const;
    void setNomClient(const QString &nom);

    int getIdChambre() const;
    void setIdChambre(int id);

    QDate getDate() const;
    void setDate(const QDate &date);

    QString getFrequence() const;
    void setFrequence(const QString &frequence);

    QStringList getServices() const;
    void setServices(const QStringList &services);

    QStringList getHeurePreferee() const;
    void setHeurePreferee(const QStringList &heures);

    bool isChambreDisponible() const;
    void setChambreDisponible(bool disponible);

    // Fonctions pour manipuler la base de données
    bool ajouter();
    bool modifier(int idChambre);
    bool supprimer(int idChambre);

private:
    QString nomClient;
    int idChambre;
    QDate date;
    QString frequence;
    QStringList services;
    QStringList heurePreferee;
    bool chambreDisponible;
};

#endif // CLEANING_H

