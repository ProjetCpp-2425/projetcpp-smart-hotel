#include "employe.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>


// Constructors
Employe::Employe() {}

Employe::Employe(int cin, QString nom, QString prenom, QString question, QString sexe, QString poste, QDate date_embauche, float salaire, QString mot_de_passe)
    : cin(cin), nom(nom), prenom(prenom), question(question), sexe(sexe), poste(poste), date_embauche(date_embauche), salaire(salaire), mot_de_passe(mot_de_passe) {}

// Getters
int Employe::getCin() const { return cin; }
QString Employe::getNom() const { return nom; }
QString Employe::getPrenom() const { return prenom; }
QString Employe::getQuestion() const { return question; }
QString Employe::getSexe() const { return sexe; }
QString Employe::getPoste() const { return poste; }
QDate Employe::getDate_embauche() const  { return date_embauche; }
float Employe::getSalaire() const { return salaire; }
QString Employe::getMot_de_passe() const { return mot_de_passe; }


// Setters
void Employe::setCin(int cin) { this->cin = cin; }
void Employe::setNom(QString nom) { this->nom = nom; }
void Employe::setPrenom(QString prenom) { this->prenom = prenom; }
void Employe::setQuestion(QString question) { this->question = question; }
void Employe::setSexe(QString sexe) { this->sexe = sexe; }
void Employe::setPoste(QString poste) { this->poste = poste; }
void Employe::setDate_embauche(QDate date_embauche) { this->date_embauche = date_embauche; }
void Employe::setSalaire(float salaire) { this->salaire = salaire; }
void Employe::setMot_de_passe(QString mot_de_passe) { this->mot_de_passe = mot_de_passe; }

// Method to add an employee
bool Employe::ajouter() {
    QSqlQuery query;

    // Prepare the SQL query
    bool prepared = query.prepare("INSERT INTO EMPLOYE (cin, nom, prenom, question, sexe, poste, date_embauche, salaire) "
                                  "VALUES (:cin, :nom, :prenom, :question, :sexe, :poste, :date_embauche, :salaire)");

    if (!prepared) {
        qDebug() << "SQL Preparation Error:" << query.lastError().text();
        return false;
    }

    // Bind values to placeholders
    query.bindValue(":cin", this->cin);
    query.bindValue(":nom", this->nom);
    query.bindValue(":prenom", this->prenom);
    query.bindValue(":question", this->question);
    query.bindValue(":sexe", this->sexe);
    query.bindValue(":poste", this->poste);
    query.bindValue(":date_embauche", this->date_embauche);
    query.bindValue(":salaire", this->salaire);

    // Log bound values to help debug parameter mismatch issues
    qDebug() << "Attempting to insert with values:";
    qDebug() << "CIN:" << this->cin;
    qDebug() << "Nom:" << this->nom;
    qDebug() << "Prenom:" << this->prenom;
    qDebug() << "question:" << this->question;
    qDebug() << "Sexe:" << this->sexe;
    qDebug() << "Poste:" << this->poste;
    qDebug() << "date_embauche:" << this->date_embauche;
    qDebug() << "Salaire:" << this->salaire;

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "SQL Execution Error:" << query.lastError().text();
        return false;
    }

    return true;
}


// Method to modify an employee
bool Employe::modifier(int cin) {
    QSqlQuery query;

    query.prepare("UPDATE employe SET nom=:nom, prenom=:prenom, question=:question, sexe=:sexe, poste=:poste, date_embauche=:date_embauche, salaire=:salaire WHERE cin=:cin");
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":question", question);
    query.bindValue(":sexe", sexe);
    query.bindValue(":poste", poste);
    query.bindValue(":date_embauche", date_embauche);
    query.bindValue(":salaire", salaire);

    return query.exec();
}

// Method to delete an employee
bool Employe::supprimer(int cin) {
    QSqlQuery query;

    query.prepare("DELETE FROM employe WHERE cin = :cin");
    query.bindValue(":cin", cin);

    return query.exec();
}

// Method to display employees
QSqlQueryModel* Employe::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employe");
    return model;
}

// In employe.cpp

QSqlQueryModel* Employe::rechercheByCin(int cin) {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Prepare the SQL query to search for an employee by ID
    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE cin = :cin");
    query.bindValue(":cin", cin);

    // Execute the query and check if it was successful
    if (query.exec()) {
        // Pass query by move instead of by copy
        model->setQuery(std::move(query));  // Use std::move here
    } else {
        qDebug() << "Error fetching employee by CIN:" << query.lastError().text();
        delete model;
        model = nullptr;
    }

    return model;
}
