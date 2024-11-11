#ifndef CHAMBRE_H
#define CHAMBRE_H
#include <QSqlQuery>
#include <QSqlQueryModel>

class Chambre
{
    int Id_chambre;
    QString categorie, type, statut_chambre;
    float tarif;

public:
    Chambre() {}
    Chambre(int, QString, QString, QString, float);

    int getIdChambre() { return Id_chambre; }
    QString getCategorie() { return categorie; }
    QString getType() { return type; }
    QString getStatutChambre() { return statut_chambre; }
    float getTarif() { return tarif; }

    void setIdChambre(int id) { Id_chambre = id; }
    void setCategorie(QString c) { categorie = c; }
    void setType(QString t) { type = t; }
    void setStatutChambre(QString s) { statut_chambre = s; }
    void setTarif(float tarif) { this->tarif = tarif; }

    bool ajouter();
    bool modifier(int Id_chambre, QString categorie, QString type, QString statut_chambre, float tarif);
    QSqlQueryModel* afficher();
    bool supprimer(int);


};

#endif // CHAMBRE_H
