#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QDoubleValidator>
#include <QSqlQueryModel>
#include <QDate>
#include <QFileDialog>
#include <QPainter>
#include <QPrinter>
#include <QFile>
#include <QTextStream>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), T(0, "", "", "", "", 0.0), model(new QSqlQueryModel(this))
{
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshTableView()
{
    // Configure la requête pour afficher toutes les transactions
    model->setQuery("SELECT id_transaction, type_transaction, date_transaction, etat_paiement, mode_paiement, montant_total FROM transaction");

    // Définit les en-têtes pour chaque colonne
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mode"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Montant"));

    // Applique le modèle au QTableView
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_pushButton_14_clicked()
{
    int id = ui->lineEdit_2->text().toInt();
    QString type = ui->comboBox->currentText();
    QString dateTime = ui->dateEdit->date().toString("yyyy-MM-dd");  // Utilisation de QDateEdit
    QString etat = ui->comboBox_3->currentText();
    QString mode = ui->comboBox_2->currentText();
    double montant = ui->lineEdit_3->text().toDouble();

    Transaction T(id, type, dateTime, mode, etat, montant);

    bool test = T.ajouter();
    if (test) {
        QMessageBox::information(this, "Succès", "Transaction ajoutée.");
        refreshTableView();  // Actualiser l'affichage
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout,meme id.");
    }
}

void MainWindow::on_pushButton_16_clicked()
{
    int id = ui->lineEdit_2->text().toInt();
    bool success = T.supprimer(id);

    if (success) {
        QMessageBox::information(this, "Succès", "Transaction supprimée.");
        refreshTableView();  // Actualiser l'affichage
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
        refreshTableView();  // Actualiser l'affichage
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification.");
    }
}
void MainWindow::on_pushButton_44_clicked()
{
    QString searchField = ui->comboBox_17->currentText(); // Récupère le champ sélectionné (id, type ou date)
    QString searchValue = ui->lineEdit_26->text().trimmed(); // Récupère la valeur de recherche

    if (searchField == "date du transaction") {
        // Vérifier si la date est valide
        QDate date = QDate::fromString(searchValue, "yyyy-MM-dd");
        if (!date.isValid()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer une date valide au format yyyy-MM-dd.");
            return;
        }

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

    // Exécuter la requête et mettre à jour le modèle
    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors du tri des transactions : " + model->lastError().text());
    } else {
        // Mettre à jour l'affichage du QTableView
        ui->tableView_2->setModel(model);
        ui->tableView_2->resizeColumnsToContents();
    }
}
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
    }
}
