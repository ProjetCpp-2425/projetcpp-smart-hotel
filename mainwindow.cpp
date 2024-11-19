#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMap>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    afficherClients();
    connect(ui->trierCinButton, &QPushButton::clicked, this, &MainWindow::on_trierCinButton_clicked);
    connect(ui->chercherButton, &QPushButton::clicked, this, &MainWindow::on_chercherButton_clicked);
    connect(ui->statistiquesButton, &QPushButton::clicked, this, &MainWindow::afficherStatistiques);
    connect(ui->exportCsvButton, &QPushButton::clicked, this, &MainWindow::exporterEnCsv);
    connect(ui->arriveeButton, &QPushButton::clicked, this, &MainWindow::enregistrerArrivee);
    connect(ui->historiqueButton, &QPushButton::clicked, this, &MainWindow::afficherHistorique);
    connect(ui->modifierButton, &QPushButton::clicked, this, &MainWindow::on_modifierButton_clicked);
    connect(ui->chatbutton, &QPushButton::clicked, this, &MainWindow::on_chatbutton_clicked);


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

    // Contrôle des champs vides
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isEmpty() || email.isEmpty() || telephone.isEmpty() || cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs sont obligatoires.");
        return;
    }

    // Contrôle du format du mail
    if (!email.contains("@") || !email.contains(".")) {
        QMessageBox::warning(this, "Erreur", "L'email doit contenir un '@' et un '.'");
        return;
    }

    // Contrôle du format du téléphone (doit être un nombre de 8 chiffres)
    bool ok;
    if (telephone.length() != 8 || telephone.toInt(&ok) == 0 || !ok) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit être composé de 8 chiffres.");
        return;
    }

    // Si tout est valide, ajouter le client
    Client client(nom, prenom, dateNaissance, email, telephone, cin);
    clients.append(client);
    afficherClients();

    // Afficher un message de confirmation
    QMessageBox::information(this, "Succès", "Client ajouté avec succès.");
}


void MainWindow::on_chercherButton_clicked()
{
    // Récupérer le numéro de téléphone saisi
    QString telephone = ui->telephoneSearchLineEdit->text();

    // Vérifier si le champ est vide
    if (telephone.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un numéro de téléphone pour effectuer la recherche.");
        return;
    }

    // Chercher le client avec ce numéro de téléphone
    bool clientTrouve = false;
    for (const Client &client : clients) {
        if (client.getTelephone() == telephone) {
            // Afficher les informations du client trouvé
            QMessageBox::information(
                this,
                "Client trouvé",
                QString("Nom : %1\nPrénom : %2\nDate de Naissance : %3\nEmail : %4\nTéléphone : %5\nCIN : %6")
                    .arg(client.getNom())
                    .arg(client.getPrenom())
                    .arg(client.getDateNaissance())
                    .arg(client.getEmail())
                    .arg(client.getTelephone())
                    .arg(client.getCin())
            );
            clientTrouve = true;
            break;
        }
    }

    // Si aucun client n'a été trouvé
    if (!clientTrouve) {
        QMessageBox::warning(this, "Erreur", "Aucun client avec ce numéro de téléphone n'a été trouvé.");
    }
}


void MainWindow::on_modifierButton_clicked()
{
    // Récupérer le CIN du champ de texte
    QString cin = ui->modif->text().trimmed();  // Le champ modif contient le CIN à rechercher

    // Vérifier si le champ CIN est vide
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un numéro de CIN pour modifier le client.");
        return;
    }

    // Trouver l'index du client avec ce CIN
    int index = trouverClient(cin);

    // Vérifier si le client existe
    if (index == -1) {
        QMessageBox::warning(this, "Erreur", "Client avec le CIN spécifié introuvable.");
        return;
    }

    // Récupérer les nouvelles informations depuis les champs de texte
    QString nom = ui->nomLineEdit->text();
    QString prenom = ui->prenomLineEdit->text();
    QString dateNaissance = ui->dateNaissanceLineEdit->text();  // Assurez-vous que ce champ est rempli
    QString email = ui->emailLineEdit->text();
    QString telephone = ui->telephoneLineEdit->text();

    // Vérifier que les champs ne sont pas vides
    if (nom.isEmpty() || prenom.isEmpty() || dateNaissance.isEmpty() || email.isEmpty() || telephone.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis pour modifier le client.");
        return;
    }

    // Mettre à jour les informations du client
    Client &client = clients[index];  // Accéder directement à l'objet Client par référence
    client.setNom(nom);               // Mettre à jour le nom
    client.setPrenom(prenom);         // Mettre à jour le prénom
    client.setDateNaissance(dateNaissance);  // Mettre à jour la date de naissance
    client.setEmail(email);           // Mettre à jour l'email
    client.setTelephone(telephone);   // Mettre à jour le téléphone

    // Rafraîchir l'affichage des clients dans le tableau
    afficherClients();

    // Afficher un message de confirmation
    QMessageBox::information(this, "Succès", "Les informations du client ont été modifiées avec succès.");
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
void MainWindow::on_trierCinButton_clicked()
{
    if (clients.size() <= 1) {
        QMessageBox::information(this, "Information", "Pas assez de clients pour effectuer un tri.");
        return;
    }

    qDebug() << "Avant tri :";
    for (const auto &client : clients) {
        qDebug() << client.getCin();
    }

    // Validate CIN values
    for (const auto &client : clients) {
        if (client.getCin().toInt() == 0 && client.getCin() != "0") {
            QMessageBox::warning(this, "Erreur", "CIN invalide détecté.");
            return;
        }
    }

    // Sort clients by CIN (numeric comparison)
    std::sort(clients.begin(), clients.end(), [](const Client &a, const Client &b) {
        return a.getCin().toInt() < b.getCin().toInt();
    });

    qDebug() << "Après tri :";
    for (const auto &client : clients) {
        qDebug() << client.getCin();
    }

    // Refresh the table widget
    afficherClients();

    // Confirmation message
    QMessageBox::information(this, "Tri effectué", "Les clients ont été triés par CIN.");
}
void MainWindow::on_chatbutton_clicked()
{
    // Créer un modèle pour afficher les messages
    QStandardItemModel *conversationModel = new QStandardItemModel(10, 1, this);
    conversationModel->setHorizontalHeaderLabels({"Messages"});

    // Champs d'entrée pour les informations utilisateur
    QLineEdit *cinField = new QLineEdit(this);
    QLineEdit *nomField = new QLineEdit(this);
    QLineEdit *prenomField = new QLineEdit(this);
    QLineEdit *telephoneField = new QLineEdit(this);
    QLineEdit *emailField = new QLineEdit(this);
    QLineEdit *confirmationField = new QLineEdit(this);
    confirmationField->setReadOnly(true);

    // Placeholder pour aider l'utilisateur
    cinField->setPlaceholderText("Entrez votre CIN");
    nomField->setPlaceholderText("Entrez votre nom");
    prenomField->setPlaceholderText("Entrez votre prénom");
    telephoneField->setPlaceholderText("Entrez votre numéro de téléphone");
    emailField->setPlaceholderText("Entrez votre email");

    // Afficher le premier message
    conversationModel->setItem(0, 0, new QStandardItem("Bienvenue ! Veuillez entrer votre CIN :"));
    ui->chatbotview->setModel(conversationModel);
    ui->chatbotview->resizeColumnsToContents();

    // Ajouter le champ pour entrer le CIN
    ui->chatbotview->setIndexWidget(conversationModel->index(1, 0), cinField);

    connect(cinField, &QLineEdit::editingFinished, this, [=]() {
        QString cin = cinField->text();
        if (cin.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le champ CIN est vide.");
            return;
        }

        // Afficher le CIN saisi
        conversationModel->setItem(1, 0, new QStandardItem("CIN : " + cin));
        ui->chatbotview->setModel(conversationModel);
        ui->chatbotview->resizeColumnsToContents();
        cinField->hide();

        // Demander le nom
        conversationModel->setItem(2, 0, new QStandardItem("Veuillez entrer votre nom :"));
        ui->chatbotview->setIndexWidget(conversationModel->index(3, 0), nomField);
        nomField->show();
    });

    connect(nomField, &QLineEdit::editingFinished, this, [=]() {
        QString nom = nomField->text();
        if (nom.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le champ nom est vide.");
            return;
        }

        // Afficher le nom saisi
        conversationModel->setItem(3, 0, new QStandardItem("Nom : " + nom));
        ui->chatbotview->setModel(conversationModel);
        ui->chatbotview->resizeColumnsToContents();
        nomField->hide();

        // Demander le prénom
        conversationModel->setItem(4, 0, new QStandardItem("Veuillez entrer votre prénom :"));
        ui->chatbotview->setIndexWidget(conversationModel->index(5, 0), prenomField);
        prenomField->show();
    });

    connect(prenomField, &QLineEdit::editingFinished, this, [=]() {
        QString prenom = prenomField->text();
        if (prenom.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le champ prénom est vide.");
            return;
        }

        // Afficher le prénom saisi
        conversationModel->setItem(5, 0, new QStandardItem("Prénom : " + prenom));
        ui->chatbotview->setModel(conversationModel);
        ui->chatbotview->resizeColumnsToContents();
        prenomField->hide();

        // Demander le numéro de téléphone
        conversationModel->setItem(6, 0, new QStandardItem("Veuillez entrer votre numéro de téléphone :"));
        ui->chatbotview->setIndexWidget(conversationModel->index(7, 0), telephoneField);
        telephoneField->show();
    });

    connect(telephoneField, &QLineEdit::editingFinished, this, [=]() {
        QString telephone = telephoneField->text();
        if (telephone.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le champ numéro de téléphone est vide.");
            return;
        }

        // Vérifier le format du numéro
        bool ok;
        if (telephone.length() != 8 || telephone.toInt(&ok) == 0 || !ok) {
            QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit être composé de 8 chiffres.");
            return;
        }

        // Afficher le numéro de téléphone saisi
        conversationModel->setItem(7, 0, new QStandardItem("Téléphone : " + telephone));
        ui->chatbotview->setModel(conversationModel);
        ui->chatbotview->resizeColumnsToContents();
        telephoneField->hide();

        // Demander l'email
        conversationModel->setItem(8, 0, new QStandardItem("Veuillez entrer votre email :"));
        ui->chatbotview->setIndexWidget(conversationModel->index(9, 0), emailField);
        emailField->show();
    });

    connect(emailField, &QLineEdit::editingFinished, this, [=]() {
        QString email = emailField->text();
        if (email.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le champ email est vide.");
            return;
        }

        // Vérifier le format de l'email
        if (!email.contains("@") || !email.contains(".")) {
            QMessageBox::warning(this, "Erreur", "L'email doit contenir un '@' et un '.'");
            return;
        }

        // Afficher l'email saisi
        conversationModel->setItem(9, 0, new QStandardItem("Email : " + email));
        ui->chatbotview->setModel(conversationModel);
        ui->chatbotview->resizeColumnsToContents();
        emailField->hide();

        // Afficher le message de confirmation
        QMessageBox::information(this, "Confirmation", "Vos informations ont été enregistrées avec succès !");
    });
}



void MainWindow::exporterEnCsv()
{
    // Ouvrir un dialogue pour sélectionner le fichier
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter vers Excel", "", "Fichiers CSV (*.csv)");

    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun fichier sélectionné.");
        return;
    }

    // Ajouter l'extension .csv si nécessaire
    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        fileName += ".csv";
    }

    // Ouvrir le fichier en mode écriture
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier pour écriture.");
        return;
    }

    QTextStream stream(&file);

    // Écrire les en-têtes de colonnes
    stream << "Nom,Prenom,CIN,Adresse Email,Telephone,Date de Naissance\n";

    // Écrire les données des clients
    for (const Client &client : clients) {
        stream << client.getNom() << ','
               << client.getPrenom() << ','
               << client.getCin() << ','
               << client.getEmail() << ','
               << client.getTelephone() << ','
               << client.getDateNaissance() << '\n';
    }

    file.close();

    QMessageBox::information(this, "Succès", "Les données ont été exportées avec succès dans le fichier : " + fileName);
}


int MainWindow::trouverClient(const QString &value) {
    for (int i = 0; i < clients.size(); ++i) {
        if (clients[i].getCin() == value) {
            return i;
        }
    }
    return -1;
}
void MainWindow::enregistrerArrivee()
{
    QString cin = ui->cinArriveeLineEdit->text().trimmed();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un numéro de CIN.");
        return;
    }

    // Vérifier si le client existe
    int index = trouverClient(cin);
    if (index == -1) {
        QMessageBox::warning(this, "Erreur", "Le client avec ce CIN n'existe pas dans la base.");
        return;
    }

    // Ajouter une visite pour ce CIN
    historiqueClients[cin]++;

    // Vérifier si le client a droit à une réduction
    if (historiqueClients[cin] >= 3) {
        QMessageBox::information(
            this,
            "Fidélité",
            QString("Ce client a visité %1 fois. Une réduction lui est offerte !").arg(historiqueClients[cin])
        );
    } else {
        QMessageBox::information(
            this,
            "Visite enregistrée",
            QString("Visite enregistrée. Ce client a visité %1 fois.").arg(historiqueClients[cin])
        );
    }
}


void MainWindow::afficherHistorique()
{
    if (historiqueClients.isEmpty()) {
        QMessageBox::information(this, "Historique vide", "Aucun client n'a été enregistré.");
        return;
    }

    QString resultat = "Historique des visites des clients :\n\n";

    for (auto it = historiqueClients.begin(); it != historiqueClients.end(); ++it) {
        resultat += QString("CIN : %1 - Nombre de visites : %2\n").arg(it.key()).arg(it.value());
    }

    QMessageBox::information(this, "Historique des clients", resultat);
}



void MainWindow::afficherStatistiques()
{
    // Utiliser un QMap pour compter les occurrences par numéro de téléphone
    QMap<QString, int> statistiques;

    // Parcourir la liste des clients et compter les téléphones
    for (const Client &client : clients) {
        QString telephone = client.getTelephone();
        statistiques[telephone]++; // Incrémenter le compteur pour ce téléphone
    }

    // Construire une chaîne de caractères pour afficher les résultats
    QString resultat = "Statistiques des téléphones :\n\n";
    for (auto it = statistiques.begin(); it != statistiques.end(); ++it) {
        resultat += QString("Téléphone: %1 - Nombre de clients: %2\n").arg(it.key()).arg(it.value());
    }

    // Afficher les statistiques dans une boîte de message
    QMessageBox::information(this, "Statistiques des téléphones", resultat);






}



