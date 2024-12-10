#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Employe {
    int cin;
    QString nom;
    QString prenom;
    QString question;
    QString sexe;
    QString poste;
    QDate date_embauche;
    float salaire;
    QString mot_de_passe;

public:
    Employe();  // Default constructor
    Employe(int cin, QString nom, QString prenom, QString question, QString sexe, QString poste, QDate date_embauche, float salaire, QString mot_de_passe);

    // Getters
    int getCin() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getQuestion() const;
    QString getSexe() const;
    QString getPoste() const;
    QDate getDate_embauche() const;
    float getSalaire() const;
    QString getMot_de_passe() const;

    // Setters
    void setCin(int cin);
    void setNom(QString nom);
    void setPrenom(QString prenom);
    void setQuestion(QString role);
    void setSexe(QString sexe);
    void setPoste(QString poste);
    void setDate_embauche(QDate date_embauche);
    void setSalaire(float salaire);
    void setMot_de_passe(QString mot_de_passe);

    std::map<QString, int> statNbrPerType();

    // Database methods
    bool ajouter();
    bool modifier(int cin);
    bool supprimer(int cin);
 QSqlQueryModel* afficher() ;
    QSqlQueryModel* rechercheByCin(int cin);





};

#endif // EMPLOYE_H
