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
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QMessageBox>
#include "qrcodegen.h"
#include <QVBoxLayout>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QStyleFactory>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    afficherClients();
    connect(ui->afficherStatistiquesButton, &QPushButton::clicked, this, &MainWindow::on_afficherStatistiquesButton_clicked);

    connect(ui->chercherButton, &QPushButton::clicked, this, &MainWindow::on_chercherButton_clicked);
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

    // Vérification de l'unicité du CIN
    for (const Client &client : clients) {
        if (client.getCin() == cin) {
            QMessageBox::warning(this, "Erreur", "Le CIN doit être unique.");
            return;
        }
    }

    // Vérification de l'unicité de l'email
    for (const Client &client : clients) {
        if (client.getEmail() == email) {
            QMessageBox::warning(this, "Erreur", "L'email doit être unique.");
            return;
        }
    }
    for (const Client &client : clients) {
        if (client.getTelephone() == telephone) {
            QMessageBox::warning(this, "Erreur", "Le numero de telephone doit être unique.");
            return;
        }
    }
    // Contrôle du format du téléphone : commence par '+' et contient des chiffres après
    if (!telephone.startsWith("+")) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit commencer par '+'");
        return;
    }

    // Vérification que les caractères après '+' sont bien des chiffres
    QString numeroSansPlus = telephone.mid(1);  // On enlève le '+' pour vérifier le reste
    bool ok;
    numeroSansPlus.toLongLong(&ok);  // Convertir en nombre pour vérifier si c'est bien numérique
    if (!ok || numeroSansPlus.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit être composé uniquement de chiffres après le '+'.");
        return;
    }

    // Contrôle de la longueur du téléphone : exemple de 10 chiffres après le '+'
    if (telephone.length() < 11) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit contenir que 8 chiffres après le '+'");
        return;
    }

    // Si tout est valide, ajouter le client
    Client client(nom, prenom, dateNaissance, email, telephone, cin,0);
    if (client.exists(cin)) {
        // Mise à jour des informations si le client existe
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE client SET NOM = :nom, PRENOM = :prenom, DATENAISSANCE = :dateNaissance, "
                            "EMAIL = :email, TELEPHONE = :telephone WHERE CIN = :cin");
        updateQuery.bindValue(":nom", nom);
        updateQuery.bindValue(":prenom", prenom);
        updateQuery.bindValue(":dateNaissance", dateNaissance);
        updateQuery.bindValue(":email", email);
        updateQuery.bindValue(":telephone", telephone);
        updateQuery.bindValue(":cin", cin);

        if (updateQuery.exec()) {
            QMessageBox::information(this, "Succès", "Client mis à jour avec succès.");

            // Réinitialiser les champs du formulaire
            ui->nomLineEdit->clear();
            ui->prenomLineEdit->clear();
            ui->dateNaissanceLineEdit->clear();
            ui->emailLineEdit->clear();
            ui->telephoneLineEdit->clear();
            ui->cinLineEdit->clear();

            ui->cinLineEdit->setDisabled(false);
            ui->ajouterButton->setText("Ajouter");
            ui->label_5->setText("Ajouter un client");

            // Mettre à jour l'affichage si nécessaire
            afficherClients();
        } else {
            QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de la mise à jour du client : ");
        }
    } else {
        // Ajouter un nouveau client si le CIN n'existe pas
        if (client.ajouter()) {
            QMessageBox::information(this, "Succès", "Client ajouté avec succès.");

            // Réinitialiser les champs du formulaire
            ui->nomLineEdit->clear();
            ui->prenomLineEdit->clear();
            ui->dateNaissanceLineEdit->clear();
            ui->emailLineEdit->clear();
            ui->telephoneLineEdit->clear();
            ui->cinLineEdit->clear();

            ui->cinLineEdit->setDisabled(false);
            ui->ajouterButton->setText("Ajouter");
            ui->label_5->setText("Ajouter un client");

            // Mettre à jour l'affichage si nécessaire
            afficherClients();
        } else {
            QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de l'ajout du client.");
            return;
        }
    }

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

    // Préparer la requête SQL
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, DATENAISSANCE, EMAIL, TELEPHONE, CIN FROM client WHERE TELEPHONE = :telephone");
    query.bindValue(":telephone", telephone);

    // Exécuter la requête
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'exécution de la requête SQL : ");
        return;
    }

    // Vérifier si un client a été trouvé
    if (query.next()) {
        QString nom = query.value("NOM").toString();
        QString prenom = query.value("PRENOM").toString();
        QString dateNaissance = query.value("DATENAISSANCE").toString();
        QString email = query.value("EMAIL").toString();
        QString cin = query.value("CIN").toString();

        // Afficher les informations du client trouvé
        QMessageBox::information(
            this,
            "Client trouvé",
            QString("Nom : %1\nPrénom : %2\nDate de Naissance : %3\nEmail : %4\nTéléphone : %5\nCIN : %6")
                .arg(nom)
                .arg(prenom)
                .arg(dateNaissance)
                .arg(email)
                .arg(telephone)
                .arg(cin)
        );
    } else {
        // Aucun client trouvé
        QMessageBox::warning(this, "Erreur", "Aucun client avec ce numéro de téléphone n'a été trouvé.");
    }
}


void MainWindow::on_modifierButton_clicked()
{
    // Récupérer le CIN saisi par l'utilisateur
    QString cin = ui->modif->text();

    // Vérifier si le champ CIN est vide
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer le CIN du client pour modifier ses informations.");
        return;
    }

    // Préparer une requête SQL pour récupérer les informations du client
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, DATENAISSANCE, EMAIL, TELEPHONE, VISITE FROM client WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    // Exécuter la requête
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'exécution de la requête SQL : ");
        return;
    }

    // Vérifier si un client a été trouvé
    if (query.next()) {
        // Remplir les champs avec les informations du client
        ui->nomLineEdit->setText(query.value("NOM").toString());
        ui->prenomLineEdit->setText(query.value("PRENOM").toString());
        ui->dateNaissanceLineEdit->setDate(query.value("DATENAISSANCE").toDate());
        ui->emailLineEdit->setText(query.value("EMAIL").toString());
        ui->telephoneLineEdit->setText(query.value("TELEPHONE").toString());
        ui->cinLineEdit->setText(cin);
        ui->cinLineEdit->setDisabled(true);
        ui->ajouterButton->setText("Modifier");
        ui->label_5->setText("Modifier un client");
        ui->modif->clear();

        // Vous pouvez afficher ou utiliser "VISITE" si nécessaire
        int visite = query.value("VISITE").toInt();
        qDebug() << "Nombre de visites : " << visite;

        QMessageBox::information(this, "Succès", "Informations du client chargées avec succès.");
    } else {
        // Si aucun client n'a été trouvé
        QMessageBox::warning(this, "Erreur", "Aucun client trouvé avec ce CIN.");
    }
}

void MainWindow::on_supprimerButton_clicked()
{
    QString cin = ui->cin_supp->text();

    // Vérifier si le champ CIN est vide
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN pour supprimer un client.");
        return;
    }

    // Vérifier si le client existe dans la base de données
    Client client;
    if (!client.exists(cin)) {
        QMessageBox::warning(this, "Erreur", "Aucun client trouvé avec ce CIN.");
        return;
    }

    // Supprimer le client
    QSqlQuery query;
    query.prepare("DELETE FROM client WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Client supprimé avec succès.");

        // Mettre à jour la liste des clients dans l'interface
        afficherClients();

        // Réinitialiser le champ de saisie
        ui->cin_supp->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de la suppression du client : ");
    }
}

void MainWindow::afficherClients()
{
    Client c;
    QSqlQueryModel* model = c.afficher();  // Assuming `client` is an instance of the `Client` class

    // Set the model to the QTableView
    ui->tableView->setModel(model);

    // Customize the table view (optional)
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    qDebug() << "Affichage des clients terminé.";
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
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter vers CSV", "", "Fichiers CSV (*.csv)");

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

    // Créer une requête SQL pour récupérer les clients depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, CIN, EMAIL, TELEPHONE, DATENAISSANCE FROM client");

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'exécution de la requête SQL.");
        return;
    }

    // Parcourir les résultats de la requête et écrire les données dans le fichier CSV
    while (query.next()) {
        stream << query.value(0).toString() << ','  // NOM
               << query.value(1).toString() << ','  // PRENOM
               << query.value(2).toString() << ','  // CIN
               << query.value(3).toString() << ','  // EMAIL
               << query.value(4).toString() << ','  // TELEPHONE
               << query.value(5).toString() << '\n'; // DATENAISSANCE
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

    // Vérifier si le client existe dans la base de données
    Client client = client.trouverClientParCin(cin);  // Assuming `trouverClientParCin` finds the client in the DB
    if (client.getCin().isEmpty()) {  // If client is not found
        QMessageBox::warning(this, "Erreur", "Le client avec ce CIN n'existe pas dans la base.");
        return;
    }

    // Increment the visit count for the client in the database
    bool visiteIncremented = client.incrementVisite(cin);  // Assuming `incrementVisite` updates the visit count in DB
    if (!visiteIncremented) {
        QMessageBox::warning(this, "Erreur", "Une erreur est survenue lors de l'enregistrement de la visite.");
        return;
    }

    afficherClients();

    // Vérifier si le client a droit à une réduction
    int nbVisites = client.getVisite();  // Get the updated visit count from the database
    if (nbVisites >= 3) {
        QMessageBox::information(
            this,
            "Fidélité",
            QString("Ce client a visité %1 fois. Une réduction lui est offerte !").arg(nbVisites)
        );
    } else {
        QMessageBox::information(
            this,
            "Visite enregistrée",
            QString("Visite enregistrée. Ce client a visité %1 fois.").arg(nbVisites)
        );
    }
}



void MainWindow::afficherHistorique()
{
    QDialog* historiqueDialog = new QDialog(this);
        historiqueDialog->setWindowTitle("Historique des Clients");
        historiqueDialog->setMinimumSize(800, 600);
        historiqueDialog->setStyle(QStyleFactory::create("Fusion"));  // Modern, minimal style

        // Layout for the dialog
        QVBoxLayout* layout = new QVBoxLayout(historiqueDialog);

        // Create the table view
        QTableView* tableView = new QTableView(historiqueDialog);
        tableView->setStyleSheet("QTableView { border: 1px solid #ddd; font-size: 14px; }"
                                 "QHeaderView::section { background-color: #f0f0f0; padding: 5px; }");

        // Set up the query model with the client data
        QSqlQueryModel* model = new QSqlQueryModel();

        // SQL query to fetch CIN, VISITES, and loyalty status (fidel)
        model->setQuery(R"(
            SELECT CIN, VISITE,
                   CASE WHEN VISITE >= 2 THEN 'Fidèle' ELSE 'Non Fidèle' END AS Fidel
            FROM client
        )");

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Visites"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Statut Fidélité"));

        // Set the model to the table view
        tableView->setModel(model);
        tableView->horizontalHeader()->setStretchLastSection(true); // Stretch the last column
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

        // Add table view to the layout
        layout->addWidget(tableView);

        // Add a close button
        QPushButton* closeButton = new QPushButton("Fermer", historiqueDialog);
        connect(closeButton, &QPushButton::clicked, historiqueDialog, &QDialog::accept); // Close the dialog when clicked
        layout->addWidget(closeButton);

        // Set the layout for the dialog
        historiqueDialog->setLayout(layout);

        // Show the dialog
        historiqueDialog->exec();
}


#include <QMap>
#include <QString>
#include <QRegularExpression>

// Fonction pour récupérer la nationalité en fonction du préfixe téléphonique
QString obtenirNationalite(const QString &numero) {
    // Table de correspondance entre les indicatifs et les nationalités
    QMap<QString, QString> indicatifs;
    indicatifs.insert("+33", "France");
    indicatifs.insert("+1", "USA");
    indicatifs.insert("+44", "Royaume-Uni");
    indicatifs.insert("+49", "Allemagne");
    indicatifs.insert("+34", "Espagne");
    indicatifs.insert("+39", "Italie");
    indicatifs.insert("+213", "Algérie");
    indicatifs.insert("+216", "Tunisie");
    indicatifs.insert("+212", "Maroc");
    indicatifs.insert("+20", "Égypte");
    indicatifs.insert("+27", "Afrique du Sud");
    indicatifs.insert("+254", "Kenya");
    indicatifs.insert("+234", "Nigéria");
    indicatifs.insert("+256", "Ouganda");

    indicatifs.insert("+971", "Émirats Arabes Unis");
    indicatifs.insert("+966", "Arabie Saoudite");
    indicatifs.insert("+965", "Koweït");
    indicatifs.insert("+974", "Qatar");
    indicatifs.insert("+92", "Pakistan");
    indicatifs.insert("+91", "Inde");
    indicatifs.insert("+81", "Japon");
    indicatifs.insert("+82", "Corée du Sud");
    indicatifs.insert("+86", "Chine");

    indicatifs.insert("+61", "Australie");
    indicatifs.insert("+64", "Nouvelle-Zélande");

    indicatifs.insert("+55", "Brésil");
    indicatifs.insert("+54", "Argentine");
    indicatifs.insert("+57", "Colombie");
    indicatifs.insert("+58", "Venezuela");
    indicatifs.insert("+52", "Mexique");
    indicatifs.insert("+56", "Chili");
    indicatifs.insert("+51", "Pérou");

    indicatifs.insert("+7", "Russie");
    indicatifs.insert("+380", "Ukraine");
    indicatifs.insert("+48", "Pologne");
    indicatifs.insert("+40", "Roumanie");
    indicatifs.insert("+36", "Hongrie");
    indicatifs.insert("+30", "Grèce");
    indicatifs.insert("+47", "Norvège");
    indicatifs.insert("+46", "Suède");
    indicatifs.insert("+358", "Finlande");
    indicatifs.insert("+41", "Suisse");
    indicatifs.insert("+43", "Autriche");
    indicatifs.insert("+31", "Pays-Bas");
    indicatifs.insert("+32", "Belgique");
    indicatifs.insert("+351", "Portugal");
    indicatifs.insert("+353", "Irlande");
    indicatifs.insert("+356", "Malte");
    indicatifs.insert("+62", "Indonésie");
    indicatifs.insert("+63", "Philippines");
    indicatifs.insert("+65", "Singapour");
    indicatifs.insert("+60", "Malaisie");
    indicatifs.insert("+66", "Thaïlande");
    indicatifs.insert("+84", "Vietnam");
    indicatifs.insert("+853", "Macao");
    indicatifs.insert("+852", "Hong Kong");


    // Vérification de la validité du numéro
    if (numero.isEmpty() || !numero.startsWith("+")) {
        qDebug() << "Numéro invalide (ne commence pas par '+'):" << numero;
        return "Inconnu";  // Si le numéro est invalide
    }

    // Expression régulière pour extraire l'indicatif international (par exemple, +33, +1)
    QStringList sortedIndicatifs = indicatifs.keys();
    std::sort(sortedIndicatifs.begin(), sortedIndicatifs.end(), [](const QString &a, const QString &b) {
        return a.length() > b.length();  // Trier les indicatifs par longueur décroissante
    });

    // Vérification si l'indicatif correspond
    for (const QString &indicatif : sortedIndicatifs) {
        if (numero.startsWith(indicatif)) {
            qDebug() << "Indicatif trouvé pour le numéro" << numero << ":" << indicatif;
            return indicatifs.value(indicatif, "Inconnu");  // "Inconnu" si l'indicatif n'est pas trouvé
        }
    }

    // Si aucun indicatif valide n'a été trouvé
    qDebug() << "Aucun indicatif trouvé pour le numéro:" << numero;
    return "Inconnu";
}

int calculerAge(const QString &dateNaissance) {
    QDate birthDate = QDate::fromString(dateNaissance, "yyyy-MM-dd");
    if (!birthDate.isValid()) {
        qDebug() << "Date de naissance invalide:" << dateNaissance;
        return -1;
    }

    QDate currentDate = QDate::currentDate();
    int age = currentDate.year() - birthDate.year();
    if (currentDate.month() < birthDate.month() || (currentDate.month() == birthDate.month() && currentDate.day() < birthDate.day())) {
        age--;  // Si l'anniversaire n'est pas encore passé cette année
    }
    return age;
}

void MainWindow::on_afficherStatistiquesButton_clicked() {
    // Map to store the nationalities and their occurrences
    QMap<QString, int> nationalityDistribution;

    // Use afficher() to get client data
    Client client;
    QSqlQueryModel* model = client.afficher();

    // Iterate over the rows of the model to get the client data
    for (int row = 0; row < model->rowCount(); ++row) {
        QString telephone = model->data(model->index(row, 5)).toString();  // Téléphone
        QString dateNaissance = model->data(model->index(row, 3)).toString();  // Date de naissance

        // Calculate the nationality from the telephone number
        QString nationalite = obtenirNationalite(telephone);

        // Calculate age from the birthdate
        int age = calculerAge(dateNaissance);

        // Count nationalities
        if (!nationalite.isEmpty()) {
            nationalityDistribution[nationalite]++;
        }

    }

    // Check if any nationality or age data was found
    if (nationalityDistribution.isEmpty()) {
        qDebug() << "Aucune donnée valide trouvée.";
        return;
    }

    // Create a new dialog window to show the statistics
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des Clients");

    // Create a QVBoxLayout for the dialog to hold the charts
    QVBoxLayout *layout = new QVBoxLayout(statsDialog);

    // Nationality Distribution Chart
    QList<QString> nationalities = nationalityDistribution.keys();
    QList<int> nationalityCounts = nationalityDistribution.values();
    QBarSet *nationalitySet = new QBarSet("Nationalités");
    for (int count : nationalityCounts) {
        nationalitySet->append(count);
    }

    QChart *nationalityChart = new QChart();
    QBarSeries *nationalitySeries = new QBarSeries();
    nationalitySeries->append(nationalitySet);
    nationalityChart->addSeries(nationalitySeries);

    QBarCategoryAxis *axisX1 = new QBarCategoryAxis();
    axisX1->append(nationalities);
    nationalityChart->setAxisX(axisX1, nationalitySeries);

    QValueAxis *axisY1 = new QValueAxis();
    axisY1->setRange(0, *std::max_element(nationalityCounts.begin(), nationalityCounts.end()));
    nationalityChart->setAxisY(axisY1, nationalitySeries);

    QChartView *nationalityChartView = new QChartView(nationalityChart);
    nationalityChartView->setRenderHint(QPainter::Antialiasing);
    nationalityChartView->setMinimumSize(400, 300);

    // Age Distribution Chart

    // Add the charts to the layout
    layout->addWidget(nationalityChartView);

    // Show the dialog
    statsDialog->setLayout(layout);
    statsDialog->exec();  // Show the dialog modally
}


void MainWindow::on_arriveeButton_clicked()
{

}
void MainWindow::on_trier_clicked()
{
    // Get the selected sort criteria from the first combo box
    QString sortBy = ui->sortBy->currentText(); // Assuming comboBox_sortBy is the name of the combo box
    QString sortOrder = ui->sortOrder->currentText(); // Assuming comboBox_sortOrder is the name of the combo box

    QString column;
    QString order;

    // Map the selected criteria to database columns
    if (sortBy == "Nom") {
        column = "NOM";
    } else if (sortBy == "Prénom") {
        column = "PRENOM";
    } else if (sortBy == "Date de Naissance") {
        column = "DATENAISSANCE";
    } else if (sortBy == "Email") {
        column = "EMAIL";
    } else if (sortBy == "Téléphone") {
        column = "TELEPHONE";
    } else if (sortBy == "CIN") {
        column = "CIN";
    } else if (sortBy == "Visite") {
        column = "VISITE";
    } else {
        QMessageBox::warning(this, "Erreur", "Critère de tri invalide.");
        return;
    }

    // Map the sort order to SQL keywords
    if (sortOrder == "Ascendant") {
        order = "ASC";
    } else if (sortOrder == "Descendant") {
        order = "DESC";
    } else {
        QMessageBox::warning(this, "Erreur", "Ordre de tri invalide.");
        return;
    }

    // Build the SQL query
    QString queryStr = QString("SELECT CIN, NOM, PRENOM, DATENAISSANCE, EMAIL, TELEPHONE, VISITE FROM client ORDER BY %1 %2")
                           .arg(column)
                           .arg(order);

    // Execute the query and update the view
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(queryStr);

    // Update the table view with the sorted data
    ui->tableView->setModel(model);

    // Optionally, add headers for better display
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date de Naissance"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Visite"));

    QMessageBox::information(this, "Succès", "Tri effectué avec succès !");
}

void MainWindow::on_Obtenir_clicked()
{
    // Get the CIN entered by the user
    QString cin = ui->cin_nationalite->text();

    // Check if CIN is empty
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN.");
        return;
    }

    // Search for the client with the given CIN
    Client client = client.trouverClientParCin(cin);
    // Check if the client was found
    if (client.getCin().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Client non trouvé avec ce CIN.");
        return;
    }

    // Get the client's phone number
    QString numero = client.getTelephone();

    // Check if the phone number is valid
    if (numero.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone du client est invalide.");
        return;
    }

    // Call the function obtenirNationalite to get the nationality
    QString nationalite = obtenirNationalite(numero);

    // Display the nationality in a message box
    QMessageBox::information(this, "Nationalité", "La nationalité du client est : " + nationalite);
}


void MainWindow::on_Imprimer_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter vers PDF", "", "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun fichier sélectionné.");
        return;
    }

    // Ajouter l'extension .pdf si nécessaire
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // Créer un objet QPdfWriter pour générer le fichier PDF
    QPdfWriter writer(fileName);
    writer.setResolution(300);

    // Créer un QPainter pour dessiner dans le fichier PDF
    QPainter painter(&writer);

    if (!painter.isActive()) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier PDF.");
        return;
    }

    // Définir la police et les marges pour le PDF
    QFont font("Arial", 10);
    painter.setFont(font);
    int x = 50;  // Position X pour dessiner le texte
    int y = 50;  // Position Y pour dessiner le texte

    // Ajouter un titre
    painter.drawText(x, y, "Liste des Clients");
    y += 60;

    // Écrire les en-têtes de colonnes
    painter.drawText(x, y, "Nom");
    painter.drawText(x + 200, y, "Prenom");
    painter.drawText(x + 400, y, "CIN");
    painter.drawText(x + 600, y, "Adresse Email");
    painter.drawText(x + 1000, y, "Telephone");
    painter.drawText(x + 1400, y, "Date de Naissance");

    y += 60;  // Ajouter un espacement après les en-têtes

    // Créer une requête SQL pour récupérer les clients depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, CIN, EMAIL, TELEPHONE, DATENAISSANCE FROM client");

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'exécution de la requête SQL.");
        return;
    }

    // Parcourir les résultats de la requête et dessiner les données dans le PDF
    while (query.next()) {
        painter.drawText(x, y, query.value(0).toString());  // NOM
        painter.drawText(x + 200, y, query.value(1).toString());  // PRENOM
        painter.drawText(x + 400, y, query.value(2).toString());  // CIN
        painter.drawText(x + 600, y, query.value(3).toString());  // EMAIL
        painter.drawText(x + 1000, y, query.value(4).toString());  // TELEPHONE
        painter.drawText(x + 1400, y, query.value(5).toString());  // DATENAISSANCE

        y += 100;  // Espacement entre les lignes de données

        // Ajouter une nouvelle page si la fin de la page est atteinte
        if (y > writer.height() - 50) {
            writer.newPage();
            y = 50;  // Réinitialiser la position Y
        }
    }

    // Terminer l'opération d'impression
    painter.end();

    // Informer l'utilisateur que l'export a réussi
    QMessageBox::information(this, "Succès", "Les données ont été exportées avec succès dans le fichier : " + fileName);
}

void MainWindow::on_Obtenir_2_clicked()
{
    QString cin = ui->cin_qrcode->text();

    // Check if CIN is empty
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN.");
        return;
    }

    // Search for the client with the given CIN
    Client client = client.trouverClientParCin(cin);
    // Check if the client was found
    if (client.getCin().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Client non trouvé avec ce CIN.");
        return;
    }
    // Create a string to encode as the QR code
    QString qrText = client.getCin() + "\n" + client.getNom() + " " + client.getPrenom() + "\n" + client.getEmail();

    // Generate the QR code
    uint8_t qrBuffer[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];

    bool ok = qrcodegen_encodeText(qrText.toStdString().c_str(),
                                   tempBuffer, qrBuffer,
                                   qrcodegen_Ecc_MEDIUM,
                                   qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX,
                                   qrcodegen_Mask_AUTO, true);

    if (!ok) {
        // Handle error (you can show a warning message here if you want)
        return;
    }

    // Get the size of the QR code
    int size = qrcodegen_getSize(qrBuffer);

    // Create a QImage to display the QR code
    QImage image(size, size, QImage::Format_RGB888);

    // Render the QR code to the QImage
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            bool isModuleSet = qrcodegen_getModule(qrBuffer, x, y);
            // Set the pixel color to black or white based on whether the module is set
            image.setPixel(x, y, isModuleSet ? qRgb(0, 0, 0) : qRgb(255, 255, 255));
        }
    }

    // Create a new dialog window to display the QR code
    QDialog *qrDialog = new QDialog(this);
    qrDialog->setWindowTitle("QR Code");

    // Create a label to display the QR code image
    QLabel *qrLabel = new QLabel(qrDialog);
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap = pixmap.scaled(300, 300, Qt::KeepAspectRatio); // Change 300 to your preferred size

    qrLabel->setPixmap(pixmap);
    qrLabel->setScaledContents(true);

    // Set up the layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(qrLabel);
    qrDialog->setLayout(layout);

    // Show the dialog
    qrDialog->exec();  // Show the dialog modally
}
