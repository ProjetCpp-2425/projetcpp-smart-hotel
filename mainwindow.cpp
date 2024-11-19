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
=======
#include <QFile>
#include <QTextStream>
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), T(0, "", "", "", "", 0.0), model(new QSqlQueryModel(this))
{
<<<<<<< HEAD


    ui->setupUi(this);
 setupTrayIcon();


    QDoubleValidator *montantValidator = new QDoubleValidator(0, 1000000, 2, this); // Limite le montant entre 0 et 1 000 000 avec 2 décimales
    montantValidator->setLocale(QLocale::English); // Utilise le point (.) comme séparateur décimal
    ui->lineEdit_3->setValidator(montantValidator);// Applique le validateur au champ du montant

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


    maxDepense = 100.0;

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
}

void MainWindow::refreshTableView()
{
<<<<<<< HEAD

=======
    // Configure la requête pour afficher toutes les transactions
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    model->setQuery("SELECT id_transaction, type_transaction, date_transaction, etat_paiement, mode_paiement, montant_total FROM transaction");

    // Définit les en-têtes pour chaque colonne
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

    // Créer une requête SQL pour trier en fonction du critère choisi
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

    // Configurer l'imprimante pour la sortie PDF
    QPrinter pdfPrinter(QPrinter::HighResolution);
    pdfPrinter.setOutputFormat(QPrinter::PdfFormat);
    pdfPrinter.setOutputFileName(fileName);

    // Utiliser QPageLayout pour définir la taille de la page en A4
    QPageLayout pageLayout;
    pageLayout.setPageSize(QPageSize(QPageSize::A4));
    pdfPrinter.setPageLayout(pageLayout);

    // Initialiser le contenu HTML pour le PDF
    QString htmlContent;

    // Date actuelle
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

    // En-tête et titre
    htmlContent += "<div class='header'>Gestion des Transactions</div>";
    htmlContent += "<div class='title'><h1>Rapport de transactions</h1>"
                   "<h4>Période : " + startDate.toString("dd/MM/yyyy") + " - " + endDate.toString("dd/MM/yyyy") + "</h4>"
                   "<h4>Date : " + todayDate + "</h4></div>";

    // Début du tableau
    htmlContent += "<table><thead><tr>"
                   "<th>ID Transaction</th><th>Type</th><th>Date</th><th>État</th><th>Mode de Paiement</th><th>Montant</th>"
                   "</tr></thead><tbody>";

    // Exécuter la requête pour obtenir les transactions dans la période spécifiée
    QSqlQuery query;
    query.prepare("SELECT id_transaction, type_transaction, date_transaction, etat_paiement, mode_paiement, montant_total "
                  "FROM transaction WHERE date_transaction BETWEEN TO_DATE(:startDate, 'YYYY-MM-DD') AND TO_DATE(:endDate, 'YYYY-MM-DD')");
    query.bindValue(":startDate", startDate.toString("yyyy-MM-dd"));
    query.bindValue(":endDate", endDate.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exécution de la requête : " + query.lastError().text());
        return;
    }

    // Remplir les lignes du tableau avec les résultats de la requête
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

    // Imprimer le document dans le fichier PDF
    document.print(&pdfPrinter);

    // Confirmation de succès
    QMessageBox::information(this, "Succès", "PDF généré avec succès !");
}
void MainWindow::on_pushButton_stat_clicked()
{
    // Récupérer les dates de début et de fin depuis les champs d'entrée
    QDate dateDebut = ui->dateEdit_5->date();
    QDate dateFin = ui->dateEdit_6->date();

    // Créer une série de graphique en secteurs pour afficher les revenus et les dépenses
    QPieSeries *series = new QPieSeries();

    // Exécuter la requête SQL pour récupérer les données de la période spécifiée
    QSqlQuery query;
    query.prepare("SELECT type_transaction, SUM(montant_total) "
                  "FROM transaction "
                  "WHERE date_transaction BETWEEN :dateDebut AND :dateFin "
                  "GROUP BY type_transaction");
    query.bindValue(":dateDebut", dateDebut);
    query.bindValue(":dateFin", dateFin);

    // Vérifier si la requête SQL s'exécute correctement
    if (!query.exec()) {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données : " + query.lastError().text());
        return;
    }

    // Parcourir les résultats de la requête pour ajouter des segments à la série
    while (query.next()) {
        QString typeTransaction = query.value(0).toString();
        double montant = query.value(1).toDouble();

        if (typeTransaction == "revenu") {
            QPieSlice *sliceRevenu = series->append("Revenus", montant);
            sliceRevenu->setBrush(QColor(173, 216, 230)); // Couleur bleu ciel pour les revenus
            sliceRevenu->setLabelVisible(true);
        } else if (typeTransaction == "depense") {
            QPieSlice *sliceDepense = series->append("Dépenses", montant);
            sliceDepense->setBrush(Qt::blue); // Couleur bleu pour les dépenses
            sliceDepense->setLabelVisible(true);
        }
    }

    // Créer un objet QChart pour afficher la série de graphique en secteurs
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Revenus et Dépenses entre " + dateDebut.toString("dd/MM/yyyy") + " et " + dateFin.toString("dd/MM/yyyy"));

    // Créer un QChartView pour afficher le graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Afficher le graphique dans une nouvelle fenêtre ou un widget de la fenêtre principale
    chartView->resize(800, 600);
    chartView->show();
}
void MainWindow::afficherPageTransactions() {
    ui->stackedWidget->setCurrentIndex(2);
}

// Fonction pour afficher la page des chambres
void MainWindow::afficherPageChambres() {
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::setupTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    trayIcon->setToolTip("Gestion des transactions");

    // Vérifier si le système supporte les notifications
    if (!trayIcon->isSystemTrayAvailable()) {
        QMessageBox::critical(this, "Erreur", "Le système ne supporte pas les notifications de la barre système.");
        return;
    }

    trayIcon->setVisible(true);

    // Appeler la fonction pour vérifier les dépenses et afficher une notification si nécessaire
    checkBudgetExceeded();  // Vérification du budget des dépenses

    // Connecter l'activation de l'icône à une fonction
    trayIcon->showMessage("attention", "vous avez dépasser le seuil des dépenses", QSystemTrayIcon::Information, 5000);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconClicked);
}


void MainWindow::checkBudgetExceeded()
{
    qDebug() << "Vérification du dépassement du budget...";

    // Requête SQL pour obtenir la somme totale des dépenses
    QSqlQuery query;
    query.prepare("SELECT SUM(montant_total) FROM transaction WHERE type_transaction = 'depense'");

    if (!query.exec()) {
        qDebug() << "Erreur SQL: " << query.lastError().text();
        return;
    }

    if (query.next()) {
        double totalSpent = query.value(0).toDouble();  // Récupérer la somme des dépenses
        qDebug() << "Total des dépenses: " << totalSpent;

        double budgetLimit = 1000.0;  // Définir un seuil de budget
        if (totalSpent > budgetLimit) {
            // Créer un message avec le montant dépassé et afficher la notification
            QString message = QString("Budget des dépenses dépassé de %1 !").arg(totalSpent - budgetLimit);
            trayIcon->showMessage("Alerte Budget",
                                  message,
                                  QSystemTrayIcon::Warning,
                                  5000);  // Afficher pendant 5 secondes
            qDebug() << "Notification affichée: " << message;
        } else {
            qDebug() << "Le budget des dépenses est dans la limite.";
        }
    }
}
void MainWindow::onTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        // Action à exécuter quand l'icône est cliquée
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

    // Requête SQL pour récupérer les transactions
    QSqlQuery query("SELECT id_transaction, type_transaction, date_transaction,etat_paiement, mode_paiement, montant_total FROM transaction");

    // Boucle pour récupérer chaque transaction et les ajouter dans le vecteur
    while (query.next()) {
        int id = query.value(0).toInt();
        QString type = query.value(1).toString();
         QString date = query.value(3).toString();
         QString etat = query.value(4).toString();
        QString mode = query.value(2).toString();
        double montant = query.value(5).toDouble();

        // Ajouter l'objet Transaction dans le vecteur
        transactions.push_back(Transaction(id, type, date, etat, mode, montant));
    }

    return transactions;
}

void MainWindow::calculerTVA()
{
    std::vector<Transaction> transactions = chargerTransactionsDepuisBase();

    double tauxTVA = 0.20;  // Taux de TVA à 20%
    double totalRevenus = 0.0;
    double totalDepenses = 0.0;

    // Parcourir les transactions et calculer les montants de TVA
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

    // Affichage du résultat dans un QLabel (ou autre widget)
    QString message = QString("Total des Revenus : %1\nTotal des Dépenses : %2\n")
                          .arg(totalRevenus)
                          .arg(totalDepenses);
    message += QString("TVA sur les Revenus : %1\nTVA sur les Dépenses : %2\n")
                  .arg(tvaRevenus)
                  .arg(tvaDepenses);
    message += QString("Solde TVA : %1").arg(soldeTVA);

    ui->lineEdit_25->setText(message);  // Affichage du résultat dans le widget de l'interface utilisateur
}
void MainWindow::on_pushButton_calculerTVA_clicked()
{
    calculerTVA();  // Appeler la fonction de calcul de TVA
}

void MainWindow::sendMailIfMaxDepense() {
    double sum = T.calculerSommeDepenses();

    if (sum > maxDepense) {
        mailling mailer;

        QString to = "trabelsiasma20@gmail.com";
        QString subject = QString("Max Depense Reached: %1").arg(maxDepense);
        QString body = QString("Dear Director,\n\n"
                              "We are writing to inform you that the maximum expenditure has been reached. "
                              "The current max depense is: %1.\n\n"
                              "An email has been sent successfully to notify you about this.\n"
                              "Please take the necessary action if required.\n\n"
                              "Best regards,\n"
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
