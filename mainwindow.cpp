#include "mainwindow.h"
#include "ui_mainwindow.h"
<<<<<<< HEAD
#include "transaction.h"
#include <QMessageBox>
#include <QSqlDatabase>
=======
#include <QMessageBox>
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QDoubleValidator>
#include <QSqlQueryModel>
#include <QDate>
#include <QFileDialog>
#include <QPainter>
#include <QPrinter>
<<<<<<< HEAD
#include <QPageSize>
#include <QFile>
#include <QTextStream>
#include <QPdfWriter>
#include <QDateEdit>
#include <QMap>
#include <QDesktopServices>
#include <QUrl>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslConfiguration>
#include <QDebug>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QTcpSocket>
#include <QTextStream>
#include <vector>
#include <iostream>

#include "mailling.h"
<<<<<<< HEAD
#include "arduino.h"
=======
=======
#include <QFile>
#include <QTextStream>
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8

>>>>>>> 5f0d2bc5ff53f00c67448f869f0670e933450b3d


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), T(0, "", "", "", "", 0.0), model(new QSqlQueryModel(this))
{
<<<<<<< HEAD


    ui->setupUi(this);
    arduino = new Arduino(this);

       connect(arduino, &Arduino::dataReceived, this, &MainWindow::handleArduinoData);

       if (!arduino->connectArduino()) {
           QMessageBox::critical(this, "Erreur", "Impossible de se connecter à Arduino.");
}


 setupTrayIcon();


    QDoubleValidator *montantValidator = new QDoubleValidator(0, 1000000, 2, this);
    montantValidator->setLocale(QLocale::English);
    ui->lineEdit_3->setValidator(montantValidator);

    refreshTableView();





        connect(ui->pushButton_47, SIGNAL(clicked()), this, SLOT(on_pushButton_calculerTVA_clicked()));



    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::afficherPageTransactions);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::afficherPageChambres);


   connect(ui->pushButton_13, &QPushButton::clicked, this, &MainWindow::on_pushButton_stat_clicked);
    connect(ui->pushButton_15, &QPushButton::clicked, this, &MainWindow::on_pushButton_15_clicked);
    connect(ui->pushButton_44, &QPushButton::clicked, this, &MainWindow::on_pushButton_44_clicked);
    connect(ui->comboBox_18, &QComboBox::currentTextChanged, this, &MainWindow::on_comboBox_18_selectionChanged);
    connect(ui->pushButton_12, &QPushButton::clicked, this, [=]() {
        generatePdfForPeriod(ui->dateEdit_3->date(), ui->dateEdit_4->date());
    });


    maxDepense = 1000.0;

    }
=======
    ui->setupUi(this);

    // Configuration du QLineEdit pour le montant total
       QDoubleValidator *montantValidator = new QDoubleValidator(0, 1000000, 2, this); // Limite le montant entre 0 et 1 000 000 avec 2 décimales
       montantValidator->setLocale(QLocale::English); // Utilise le point (.) comme séparateur décimal
       ui->lineEdit_3->setValidator(montantValidator); // Applique le validateur au champ du montant


    refreshTableView();
    connect(ui->pushButton_15, &QPushButton::clicked, this, &MainWindow::on_pushButton_15_clicked);
    connect(ui->pushButton_44, &QPushButton::clicked, this, &MainWindow::on_pushButton_44_clicked);
    connect(ui->comboBox_18, &QComboBox::currentTextChanged, this, &MainWindow::on_comboBox_18_selectionChanged);
    // Connecter les boutons aux slots
       connect(ui->pushButton_12, &QPushButton::clicked, this, &MainWindow::generatePdf);

   }
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8

MainWindow::~MainWindow()
{
    delete ui;
    arduino->disconnectArduino();
}

void MainWindow::refreshTableView()
{
<<<<<<< HEAD

=======
    // Configure la requête pour afficher toutes les transactions
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    model->setQuery("SELECT id_transaction, type_transaction, date_transaction, etat_paiement, mode_paiement, montant_total FROM transaction");


    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mode"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Montant"));

<<<<<<< HEAD


    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
     sendMailIfMaxDepense();
}


=======
    // Applique le modèle au QTableView
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
void MainWindow::on_pushButton_14_clicked()
{
    int id = ui->lineEdit_2->text().toInt();
    QString type = ui->comboBox->currentText();
<<<<<<< HEAD
    QString dateTime = ui->dateEdit->date().toString("yyyy-MM-dd");
=======
    QString dateTime = ui->dateEdit->date().toString("yyyy-MM-dd");  // Utilisation de QDateEdit
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    QString etat = ui->comboBox_3->currentText();
    QString mode = ui->comboBox_2->currentText();
    double montant = ui->lineEdit_3->text().toDouble();

<<<<<<< HEAD
    Transaction T(id, type, dateTime,etat, mode,  montant);
=======
    Transaction T(id, type, dateTime, mode, etat, montant);
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8

    bool test = T.ajouter();
    if (test) {
        QMessageBox::information(this, "Succès", "Transaction ajoutée.");
        refreshTableView();  // Actualiser l'affichage
<<<<<<< HEAD
        sendMailIfMaxDepense();
=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout,meme id.");
    }
}

<<<<<<< HEAD

=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
void MainWindow::on_pushButton_16_clicked()
{
    int id = ui->lineEdit_2->text().toInt();
    bool success = T.supprimer(id);

    if (success) {
        QMessageBox::information(this, "Succès", "Transaction supprimée.");
<<<<<<< HEAD
        refreshTableView();
=======
        refreshTableView();  // Actualiser l'affichage
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    } else {
        QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
    }
}

void MainWindow::on_pushButton_15_clicked()
{
    int id_transaction = ui->lineEdit_2->text().toInt();
    QString type_transaction = ui->comboBox->currentText();
    QString date_transaction = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString mode_paiement = ui->comboBox_2->currentText();
    QString etat_paiement = ui->comboBox_3->currentText();
    float montant_total = ui->lineEdit_3->text().toFloat();

    bool test = T.modifier(id_transaction, type_transaction, date_transaction, mode_paiement, etat_paiement, montant_total);
    if (test) {
        QMessageBox::information(this, "Succès", "Transaction modifiée.");
<<<<<<< HEAD
        refreshTableView();
        sendMailIfMaxDepense();
=======
        refreshTableView();  // Actualiser l'affichage
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification.");
    }
}
void MainWindow::on_pushButton_44_clicked()
{
<<<<<<< HEAD

    QSqlQueryModel *modelTableView2 = new QSqlQueryModel(this);

    QString searchField = ui->comboBox_17->currentText();
    QString searchValue = ui->lineEdit_26->text().trimmed();
=======
    QString searchField = ui->comboBox_17->currentText(); // Récupère le champ sélectionné (id, type ou date)
    QString searchValue = ui->lineEdit_26->text().trimmed(); // Récupère la valeur de recherche
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8

    if (searchField == "date du transaction") {
        // Vérifier si la date est valide
        QDate date = QDate::fromString(searchValue, "yyyy-MM-dd");
        if (!date.isValid()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer une date valide au format yyyy-MM-dd.");
            return;
        }

<<<<<<< HEAD

        modelTableView2->setQuery(QString("SELECT * FROM transaction WHERE date_transaction = TO_DATE('%1', 'YYYY-MM-DD')").arg(searchValue));

    } else if (searchField == "id du transaction") {
        // Recherche par ID
        modelTableView2->setQuery(QString("SELECT * FROM transaction WHERE id_transaction = %1").arg(searchValue.toInt()));

    } else if (searchField == "type du transaction") {
        // Recherche par type
        modelTableView2->setQuery(QString("SELECT * FROM transaction WHERE type_transaction LIKE '%%1%'").arg(searchValue));
    }


    if (modelTableView2->rowCount() == 0) {
        QMessageBox::information(this, "Résultat", "Aucun résultat trouvé pour la recherche.");
    } else {

        ui->tableView_2->setModel(modelTableView2);
        ui->tableView_2->resizeColumnsToContents(); // Ajuste les colonnes
    }
}


void MainWindow::on_comboBox_18_selectionChanged()
{

<<<<<<< HEAD
    QString selectedCriteria = ui->comboBox_18->currentText();

=======
=======
        // Exécuter la requête pour rechercher la date
        model->setQuery(QString("SELECT * FROM transaction WHERE date_transaction = TO_DATE('%1', 'YYYY-MM-DD')").arg(searchValue));

    } else if (searchField == "id du transaction") {
        // Recherche par ID
        model->setQuery(QString("SELECT * FROM transaction WHERE id_transaction = %1").arg(searchValue.toInt()));

    } else if (searchField == "type du transaction") {
        // Recherche par type
        model->setQuery(QString("SELECT * FROM transaction WHERE type_transaction LIKE '%%1%'").arg(searchValue));
    }

    // Vérifier si des résultats ont été trouvés
    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Résultat", "Aucun résultat trouvé pour la recherche.");
    } else {
        ui->tableView_2->setModel(model); // Met à jour l'affichage de la table
        ui->tableView_2->resizeColumnsToContents(); // Ajuste les colonnes
    }
}
void MainWindow::on_comboBox_18_selectionChanged()
{
    // Récupérer le critère sélectionné dans le QComboBox
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    QString selectedCriteria = ui->comboBox_18->currentText();  // Assurez-vous que le nom de votre comboBox est correct (ici 'comboBox_18')
>>>>>>> 5f0d2bc5ff53f00c67448f869f0670e933450b3d

    QString queryStr;
    if (selectedCriteria == "Date") {
        queryStr = "SELECT id_transaction, type_transaction, date_transaction, etat_paiement, mode_paiement, montant_total "
                   "FROM transaction ORDER BY date_transaction ASC";
    } else if (selectedCriteria == "Type") {
        queryStr = "SELECT id_transaction, type_transaction, date_transaction, etat_paiement, mode_paiement, montant_total "
                   "FROM transaction ORDER BY type_transaction ASC";
    } else if (selectedCriteria == "Montant") {
        queryStr = "SELECT id_transaction, type_transaction, date_transaction, etat_paiement, mode_paiement, montant_total "
                   "FROM transaction ORDER BY montant_total ASC";
    } else {
        QMessageBox::warning(this, "Erreur", "Critère de tri invalide.");
        return;
    }

<<<<<<< HEAD

=======
    // Exécuter la requête et mettre à jour le modèle
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors du tri des transactions : " + model->lastError().text());
    } else {
        // Mettre à jour l'affichage du QTableView
        ui->tableView_2->setModel(model);
        ui->tableView_2->resizeColumnsToContents();
    }
}
<<<<<<< HEAD
void MainWindow::generatePdfForPeriod(const QDate &startDate, const QDate &endDate) {
    // Ouvrir une boîte de dialogue pour sélectionner l'emplacement de sauvegarde du fichier PDF
    QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer PDF"), "", "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName.append(".pdf");
    }


    QPrinter pdfPrinter(QPrinter::HighResolution);
    pdfPrinter.setOutputFormat(QPrinter::PdfFormat);
    pdfPrinter.setOutputFileName(fileName);

    //  la taille de la page en A4
    QPageLayout pageLayout;
    pageLayout.setPageSize(QPageSize(QPageSize::A4));
    pdfPrinter.setPageLayout(pageLayout);

    // Initialiser HTML pour le PDF
    QString htmlContent;

    // Date
    QString todayDate = QDate::currentDate().toString("dd/MM/yyyy");

    // Construire le contenu HTML
    htmlContent += "<html><head>"
                   "<meta charset='UTF-8'>"
                   "<title>Rapport de Transactions</title>"
                   "<style>"
                   "body { font-family: Arial, sans-serif; margin: 25px; }"
                   ".header { font-size: 18px; margin-bottom: 15px; }"
                   ".title { text-align: center; }"
                   ".title h1 { margin-bottom: 10px; }"
                   ".title h4 { margin-top: 0; }"
                   "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                   "th, td { border: 1px solid #333; padding: 8px; text-align: left; }"
                   "th { background-color: #f0f0f0; font-weight: bold; }"
                   "</style>"
                   "</head><body>";

    //  titre
    htmlContent += "<div class='header'>Gestion des Transactions</div>";
    htmlContent += "<div class='title'><h1>Rapport de transactions</h1>"
                   "<h4>Période : " + startDate.toString("dd/MM/yyyy") + " - " + endDate.toString("dd/MM/yyyy") + "</h4>"
                   "<h4>Date : " + todayDate + "</h4></div>";

    // Début du tableau
    htmlContent += "<table><thead><tr>"
                   "<th>ID Transaction</th><th>Type</th><th>Date</th><th>État</th><th>Mode de Paiement</th><th>Montant</th>"
                   "</tr></thead><tbody>";

    // Exécuter la requête
    QSqlQuery query;
    query.prepare("SELECT id_transaction, type_transaction, date_transaction, etat_paiement, mode_paiement, montant_total "
                  "FROM transaction WHERE date_transaction BETWEEN TO_DATE(:startDate, 'YYYY-MM-DD') AND TO_DATE(:endDate, 'YYYY-MM-DD')");
    query.bindValue(":startDate", startDate.toString("yyyy-MM-dd"));
    query.bindValue(":endDate", endDate.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exécution de la requête : " + query.lastError().text());
        return;
    }

    // Remplir les lignes du tableau
    while (query.next()) {
        htmlContent += "<tr>";
        htmlContent += "<td>" + query.value("id_transaction").toString() + "</td>";
        htmlContent += "<td>" + query.value("type_transaction").toString() + "</td>";
        htmlContent += "<td>" + query.value("date_transaction").toDate().toString("dd/MM/yyyy") + "</td>";
        htmlContent += "<td>" + query.value("etat_paiement").toString() + "</td>";
        htmlContent += "<td>" + query.value("mode_paiement").toString() + "</td>";
        htmlContent += "<td>" + QString::number(query.value("montant_total").toDouble(), 'f', 2) + " TND</td>";
        htmlContent += "</tr>";
    }

    // Fermer le tableau et le HTML
    htmlContent += "</tbody></table></body></html>";

    // Créer un document texte pour le rendu HTML
    QTextDocument document;
    document.setHtml(htmlContent);


    document.print(&pdfPrinter);


    QMessageBox::information(this, "Succès", "PDF généré avec succès !");
}
void MainWindow::on_pushButton_stat_clicked()
{
    QDate dateDebut = ui->dateEdit_5->date();
    QDate dateFin = ui->dateEdit_6->date();

    // Créer une série pour le graphique
    QPieSeries *series = new QPieSeries();

    QSqlQuery query;
    query.prepare("SELECT type_transaction, SUM(montant_total) "
                  "FROM transaction "
                  "WHERE date_transaction BETWEEN :dateDebut AND :dateFin "
                  "GROUP BY type_transaction");
    query.bindValue(":dateDebut", dateDebut);
    query.bindValue(":dateFin", dateFin);

    if (!query.exec()) {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données : " + query.lastError().text());
        return;
    }

    // Variables pour calculer le total
    double totalMontant = 0.0;
    QList<QPair<QString, double>> data;

    while (query.next()) {
        QString typeTransaction = query.value(0).toString();
        double montant = query.value(1).toDouble();
        totalMontant += montant;
        data.append(qMakePair(typeTransaction, montant));
    }

    // Vérifier que le total n'est pas nul
    if (totalMontant == 0) {
        QMessageBox::information(this, "Aucune donnée", "Il n'y a pas de revenus ou de dépenses dans cette période.");
        return;
    }

    // Ajouter les données à la série avec les pourcentages
    for (const auto &entry : data) {
        QString typeTransaction = entry.first;
        double montant = entry.second;
        double pourcentage = (montant / totalMontant) * 100.0;

        if (typeTransaction == "revenu") {
            QPieSlice *sliceRevenu = series->append(
                QString("Revenus: %1%").arg(pourcentage, 0, 'f', 1), montant);
            sliceRevenu->setBrush(QColor(173, 216, 230)); // Couleur bleu ciel pour les revenus
            sliceRevenu->setLabelVisible(true);
        } else if (typeTransaction == "depense") {
            QPieSlice *sliceDepense = series->append(
                QString("Dépenses: %1%").arg(pourcentage, 0, 'f', 1), montant);
            sliceDepense->setBrush(Qt::blue); // Couleur bleu pour les dépenses
            sliceDepense->setLabelVisible(true);
        }
    }

    // Créer un objet QChart pour afficher la série de graphique en secteurs
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Revenus et Dépenses entre " +
                    dateDebut.toString("dd/MM/yyyy") + " et " +
                    dateFin.toString("dd/MM/yyyy"));

    // Ajouter des options d'affichage
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Créer un QChartView pour afficher le graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Afficher le graphique dans une nouvelle fenêtre
    chartView->resize(800, 600);
    chartView->show();
}

void MainWindow::afficherPageTransactions() {
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::afficherPageChambres() {
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::setupTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    trayIcon->setToolTip("Gestion des transactions");


    if (!trayIcon->isSystemTrayAvailable()) {
        QMessageBox::critical(this, "Erreur", "Le système ne supporte pas les notifications de la barre système.");
        return;
    }

    trayIcon->setVisible(true);



    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconClicked);
}



void MainWindow::onTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {

        qDebug() << "Icône tray cliquée";
    }
}

void MainWindow::readyRead() {
    qDebug() << "Données prêtes à être lues.";
}

void MainWindow::connected() {
    qDebug() << "Connecté au serveur.";
}
std::vector<Transaction> MainWindow::chargerTransactionsDepuisBase()
{
    std::vector<Transaction> transactions;


    QSqlQuery query("SELECT id_transaction, type_transaction, date_transaction,etat_paiement, mode_paiement, montant_total FROM transaction");


    while (query.next()) {
        int id = query.value(0).toInt();
        QString type = query.value(1).toString();
         QString date = query.value(3).toString();
         QString etat = query.value(4).toString();
        QString mode = query.value(2).toString();
        double montant = query.value(5).toDouble();


        transactions.push_back(Transaction(id, type, date, etat, mode, montant));
    }

    return transactions;
}

void MainWindow::calculerTVA()
{
    std::vector<Transaction> transactions = chargerTransactionsDepuisBase();

    double tauxTVA = 0.19;
    double totalRevenus = 0.0;
    double totalDepenses = 0.0;


    for (const auto& trans : transactions) {
        if (trans.gettype_transaction() == "revenu") {
            totalRevenus += trans.getmontant_total();
        } else if (trans.gettype_transaction() == "depense") {
            totalDepenses += trans.getmontant_total();
        }
    }

    double tvaRevenus = totalRevenus * tauxTVA;
    double tvaDepenses = totalDepenses * tauxTVA;
    double soldeTVA = tvaRevenus - tvaDepenses;


    QString message = QString("Total des Revenus : %1\nTotal des Dépenses : %2\n")
                          .arg(totalRevenus)
                          .arg(totalDepenses);
    message += QString("TVA sur les Revenus : %1\nTVA sur les Dépenses : %2\n")
                  .arg(tvaRevenus)
                  .arg(tvaDepenses);
    message += QString("Solde TVA : %1").arg(soldeTVA);

    ui->lineEdit_25->setText(message);


    if (tvaRevenus > tvaDepenses) {
        QString notificationMessage = QString(
            "Attention : Vous devez payer la TVA. \n"
            "TVA sur les revenus : %1\n"
            "TVA sur les dépenses : %2\n"
            "Montant TVA à payer : %3")
            .arg(tvaRevenus)
            .arg(tvaDepenses)
            .arg(soldeTVA);


        trayIcon->showMessage("Notification TVA", notificationMessage, QSystemTrayIcon::Information, 5000);

        qDebug() << "Notification de TVA envoyée : " << notificationMessage;
    }
}
void MainWindow::on_pushButton_calculerTVA_clicked()
{
    calculerTVA();
}

void MainWindow::sendMailIfMaxDepense() {
    double sum = T.calculerSommeDepenses();

    if (sum > maxDepense) {
        mailling mailer;

        QString to = "trabelsiasma20@gmail.com";
        QString subject = QString("Max Depense Reached: %1").arg(maxDepense);
        QString body = QString("Cher Directeur,\n\n"
                               "Nous vous informons que le plafond des dépenses a été atteint. "
                               "Le montant actuel des dépenses maximales est : %1.\n\n"
                               "Un email vous a été envoyé avec succès pour vous notifier à ce sujet.\n"
                               "Veuillez prendre les mesures nécessaires si besoin.\n\n"
                               "Cordialement,\n"
                               "Smart Hotel").arg(maxDepense);

        bool res = mailer.sendEmail(
                    to,
                    subject,
                    body
        );

        if (res) {
            QMessageBox::information(nullptr, QObject::tr("Max Depense Reached"),
                        QObject::tr("Max Depense Reached : email sent successfully.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Max Depense Reached"),
                        QObject::tr("Max Depense Reached : error sending mail.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);

        }


=======
void MainWindow::generatePdf()
{
    // Récupérer la date unique spécifiée par l'utilisateur
    QString date = ui->dateEdit_3->date().toString("yyyy-MM-dd");

    // Requête SQL pour récupérer les transactions pour la date spécifiée
    QString queryStr = QString("SELECT * FROM transaction WHERE date_transaction = '%1'").arg(date);
    QSqlQuery query;

    if (!query.exec(queryStr)) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exécution de la requête SQL : " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::information(this, "Aucune transaction", "Aucune transaction trouvée pour la date spécifiée.");
        return; // Sortie si aucune transaction n'est trouvée
    }

    // Créer un QPrinter pour générer le PDF
    QPrinter printer;
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFormat(QPrinter::PdfFormat);

    // Demander à l'utilisateur où enregistrer le PDF
    QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer le PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty()) {
        return; // Si l'utilisateur annule, on arrête la fonction
    }
    printer.setOutputFileName(fileName);

    // Initialiser QPainter pour dessiner dans le PDF
    QPainter painter;
    if (painter.begin(&printer)) {
        int yOffset = 50; // Début de l'impression

        // Titre du PDF
        painter.setFont(QFont("Arial", 16));
        painter.drawText(200, yOffset, "Récapitulatif des Transactions");
        yOffset += 40;
        painter.setFont(QFont("Arial", 12));

        // Réinitialiser le curseur de la requête pour parcourir les résultats
        query.first();
        do {
            QString id = query.value(0).toString();
            QString type = query.value(1).toString();
            QString date = query.value(2).toString();
            QString etat = query.value(3).toString();
            QString mode = query.value(4).toString();
            double montant = query.value(5).toDouble();

            painter.drawText(100, yOffset, QString("ID: %1, Type: %2, Date: %3, État: %4, Mode: %5, Montant: %6")
                                     .arg(id).arg(type).arg(date).arg(etat).arg(mode).arg(montant));
            yOffset += 20;

            // Vérifier si la hauteur dépasse la page et passer à la page suivante
            if (yOffset > printer.pageRect(QPrinter::Point).height() - 50) {
                printer.newPage();
                yOffset = 50; // Réinitialiser le décalage Y pour la nouvelle page
            }
        } while (query.next());

        painter.end();  // Terminer l'écriture du PDF
        QMessageBox::information(this, "Succès", "PDF généré avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de générer le PDF.");
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    }
}

void MainWindow::handleArduinoData(QString data)
{
    // Divise les données reçues par Arduino (format attendu : "Chambre 1 : 1" ou "Chambre 2 : 0")
    QStringList parts = data.split(" : ");
    if (parts.size() != 2) {
        qDebug() << "Format de données invalide :" << data;
        return;
    }

    QString chambre = parts[0].trimmed(); // "Chambre 1" ou "Chambre 2"
    QString status = parts[1].trimmed(); // "1" si flamme détectée, "0" sinon

    static bool chambre1Alert = false; // État d'alerte pour la chambre 1
    static bool chambre2Alert = false; // État d'alerte pour la chambre 2

    int idChambre = 0;

    // Associer le capteur à l'ID de la chambre
    if (chambre == "Chambre 1") {
        idChambre = 44;
        chambre1Alert = (status == "1"); // Mémoriser l'état d'alerte
    } else if (chambre == "Chambre 2") {
        idChambre = 22;
        chambre2Alert = (status == "1"); // Mémoriser l'état d'alerte
    }

    // Si une alerte a été détectée dans au moins une chambre
    if ((chambre1Alert || chambre2Alert) && idChambre != 0) {
        QString message;

        // Récupère les informations de la chambre 1 si une flamme est détectée
        if (chambre1Alert) {
            QSqlQuery query;
            query.prepare("SELECT * FROM CHAMBRE WHERE ID_CHAMBRE = 44");
            if (query.exec() && query.next()) {
                QString categorie = query.value("CATEGORIE").toString();
                QString type = query.value("TYPE_CH").toString();
                QString statut = query.value("STATUT_CHAMBRE").toString();
                double tarif = query.value("TARIF").toDouble();

                message += "Chambre 1 (ID 44) :\n"
                           "- Catégorie : " + categorie + "\n"
                           "- Type : " + type + "\n"
                           "- Statut : " + statut + "\n"
                           "- Tarif : " + QString::number(tarif, 'f', 2) + " TND\n\n";
            } else {
                qDebug() << "Erreur SQL ou chambre 1 non trouvée : " << query.lastError().text();
                message += "Chambre 1 (ID 44) : Informations indisponibles.\n\n";
            }
        }

        // Récupère les informations de la chambre 2 si une flamme est détectée
        if (chambre2Alert) {
            QSqlQuery query;
            query.prepare("SELECT * FROM CHAMBRE WHERE ID_CHAMBRE = 22");
            if (query.exec() && query.next()) {
                QString categorie = query.value("CATEGORIE").toString();
                QString type = query.value("TYPE_CH").toString();
                QString statut = query.value("STATUT_CHAMBRE").toString();
                double tarif = query.value("TARIF").toDouble();

                message += "Chambre 2 (ID 22) :\n"
                           "- Catégorie : " + categorie + "\n"
                           "- Type : " + type + "\n"
                           "- Statut : " + statut + "\n"
                           "- Tarif : " + QString::number(tarif, 'f', 2) + " TND\n";
            } else {
                qDebug() << "Erreur SQL ou chambre 2 non trouvée : " << query.lastError().text();
                message += "Chambre 2 (ID 22) : Informations indisponibles.\n";
            }
        }

        // Affiche l'alerte combinée si les deux chambres détectent une flamme simultanément
        if (chambre1Alert && chambre2Alert) {
            QMessageBox::critical(this, "Alerte Incendie !",
                                  "Des incendies ont été détectés dans les deux chambres :\n\n" + message);
        }
        // Affiche une alerte individuelle si une seule chambre détecte une flamme
        else if (chambre1Alert || chambre2Alert) {
            QMessageBox::warning(this, "Alerte Incendie !",
                                 "Un incendie a été détecté :\n\n" + message);
        }

        // Réinitialise les alertes après traitement
        chambre1Alert = chambre2Alert = false;
    }
}


