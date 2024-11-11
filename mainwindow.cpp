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



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    connect(ui->pushButton_afficher, &QPushButton::clicked, this, &MainWindow::refreshTableView);
    connect(ui->buttonRechercher, &QPushButton::clicked, this, &MainWindow::on_buttonRechercher_clicked);
    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::on_pushButton_pdf_clicked);



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
    // Query the database to count the number of rooms for each type
    QSqlQuery query;
    query.prepare("SELECT type, COUNT(*) as count FROM chambre GROUP BY type");

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to retrieve data: " + query.lastError().text());
        return;
    }

    // Process the data and calculate percentages
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

    // Calculate percentages
    QMap<QString, double> typePercentages;
    for (auto it = typeCounts.begin(); it != typeCounts.end(); ++it) {
        typePercentages[it.key()] = (it.value() / (double)totalRooms) * 100.0;
    }

    // File save dialog
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "*.pdf");
    if (fileName.isEmpty()) return; // User canceled the dialog
    if (!fileName.endsWith(".pdf")) fileName += ".pdf";

    // Create the PDF
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMargins(20, 20, 20, 20));

    QPainter painter(&pdfWriter);

    // Title
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(200, 100, "Statistiques par Type");

    // Draw statistics
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
