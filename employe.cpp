#include "employe.h"

// Constructeurs
Employe::Employe() {}
Employe::Employe(int id, QString nom, QString prenom, QString role, QString sexe, QString poste, QString date_embauche, float salaire)
    : id(id), nom(nom), prenom(prenom), role(role), sexe(sexe), poste(poste), date_embauche(date_embauche), salaire(salaire) {}

// Getters
int Employe::getId() const { return id; }
QString Employe::getNom() const { return nom; }
QString Employe::getPrenom() const { return prenom; }
QString Employe::getRole() const { return role; }
QString Employe::getSexe() const { return sexe; }
QString Employe::getPoste() const { return poste; }
QString Employe::getDateEmbauche() const { return date_embauche; }
float Employe::getSalaire() const { return salaire; }

// Méthode pour ajouter un employé
bool Employe::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO employe (id, nom, prenom, role, sexe, poste, date_embauche, salaire)"
                  "VALUES (:id, :nom, :prenom, :role, :sexe, :poste, :date_embauche, :salaire)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":role", role);
    query.bindValue(":sexe", sexe);
    query.bindValue(":poste", poste);
    query.bindValue(":date_embauche", date_embauche);
    query.bindValue(":salaire", salaire);

    return query.exec();
}

// Méthode pour modifier un employé
bool Employe::modifier(int id) {
    QSqlQuery query;
    query.prepare("UPDATE employe SET NOM=:nom, PRENOM=:prenom, ROLE=:role, SEXE=:sexe, POSTE=:poste, DATE_EMBAUCHE=:date_embauche, SALAIRE=:salaire WHERE ID=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":role", role);
    query.bindValue(":sexe", sexe);
    query.bindValue(":poste", poste);
    query.bindValue(":date_embauche", date_embauche);
    query.bindValue(":salaire", salaire);

    return query.exec();
}

// Méthode pour supprimer un employé
bool Employe::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM employe WHERE ID = :id");
    query.bindValue(":id", id);

    return query.exec();
}

// Méthode pour afficher les employés
QSqlQueryModel* Employe::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employe");
    return model;
}
