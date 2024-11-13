#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    afficherClients();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouterButton_clicked()
{
    qDebug() << "Nombre de clients à afficher : " << clients.size();

    QString nom = ui->nomLineEdit->text();
    QString prenom = ui->prenomLineEdit->text();
    QString dateNaissance = ui->dateNaissanceLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString telephone = ui->telephoneLineEdit->text();
    QString cin = ui->cinLineEdit->text();

    if (nom.isEmpty() || cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Nom et CIN sont obligatoires");
        return;
    }

    Client client(nom, prenom, dateNaissance, email, telephone, cin);
    clients.append(client);
    afficherClients();
}

void MainWindow::on_modifierButton_clicked()
{
    // Récupérer le CIN du champ de texte
    QString cin = ui->modif->text();

    // Vérifier si le champ CIN est vide
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un numéro de CIN pour modifier le client.");
        return;
    }

    // Afficher le CIN pour vérifier sa récupération
       qDebug() << "CIN recherché pour modification:" << cin;

       // Trouver l'index du client avec ce CIN
       int index = trouverClient(cin);

       // Afficher l'index trouvé pour débogage
       qDebug() << "Index trouvé:" << index;

       // Vérifier si le client existe
       if (index == -1) {
           QMessageBox::warning(this, "Erreur", "Client avec le CIN spécifié introuvable.");
           return;
    }

    // Récupérer les nouvelles informations depuis les champs de texte
    QString nom = ui->nomLineEdit->text();
    QString prenom = ui->prenomLineEdit->text();
    QString dateNaissance = ui->dateNaissanceLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString telephone = ui->telephoneLineEdit->text();

    // Vérifier que les champs ne sont pas vides
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isEmpty() || email.isEmpty() || telephone.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis pour modifier le client.");
        return;
    }

    // Mettre à jour les informations du client
    clients[index].setNom(nom);
    clients[index].setPrenom(prenom);
    clients[index].setDateNaissance(dateNaissance);
    clients[index].setEmail(email);
    clients[index].setTelephone(telephone);

    // Afficher un message de confirmation
    QMessageBox::information(this, "Succès", "Les informations du client ont été modifiées avec succès.");

    // Rafraîchir l'affichage des clients dans le tableau
    afficherClients();
}


void MainWindow::on_supprimerButton_clicked()
{
    QString cin = ui->cin_supp->text();
    int index = trouverClient(cin);

    if (index != -1) {
        clients.remove(index);
        afficherClients();
    } else {
        QMessageBox::warning(this, "Erreur", "Client non trouvé");
    }
}

void MainWindow::afficherClients()
{
    qDebug() << "Nombre de clients dans la liste pour affichage : " << clients.size();

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(clients.size());
    ui->tableWidget->setColumnCount(6);

    QStringList headers = {"Nom", "Prénom", "Date de naissance", "Email", "Téléphone", "CIN"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < clients.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(clients[i].getNom()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(clients[i].getPrenom()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(clients[i].getDateNaissance()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(clients[i].getEmail()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(clients[i].getTelephone()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(clients[i].getCin()));
    }
}


int MainWindow::trouverClient(const QString &cin)
{
    for (int i = 0; i < clients.size(); ++i) {
        if (clients[i].getCin() == cin) {
            return i;
        }
    }
    return -1;
}
