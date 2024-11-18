#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTableView>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <algorithm>
#include <QSqlDatabase>
#include <QDebug>

#include <QProcess>
#include "notesdialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_afficher, &QPushButton::clicked, this, &MainWindow::refreshTableView);
    connect(ui->buttonRechercher, &QPushButton::clicked, this, &MainWindow::on_buttonRechercher_clicked);
    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::on_pushButton_pdf_clicked);
    connect(ui->pushButton_QR, &QPushButton::clicked, this, &MainWindow::on_pushButton_QR_clicked);




}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_ajouter_clicked()
{
    QString idText = ui->lineEdit_id_chambre->text();
    QString prixText = ui->lineEdit_prix->text();

    bool idValid;
    int id = idText.toInt(&idValid);
    if (!idValid || idText.length() > 3) {
        QMessageBox::warning(this, tr("Invalid ID"),
                             tr("L'ID de la chambre doit être un entier et ne peut pas dépasser 3 chiffres."));
        return;
    }

    bool prixValid;
    float prix = prixText.toFloat(&prixValid);
    if (!prixValid) {
        QMessageBox::warning(this, tr("Invalid Price"),
                             tr("Le prix doit être un nombre valide et ne peut pas contenir de caractères."));
        return;
    }

    QString type;
    if (ui->radioButton_normal->isChecked()) {
        type = "normal";
    } else if (ui->radioButton_double->isChecked()) {
        type = "double";
    } else if (ui->radioButton_suite->isChecked()) {
        type = "suite";
    }

    QString categorie = ui->comboBox_categorie->currentText();

    chambre.setIdChambre(id);
    chambre.setTarif(prix);
    chambre.setType(type);
    chambre.setCategorie(categorie);
    chambre.setStatutChambre("Unavailable");
    bool success = chambre.ajouter();

    if (success) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\nClick Cancel to exit."),
                                 QMessageBox::Cancel);
         refreshTableView();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\nClick Cancel to exit."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
    QString idText = ui->lineEdit_id_chambre->text();
    QString prixText = ui->lineEdit_prix->text();

    bool idValid;
    int id = idText.toInt(&idValid);
    if (!idValid || idText.length() > 3) {
        QMessageBox::warning(this, tr("Invalid ID"),
                             tr("L'ID de la chambre doit être un entier et ne peut pas dépasser 3 chiffres."));
        return;
    }

    bool prixValid;
    float prix = prixText.toFloat(&prixValid);
    if (!prixValid) {
        QMessageBox::warning(this, tr("Invalid Price"),
                             tr("Le prix doit être un nombre valide et ne peut pas contenir de caractères."));
        return;
    }

    QString type;
    if (ui->radioButton_normal->isChecked()) {
        type = "normal";
    } else if (ui->radioButton_double->isChecked()) {
        type = "double";
    } else if (ui->radioButton_suite->isChecked()) {
        type = "suite";
    }

    QString categorie = ui->comboBox_categorie->currentText();
    QString statut_chambre = "Unavailable";

    bool success = chambre.modifier(id, categorie, type, statut_chambre, prix);

    if (success) {
        QMessageBox::information(this, tr("Modification réussie"),
                                 tr("La chambre a été modifiée avec succès."));
         refreshTableView();
    } else {
        QMessageBox::critical(this, tr("Erreur de modification"),
                              tr("L'ID de la chambre n'existe pas. La modification n'a pas été effectuée."));
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    int id = ui->lineEdit_id_chambre->text().toInt();
    bool success = chambre.supprimer(id);

    if (success) {
        QMessageBox::information(this, tr("Suppression réussie"),
                                 tr("La chambre a été supprimée avec succès."));
       refreshTableView();
    } else {
        QMessageBox::critical(this, tr("Erreur de suppression"),
                              tr("L'ID de la chambre n'existe pas. La suppression n'a pas été effectuée."));
    }
}

void MainWindow::refreshTableView()
{
    Chambre ch;
    QSqlQueryModel *model = ch.afficher();

    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "No Matching Rooms",
                                 "No rooms found in the database.",
                                 QMessageBox::Ok);
        delete model;
        return;
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Chambre"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Tarif"));

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}





void MainWindow::on_buttonRechercher_clicked()
{
    QString input = ui->lineEdit_recherche_3->text().trimmed();
    QString modeRecherche = ui->comboBox_18->currentText().trimmed();
    qDebug() << "Mode de recherche sélectionné :" << modeRecherche;

    if (modeRecherche == "Rechercher_dans_une_Fourchette_de_Prix") {
        QRegularExpression regex("^\\d{1,4} \\d{1,4}$");
        if (!regex.match(input).hasMatch()) {
            QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer deux nombres (1-9999) séparés par un espace.");
            return;
        }

        QStringList prix = input.split(" ");
        int minPrix = prix[0].toInt();
        int maxPrix = prix[1].toInt();

        if (minPrix > maxPrix) {
            QMessageBox::warning(this, "Erreur de saisie", "Le premier prix doit être inférieur ou égal au second.");
            return;
        }

        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery query;
        query.prepare("SELECT id_chambre, categorie, type, statut_chambre, tarif "
                      "FROM chambre WHERE tarif BETWEEN :minPrix AND :maxPrix");
        query.bindValue(":minPrix", minPrix);
        query.bindValue(":maxPrix", maxPrix);

        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur SQL", "Erreur lors de l'exécution de la requête : " + query.lastError().text());
            return;
        }

        model->setQuery(std::move(query));
        ui->tableView_4->setModel(model);
        ui->tableView_4->resizeColumnsToContents();
    }
    else if (modeRecherche == "Rechercher_par_ID") {
        QRegularExpression regex("^\\d+$");
        if (!regex.match(input).hasMatch()) {
            QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un identifiant valide (nombre entier).");
            return;
        }

        int idChambre = input.toInt();

        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery query;
        query.prepare("SELECT id_chambre, categorie, type, statut_chambre, tarif "
                      "FROM chambre WHERE id_chambre = :idChambre");
        query.bindValue(":idChambre", idChambre);

        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur SQL", "Erreur lors de l'exécution de la requête : " + query.lastError().text());
            return;
        }

        if (query.size() == 0) {
            QMessageBox::information(this, "Aucune correspondance", "Aucune chambre trouvée avec cet identifiant.");
            return;
        }

        model->setQuery(std::move(query));
        ui->tableView_4->setModel(model);
        ui->tableView_4->resizeColumnsToContents();
    }
    else {
        QMessageBox::warning(this, "Mode inconnu", "Mode de recherche inconnu");
    }
}

void MainWindow::on_buttonTrier_4_clicked()
{
    QString modeTri = ui->comboBox_10->currentText().trimmed();
    qDebug() << "Selected sorting mode: " << modeTri;

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    if (modeTri == "Trier_par_Prix") {
        query.prepare("SELECT id_chambre, categorie, type, statut_chambre, tarif "
                      "FROM chambre ORDER BY tarif ASC");
    }
    else if (modeTri == "Trier_par_Type") {
        query.prepare("SELECT id_chambre, categorie, type, statut_chambre, tarif "
                      "FROM chambre ORDER BY CASE "
                      "WHEN type = 'suite' THEN 1 "
                      "WHEN type = 'double' THEN 2 "
                      "WHEN type = 'simple' THEN 3 "
                      "END ASC");
    }
    else {
        QMessageBox::warning(this, "Unknown sorting mode", "Mode de tri inconnu.");
        return;
    }

    if (!query.exec()) {
        QMessageBox::critical(this, "SQL Error", "Error executing the query: " + query.lastError().text());
        return;
    }

    qDebug() << "Query executed successfully!";

    if (query.size() == 0) {
        QMessageBox::information(this, "No results", "No rooms found.");
    } else {
        model->setQuery(query);

        model->setHeaderData(0, Qt::Horizontal, "ID Chambre");
        model->setHeaderData(1, Qt::Horizontal, "Catégorie");
        model->setHeaderData(2, Qt::Horizontal, "Type");
        model->setHeaderData(3, Qt::Horizontal, "Statut Chambre");
        model->setHeaderData(4, Qt::Horizontal, "Tarif");

        ui->tableView_4->setModel(model);
        ui->tableView_4->resizeColumnsToContents();

        qDebug() << "Data displayed in the table.";
    }
}

void MainWindow::on_pushButton_pdf_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT type, COUNT(*) as count FROM chambre GROUP BY type");

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to retrieve data: " + query.lastError().text());
        return;
    }

    QMap<QString, int> typeCounts;
    int totalRooms = 0;

    while (query.next()) {
        QString type = query.value("type").toString();
        int count = query.value("count").toInt();
        typeCounts[type] = count;
        totalRooms += count;
    }

    if (totalRooms == 0) {
        QMessageBox::information(this, "No Data", "No rooms found in the database.");
        return;
    }

    QMap<QString, double> typePercentages;
    for (auto it = typeCounts.begin(); it != typeCounts.end(); ++it) {
        typePercentages[it.key()] = (it.value() / (double)totalRooms) * 100.0;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "*.pdf");
    if (fileName.isEmpty()) return; // User canceled the dialog
    if (!fileName.endsWith(".pdf")) fileName += ".pdf";

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMargins(20, 20, 20, 20));

    QPainter painter(&pdfWriter);

    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(200, 100, "Statistiques par Type");

    painter.setFont(QFont("Arial", 12));
    int y = 500; // Increased distance from the title
    int lineSpacing = 500; // Increased spacing between lines

    for (auto it = typePercentages.begin(); it != typePercentages.end(); ++it) {
        QString line = QString("%1: %2%").arg(it.key()).arg(QString::number(it.value(), 'f', 2));
        painter.drawText(100, y, line);
        y += lineSpacing; // Increment y-position for the next line
    }

    painter.end();

    QMessageBox::information(this, "PDF Created", "The PDF has been successfully created.");
}








void MainWindow::on_pushButton_statistique_clicked()
{


    // Définir les fourchettes de prix
    int range_0_100 = 0;
    int range_101_200 = 0;
    int range_201_500 = 0;
    int range_500_plus = 0;

    // Requête SQL pour récupérer les tarifs des chambres
    QSqlQuery query;
    if (!query.exec("SELECT tarif FROM chambre")) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible de récupérer les données : " + query.lastError().text());
        return;
    }

    // Parcourir les résultats et compter les chambres dans chaque fourchette de prix
    while (query.next()) {
        int tarif = query.value(0).toInt();
        if (tarif <= 100) {
            range_0_100++;
        } else if (tarif <= 200) {
            range_101_200++;
        } else if (tarif <= 500) {
            range_201_500++;
        } else {
            range_500_plus++;
        }
    }

    // Créer un ensemble de barres pour chaque fourchette de prix
    QBarSet *setRanges = new QBarSet("Nombre de Chambres");
    *setRanges << range_0_100 << range_101_200 << range_201_500 << range_500_plus;

    // Ajouter l'ensemble de barres à une série de barres
    QBarSeries *series = new QBarSeries();
    series->append(setRanges);

    // Créer un graphique et ajouter la série de barres
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Classement des chambres par fourchette de prix");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Définir les catégories pour l'axe X
    QStringList categories;
    categories << "0 - 100" << "101 - 200" << "201 - 500" << "500+";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Fourchette de Prix (unités)");
    axisX->setLabelsAngle(-45);  // Ajuster l'angle pour plus de lisibilité
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Définir l'axe Y pour le nombre de chambres
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de Chambres");
    int maxRange = std::max({range_0_100, range_101_200, range_201_500, range_500_plus});
    axisY->setRange(0, maxRange + 5);  // Ajouter une marge pour l'affichage
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Créer une vue pour le graphique et l'afficher dans une nouvelle fenêtre
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow *chartWindow = new QMainWindow(this);
    chartWindow->setCentralWidget(chartView);
    chartWindow->resize(800, 600);
    chartWindow->setWindowTitle("Statistiques des Chambres par Fourchette de Prix");
    chartWindow->show();

    // Pour débogage, afficher les valeurs dans la console
    qDebug() << "Chambres dans chaque fourchette de prix :";
    qDebug() << "0 - 100 unités :" << range_0_100;
    qDebug() << "101 - 200 unités :" << range_101_200;
    qDebug() << "201 - 500 unités :" << range_201_500;
    qDebug() << "500+ unités :" << range_500_plus;
}


void MainWindow::on_pushButton_QR_clicked()
{

    // Répertoire pour enregistrer les QR codes
    QString outputDir = QString("%1/QR_Codes").arg(QDir::currentPath());
    if (!QDir(outputDir).exists()) {
        QDir().mkdir(outputDir);
        qDebug() << "Répertoire créé pour les QR codes : " << outputDir;
    }

    // Vérifier si qrencode.exe existe
    QString qrencodePath = "C:/Users/DF-JULLANAR/Documents/CRUD/tools/qrencode.exe";
    if (!QFile::exists(qrencodePath)) {
        QMessageBox::critical(this, "Erreur", QString("Le fichier qrencode.exe est introuvable au chemin : %1").arg(qrencodePath));
        qDebug() << "Le fichier qrencode.exe est introuvable.";
        return;
    }

    // Requête SQL pour récupérer les IDs des chambres
    QSqlQuery query;
    if (!query.exec("SELECT id_chambre FROM chambre")) {
        // Output specific error to debug more effectively
        QString errorMsg = QString("Erreur SQL : %1").arg(query.lastError().text());
        QMessageBox::critical(this, "Erreur SQL", errorMsg);
        qDebug() << errorMsg;  // Output to console for more detail
        return;
    }

    // Parcourir les résultats et générer un QR code pour chaque ID
    while (query.next()) {
        QString idChambre = query.value(0).toString();
        QString filePath = QString("%1/Room_%2.png").arg(outputDir).arg(idChambre);

        // Commande pour générer un QR code avec qrencode.exe
        QString command = QString("\"%1\" -o \"%2\" \"%3\"")
                              .arg(qrencodePath)
                              .arg(filePath)
                              .arg(idChambre);

        QProcess process;
        process.start(command);
        if (!process.waitForFinished()) {
            QMessageBox::critical(this, "Erreur", QString("Échec de l'exécution de la commande : %1").arg(command));
            qDebug() << "Commande échouée : " << command;
            return;
        }

        // Vérifier si le fichier a été généré
        if (!QFile::exists(filePath)) {
            qDebug() << "Échec de génération pour ID chambre : " << idChambre << ", fichier attendu : " << filePath;
        } else {
            qDebug() << "QR code généré pour ID chambre : " << idChambre << ", fichier : " << filePath;
        }
    }

    QMessageBox::information(this, "Succès", QString("Les QR codes ont été générés dans : %1").arg(outputDir));
    qDebug() << "Génération des QR codes terminée avec succès.";
}


void MainWindow::addNoteManually(QString roomId, QString note)
{
    // Correctly creating an instance of the notesdialog class
    notesdialog *notesDialog = new notesdialog(roomId, note, this);  // Use notesdialog here
    notesDialog->show();
}

void MainWindow::on_pushButton_list_clicked()
{
    QString roomId = ui->lineEdit_id3->text();

    if (roomId.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a room ID.");
        return;
    }

    // Convert roomId to an integer
    bool ok;
    int roomIdInt = roomId.toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Error", "Invalid room ID. Please enter a valid number.");
        return;
    }

    // Check if the room ID exists in the 'chambre' table
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM chambre WHERE id_chambre = :room_id");
    query.bindValue(":room_id", roomIdInt);  // Bind as an integer

    if (!query.exec()) {
        QMessageBox::critical(this, "SQL Error", query.lastError().text());
        return;
    }

    query.next();
    int count = query.value(0).toInt();

    if (count == 0) {
        QMessageBox::warning(this, "Error", "Room ID not found in the database. Please enter a valid ID.");
        return;
    }

    addNoteManually(roomId, "");  // Pass roomId and initial note
}
