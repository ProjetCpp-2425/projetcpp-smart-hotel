#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_ajouter, &QPushButton::clicked, this, &MainWindow::on_button_ajouter_clicked);
    connect(ui->pushButton_modifier, &QPushButton::clicked, this, &MainWindow::on_button_modifier_clicked);
    connect(ui->pushButton_supprimer, &QPushButton::clicked, this, &MainWindow::on_button_supprimer_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Ajouter un employé
void MainWindow::on_button_ajouter_clicked() {
    int id = ui->lineEdit_id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString role = ui->lineEdit_role->text();
    QString sexe = ui->lineEdit_sexe->text();
    QString poste = ui->comboBox_poste->currentText();
    QString date_embauche = ui->dateEdit_date_embauche->text();
    float salaire = ui->lineEdit_salaire->text().toFloat();

    Employe emp(id, nom, prenom, role, sexe, poste, date_embauche, salaire);
    if (emp.ajouter()) {
        QMessageBox::information(this, "Ajout", "Employé ajouté avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout de l'employé.");
    }
}

// Modifier un employé
void MainWindow::on_button_modifier_clicked() {
    int id = ui->lineEdit_id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString role = ui->lineEdit_role->text();
    QString sexe = ui->lineEdit_sexe->text();
    QString poste = ui->comboBox_poste->currentText();
    QString date_embauche = ui->dateEdit_date_embauche->text();
    float salaire = ui->lineEdit_salaire->text().toFloat();

    Employe emp(id, nom, prenom, role, sexe, poste, date_embauche, salaire);
    if (emp.modifier(id)) {
        QMessageBox::information(this, "Modification", "Employé modifié avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de l'employé.");
    }
}

// Supprimer un employé
void MainWindow::on_button_supprimer_clicked() {
    int id = ui->lineEdit_id->text().toInt();
    if (employe.supprimer(id)) {
        QMessageBox::information(this, "Suppression", "Employé supprimé avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression de l'employé.");
    }
}

// Afficher les employés
void MainWindow::on_button_afficher_clicked() {
    ui->tableView->setModel(employe.afficher());
}
