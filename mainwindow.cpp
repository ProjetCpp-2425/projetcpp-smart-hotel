#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cleaning(new Cleaning())
{
    ui->setupUi(this);

    connect(ui->ajouterButton, &QPushButton::clicked, this, &MainWindow::ajouterCleaning);
    connect(ui->modifierButton, &QPushButton::clicked, this, &MainWindow::modifierCleaning);
    connect(ui->supprimerButton, &QPushButton::clicked, this, &MainWindow::supprimerCleaning);
}

MainWindow::~MainWindow() {
    delete ui;
    delete cleaning;
}

void MainWindow::ajouterCleaning() {
    cleaning->setNomClient(ui->nomClientEdit->text());
    cleaning->setIdChambre(ui->idChambreEdit->text().toInt());
    cleaning->setDate(ui->dateEdit->date());

    // Fréquence de nettoyage
    if (ui->radioQuotidien->isChecked()) {
        cleaning->setFrequence("Quotidien");
    } else if (ui->radioHebdomadaire->isChecked()) {
        cleaning->setFrequence("Hebdomadaire");
    } else if (ui->radioMensuel->isChecked()) {
        cleaning->setFrequence("Mensuel");
    }

    // Services demandés
    QStringList services;
    if (ui->checkDraps->isChecked()) services << "Draps";
    if (ui->checkServiettes->isChecked()) services << "Serviettes";
    if (ui->checkSol->isChecked()) services << "Nettoyage du sol";
    if (ui->checkProduits->isChecked()) services << "Produits de bain";
    cleaning->setServices(services);

    // Heure de passage préférée
    QStringList heures;
    if (ui->checkMatin->isChecked()) heures << "Matin";
    if (ui->checkMidi->isChecked()) heures << "Midi";
    if (ui->checkApresMidi->isChecked()) heures << "Après-midi";
    if (ui->checkSoir->isChecked()) heures << "Soir";
    cleaning->setHeurePreferee(heures);

}

void MainWindow::modifierCleaning() {
    int idChambre = ui->idChambreEdit->text().toInt();
    cleaning->setNomClient(ui->nomClientEdit->text());

    if (cleaning->modifier(idChambre)) {
        QMessageBox::information(this, "Succès", "Nettoyage modifié avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du nettoyage.");
    }
}

void MainWindow::supprimerCleaning() {
    int idChambre = ui->idChambreEdit->text().toInt();

    if (cleaning->supprimer(idChambre)) {
        QMessageBox::information(this, "Succès", "Nettoyage supprimé avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du nettoyage.");
    }
}
