#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "service.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlError>  // Add this include to resolve the error
#include <QPdfWriter>
#include <QPainter>
#include <QUrlQuery>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    service(new Service()) // Initialize the Service object
{
    ui->setupUi(this);
    connect(ui->pushButton_afficher, &QPushButton::clicked, this, &MainWindow::refreshTableView);
}

MainWindow::~MainWindow() {
    delete service; // Release allocated memory
    delete ui;
}

void MainWindow::on_pushButton_ajouter_clicked() {
    int id_service = ui->lineEdit_id->text().toInt();
    QString service_demande = ui->comboBox_17->currentText();
    int numero = ui->lineEdit_numero->text().toInt();
    QDate datee = ui->dateEdit_date->date();
    float tarif = ui->lineEdit_tarif->text().toFloat();

    // Create the Service object without the 'nom' field
    Service newService(id_service, service_demande, numero, datee, tarif);

    if (newService.ajouter()) {
        QMessageBox::information(this, "Success", "Service added successfully.");

        ui->lineEdit_id->clear();
        ui->lineEdit_numero->clear();
        ui->lineEdit_tarif->clear();
        ui->dateEdit_date->setDate(QDate::currentDate());
        ui->comboBox_17->setCurrentIndex(0);

    } else {
        QMessageBox::critical(this, "Error", "Failed to add service.");
    }
}
void MainWindow::on_pushButton_modifier_clicked() {
    int id_service = ui->lineEdit_id->text().toInt();

    // Vérifier si l'ID existe
    QSqlQuery query;
    query.prepare("SELECT * FROM services WHERE ID_SERVICE = :id_service");
    query.bindValue(":id_service", id_service);

    if (query.exec() && query.next()) {
        // Si le service existe, permettre la modification
        QString service_demande = ui->comboBox_17->currentText();
        int numero = ui->lineEdit_numero->text().toInt();
        QDate datee = ui->dateEdit_date->date();
        float tarif = ui->lineEdit_tarif->text().toFloat();

        // Appeler la méthode pour modifier
        if (service->modifier(id_service, service_demande, numero, datee, tarif)) {
            QMessageBox::information(this, "Success", "Service updated successfully.");

            // Rafraîchir la table après modification
            refreshTableView();

            // Réinitialiser les champs si nécessaire
            ui->lineEdit_id->clear();
            ui->lineEdit_numero->clear();
            ui->lineEdit_tarif->clear();
            ui->dateEdit_date->setDate(QDate::currentDate());
            ui->comboBox_17->setCurrentIndex(0);
        } else {
            QMessageBox::critical(this, "Error", "Failed to update service.");
        }
    } else {
        QMessageBox::warning(this, "Not Found", "Service ID not found.");
    }
}

void MainWindow::on_pushButton_supprimer_clicked() {
    int id_service = ui->lineEdit_id->text().toInt();

    // Check if the ID exists in the database
    QSqlQuery query;
    query.prepare("SELECT * FROM services WHERE ID_SERVICE = :id_service");
    query.bindValue(":id_service", id_service);

    if (query.exec() && query.next()) {
        // If the service exists, delete it
        query.prepare("DELETE FROM services WHERE ID_SERVICE = :id_service");
        query.bindValue(":id_service", id_service);

        if (query.exec()) {
            QMessageBox::information(this, "Success", "Service deleted successfully.");
            ui->lineEdit_id->clear(); // Clear the input field
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete service.");
        }
    } else {
        QMessageBox::warning(this, "Not Found", "Service ID not found.");
    }
}
void MainWindow::on_pushButton_afficher_clicked() {
    // Create a QSqlTableModel to interact with the 'services' table
    QSqlTableModel *model = new QSqlTableModel(this);

    // Set the table and make sure it's connected to the correct database
    model->setTable("services");

    // Optional: Set the filter to display only certain rows (if needed)
    // model->setFilter("ID_SERVICE > 0");

    // Set the edit strategy
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // Check if data can be fetched from the database
    if (!model->select()) {
        qDebug() << "Error: Could not fetch data from services table.";
        qDebug() << model->lastError().text();  // Log any error message
        return;
    }

    // Check the number of rows returned
    int rowCount = model->rowCount();
    qDebug() << "Number of rows in services table: " << rowCount;

    if (rowCount == 0) {
        qDebug() << "The services table is empty.";
        return;
    }

    // Set the model to the QTableView to display the data
    ui->tableView->setModel(model);

    // Optional: Set custom headers (based on column order)
    model->setHeaderData(0, Qt::Horizontal, "ID_SERVICE");
    model->setHeaderData(1, Qt::Horizontal, "SERVICE_DEMANDE");
    model->setHeaderData(2, Qt::Horizontal, "NUMERO");
    model->setHeaderData(3, Qt::Horizontal, "DATEE");
    model->setHeaderData(4, Qt::Horizontal, "TARIF");

    // Resize the columns to fit content
    ui->tableView->resizeColumnsToContents();
}
void MainWindow::refreshTableView()
{
    Service service;
    QSqlQueryModel *model = service.afficher();  // Use the Service class to get data from the services table

    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "No Matching Services",
                                 "No services found in the database.",
                                 QMessageBox::Ok);
        delete model;
        return;
    }

    // Set the headers based on the columns of the "services" table
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Service"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Service Demand"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Number"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Tariff"));

    // Set the model for the QTableView
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}
void MainWindow::on_pushButton_statistique_clicked()
{
    QSqlQuery query;

    // Query to count the occurrences of 'cleaning' in SERVICE_DEMANDE
    query.prepare("SELECT COUNT(*) FROM services WHERE SERVICE_DEMANDE = 'cleaning'");
    if (!query.exec()) {
        qDebug() << "Error counting cleaning services:" << query.lastError().text();
        return;
    }
    query.next();
    int cleaningCount = query.value(0).toInt();

    // Query to count the occurrences of 'food' in SERVICE_DEMANDE
    query.prepare("SELECT COUNT(*) FROM services WHERE SERVICE_DEMANDE = 'food'");
    if (!query.exec()) {
        qDebug() << "Error counting food services:" << query.lastError().text();
        return;
    }
    query.next();
    int foodCount = query.value(0).toInt();

    // Create the custom widget and set the statistics
    StatistiqueWidget *statWidget = new StatistiqueWidget(this);
    statWidget->setStatistics(cleaningCount, foodCount);
    ui->verticalLayout->addWidget(statWidget);  // Assuming you have a layout in your UI
}


#include <QFileDialog>




void MainWindow::on_pushButton_pdf_clicked()
{
    // Prompt the user to choose a location and filename for the HTML file
    QString filePath = QFileDialog::getSaveFileName(this, "Save HTML", "", "HTML Files (*.html);;All Files (*)");

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "No File Selected", "No file was selected to save.");
        return;
    }

    // Ensure the selected file has a .html extension
    if (!filePath.endsWith(".html", Qt::CaseInsensitive)) {
        filePath += ".html";
    }

    // Create an HTML string to hold the document content
    QString htmlContent;

    // Start HTML structure
    htmlContent += "<!DOCTYPE html>\n<html lang='en'>\n<head>\n";
    htmlContent += "<meta charset='UTF-8'>\n";
    htmlContent += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
    htmlContent += "<title>Service Evaluation Report</title>\n";
    htmlContent += "<style>\n";
    htmlContent += "body { font-family: Arial, sans-serif; margin: 20px; }\n";
    htmlContent += "h1 { font-size: 24px; }\n";
    htmlContent += "table { width: 100%; border-collapse: collapse; margin-top: 20px; }\n";
    htmlContent += "table, th, td { border: 1px solid black; }\n";
    htmlContent += "th, td { padding: 8px; text-align: left; }\n";
    htmlContent += "tr:nth-child(even) { background-color: #f2f2f2; }\n";
    htmlContent += "</style>\n</head>\n<body>\n";

    // Add the title
    htmlContent += "<h1>Service Evaluation Report</h1>\n";

    // Query to fetch all services from the database
    QSqlQuery query;
    query.prepare("SELECT id_service, service_demande, numero, datee, tarif FROM services");

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to fetch data from the database.");
        return;
    }

    // Start the table
    htmlContent += "<table>\n";
    htmlContent += "<tr><th>ID</th><th>Service</th><th>Number</th><th>Date</th><th>Price</th><th>Evaluation</th></tr>\n";

    // Loop through each service and add its details to the HTML table
    while (query.next()) {
        int serviceId = query.value(0).toInt();
        QString serviceDemand = query.value(1).toString();
        QString numero = query.value(2).toString();
        QDate datee = query.value(3).toDate();
        float tarif = query.value(4).toFloat();

        // Compress the data by limiting the length of fields
        if (serviceDemand.length() > 30) {
            serviceDemand = serviceDemand.left(30) + "..."; // Truncate long service demands
        }
        if (numero.length() > 15) {
            numero = numero.left(15) + "..."; // Truncate long numbers
        }

        // Add service information to the table
        htmlContent += "<tr>\n";
        htmlContent += QString("<td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>")
                           .arg(serviceId)
                           .arg(serviceDemand)
                           .arg(numero)
                           .arg(datee.toString("yyyy-MM-dd"))
                           .arg(tarif);

        // Add a simple evaluation
        QString evaluation;
        if (tarif > 500) {
            evaluation = "High Price";
        } else if (tarif > 200) {
            evaluation = "Medium Price";
        } else {
            evaluation = "Low Price";
        }

        htmlContent += evaluation + "</td>\n";
        htmlContent += "</tr>\n";
    }

    // Close the table and body
    htmlContent += "</table>\n";
    htmlContent += "</body>\n</html>\n";

    // Write the HTML content to the file
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "File Error", "Failed to save the HTML file.");
        return;
    }

    QTextStream out(&file);
    out << htmlContent;
    file.close();

    // Notify the user that the HTML has been saved
    QMessageBox::information(this, "HTML Saved", "The HTML was saved successfully at " + filePath);
}




void MainWindow::on_pushButton_trier_clicked()
{
    // Récupérer la valeur sélectionnée dans le combobox
    QString selectedOption = ui->comboBox_19->currentText();

    // Créer la requête SQL de base pour sélectionner tous les services
    QString queryStr = "SELECT * FROM services";

    // Ajouter un ordre de tri selon l'option choisie
    if (selectedOption == "trie_par_id") {
        queryStr += " ORDER BY Id_service";  // Trier par ID de service
    } else if (selectedOption == "trie_par_service_demande") {
        queryStr += " ORDER BY service_demande";  // Trier par service demandé
    } else if (selectedOption == "trie_par_numero") {
        queryStr += " ORDER BY numero";  // Trier par numéro
    }

    // Créer un modèle pour afficher les résultats
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Préparer la requête
    query.prepare(queryStr);

    // Exécuter la requête
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exécution de la requête de tri.");
        return;
    }

    // Attacher les résultats de la requête au modèle
    model->setQuery(query);

    // Vérifier si la requête a retourné des résultats
    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Aucun Résultat", "Aucun service trouvé après le tri.");
        delete model;
        return;
    }

    // Afficher les données dans le tableView_3
    ui->tableView_3->setModel(model);
    ui->tableView_3->resizeColumnsToContents();  // Ajuster la taille des colonnes

    // Optionnel : vous pouvez définir les en-têtes de colonnes si nécessaire
    model->setHeaderData(0, Qt::Horizontal, "ID Service");
    model->setHeaderData(1, Qt::Horizontal, "Service Demandé");
    model->setHeaderData(2, Qt::Horizontal, "Numéro");
    model->setHeaderData(3, Qt::Horizontal, "Date");
    model->setHeaderData(4, Qt::Horizontal, "Tarif");
}
void MainWindow::on_pushButton_recherche_clicked()
{
    // Récupérer la valeur saisie dans lineEdit_recherche
    QString searchValue = ui->lineEdit_recherche->text().trimmed();

    // Récupérer l'option sélectionnée dans comboBox_20
    QString searchType = ui->comboBox_20->currentText();

    // Initialiser la requête SQL
    QString queryStr = "SELECT * FROM services WHERE ";

    // Vérifier le type de recherche et ajuster la requête
    if (searchType == "recherche_par_id") {
        bool ok;
        int searchId = searchValue.toInt(&ok);

        // Vérifier si la conversion en entier a réussi
        if (!ok) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un identifiant valide.");
            return;
        }

        queryStr += QString("Id_service = %1").arg(searchId);  // Recherche par ID de service
    }
    else if (searchType == "recherche_par_tarif") {
        bool ok;
        float searchTarif = searchValue.toFloat(&ok);

        // Vérifier si la conversion en float a réussi
        if (!ok) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un tarif valide.");
            return;
        }

        queryStr += QString("tarif = %1").arg(searchTarif);  // Recherche par tarif
    }
    else if (searchType == "recherche_par_numero") {
        bool ok;
        int searchNumero = searchValue.toInt(&ok);

        // Vérifier si la conversion en entier a réussi
        if (!ok) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un numéro valide.");
            return;
        }

        queryStr += QString("numero = %1").arg(searchNumero);  // Recherche par numéro
    }
    else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un type de recherche.");
        return;
    }

    // Créer un modèle pour afficher les résultats
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Préparer la requête
    query.prepare(queryStr);

    // Exécuter la requête
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exécution de la requête.");
        return;
    }

    // Vérifier si la requête a retourné des résultats
    if (query.size() == 0) {
        QMessageBox::information(this, "Aucun Résultat", "Aucun service trouvé avec les critères de recherche.");
        delete model;
        return;
    }

    // Attacher les résultats de la requête au modèle
    model->setQuery(query);

    // Afficher les données dans tableView_3
    ui->tableView_3->setModel(model);
    ui->tableView_3->resizeColumnsToContents();  // Ajuster la taille des colonnes

    // Optionnel : vous pouvez définir les en-têtes de colonnes si nécessaire
    model->setHeaderData(0, Qt::Horizontal, "ID Service");
    model->setHeaderData(1, Qt::Horizontal, "Service Demandé");
    model->setHeaderData(2, Qt::Horizontal, "Numéro");
    model->setHeaderData(3, Qt::Horizontal, "Date");
    model->setHeaderData(4, Qt::Horizontal, "Tarif");
}

void MainWindow::sendSMS(const QString &clientPhoneNumber, const QString &messageBody) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QString accountSID = "AC956f9e29ad437a8117b12f33da2d93e5";  // Remplacez par votre Account SID Twilio
    QString authToken = "c7c00354535a870aa95760a945862bec";    // Remplacez par votre Auth Token Twilio
    QString fromPhoneNumber = "+12512209634"; // Remplacez par votre numéro Twilio (format international)

    // Convertir le numéro du client en format E.164 (ajouter le préfixe +216 pour la Tunisie)
    QString formattedPhoneNumber = "+216" + clientPhoneNumber;  // Assurez-vous que le clientPhoneNumber est juste un numéro local

    QUrl url(QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(accountSID));
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(accountSID, authToken).toUtf8()).toBase64());

    QUrlQuery params;
    params.addQueryItem("To", formattedPhoneNumber); // Numéro du destinataire au format E.164
    params.addQueryItem("From", fromPhoneNumber);    // Votre numéro Twilio
    params.addQueryItem("Body", messageBody);        // Le message

    QNetworkReply *reply = manager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "SMS envoyé avec succès!";
        } else {
            qDebug() << "Erreur lors de l'envoi du SMS:" << reply->errorString();
            // Afficher la réponse complète de l'API Twilio pour plus de détails
            qDebug() << "Réponse de l'API Twilio: " << reply->readAll();
        }
        reply->deleteLater();
    });
}
void MainWindow::notifySMS(const QString &TELEPHONE, const QString &message) {
    // Simuler l'envoi d'un SMS en affichant un message
    QString notification = QString("SMS envoyé à %1 :\n%2").arg(TELEPHONE, message);
    QMessageBox::information(this, "Notification SMS", notification);
}

bool MainWindow::validateForm() {
    // Vérifier que les champs TELEPHONE et statut sont remplis
    if (ui->lineEdit_TELEPHONE->text().isEmpty() ||
        ui->lineEdit_statut->text().isEmpty()) {

        QMessageBox::warning(this, "Formulaire incomplet", "Veuillez remplir les champs Téléphone et Statut.");
        return false;  // Retourner false si les champs ne sont pas remplis
    }
    return true;  // Retourner true si les champs sont valides
}


void MainWindow::on_notifications_clicked() {
    if (validateForm()) {
        // Récupérer les champs liés à la table services
        int id_service = ui->lineEdit_id->text().toInt(); // Exemple : ID du service
        QString service_demande = ui->comboBox_17->currentText(); // Exemple : Service demandé
        int numero = ui->lineEdit_numero->text().toInt(); // Exemple : Numéro
        QDate datee = ui->dateEdit_date->date(); // Exemple : Date
        float tarif = ui->lineEdit_tarif->text().toFloat(); // Exemple : Tarif

        // Créer un objet service avec les champs récupérés
        Service newService(id_service,  service_demande, numero, datee, tarif);
        bool test = newService.ajouter();  // Ajouter le service à la base de données

        if (test) {
            QMessageBox::information(this, "OK", "Service ajouté avec succès.");

            // Simuler l'envoi d'une notification SMS
            notifySMS(ui->lineEdit_TELEPHONE->text(), "Votre service a été créé avec succès !");

            // Actualiser l'affichage dans le tableau
            ui->tableView_services->setModel(newService.afficher());

            // Réinitialiser les champs du formulaire
            ui->lineEdit_id->clear();
            ui->lineEdit_numero->clear();
            ui->lineEdit_tarif->clear();
            ui->lineEdit_statut->clear();  // Clear the statut field as well

            ui->comboBox_17->setCurrentIndex(0);
            ui->dateEdit_date->setDate(QDate::currentDate());
            ui->lineEdit_TELEPHONE->clear();
        } else {
            QMessageBox::critical(this, "Erreur", "L'ajout a échoué.");
        }
    }
}
