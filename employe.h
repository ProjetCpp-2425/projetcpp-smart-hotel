#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
class Employe
{
public:
    // Constructeurs
    Employe();
    Employe(int id_employe, const QString& nom_employe, const QString& prenom_employe, const QDate& date_embauche, const QString& poste, double salaire, const QString& mdp);

    // Getters
    int getIdEmploye() const;
    QString getNomEmploye() const;
    QString getPrenomEmploye() const;
    QDate getDateEmbauche() const;
    QString getPoste() const;
    double getSalaire() const;
    QString getMdp() const;

    // Setters
    void setIdEmploye(int id_employe);
    void setNomEmploye(const QString& nom_employe);
    void setPrenomEmploye(const QString& prenom_employe);
    void setDateEmbauche(const QDate& date_embauche);
    void setPoste(const QString& poste);
    void setSalaire(double salaire);
    void setMdp(const QString& mdp);

private:
    int id_employe;
    QString nom_employe;
    QString prenom_employe;
    QDate date_embauche;
    QString poste;
    double salaire;
    QString mdp;


    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id_employe, const QString& nom_employe, const QString& prenom_employe, const QDate& date_embauche, const QString& poste, double salaire, const QString& mdp);
    QSqlQueryModel* afficher();
};






#endif // EMPLOYE_H
