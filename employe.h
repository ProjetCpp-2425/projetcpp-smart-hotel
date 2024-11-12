#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Employe {
private:
    int id;
    QString nom;
    QString prenom;
    QString role;
    QString sexe;
    QString poste;
    QString date_embauche;
    float salaire;

public:
    Employe();  // Constructeur par défaut
    Employe(int id, QString nom, QString prenom, QString role, QString sexe, QString poste, QString date_embauche, float salaire);

    // Getters et setters
    int getId() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getRole() const;
    QString getSexe() const;
    QString getPoste() const;
    QString getDateEmbauche() const;
    float getSalaire() const;

    bool ajouter();
    bool modifier(int id);
    bool supprimer(int id);
    QSqlQueryModel *afficher();
};

#endif // EMPLOYE_H
