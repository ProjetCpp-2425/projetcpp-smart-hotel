#include "gestionreservation.h"
#include "ui_gestionreservation.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>
#include <QSqlError>


#include <QFileDialog>
#include <QPrinter>
#include <QTextStream>
#include <QTextDocument>

#include <QPieSeries>
#include <QPieSlice>
#include <QBarSeries>
#include <QBarSet>
#include <QtCharts>

GestionReservation::GestionReservation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GestionReservation)
{
    ui->setupUi(this);

    ui->tableView_reservation->setModel(this->reservation.afficher());
    ui->tableView_reservation->resizeColumnsToContents();

}

GestionReservation::~GestionReservation()
{
    delete ui;
}

void GestionReservation::on_pushButton_ajouter_clicked()
{
    QString montantText = ui->lineEdit_Montant->text();
    QDate dateArrivee = ui->dateEdit_arrive->date();
    QDate dateDepart = ui->dateEdit_depart->date();
    QString type = ui->comboBox_type->currentText();
    QString statutReservation = ui->comboBox_status->currentText();
    QString cinText = ui->lineEdit_cin->text();




    if (montantText.isEmpty() || type.isEmpty() || statutReservation.isEmpty() || cinText.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    if (dateArrivee >= dateDepart) {
        QMessageBox::warning(this, "Erreur de date", "La date d'arrivée doit être antérieure à la date de départ.");
        return;
    }

    bool test;
    double montant = montantText.toDouble(&test);

    if (test == false  || montant <= 0) {
        QMessageBox::warning(this, "Montant invalide", "Veuillez entrer un montant valide et positif.");
        return;
    }

    bool cinOk;
    int cin = cinText.toInt(&cinOk);
    if (!cinOk || cin <= 0) {
        QMessageBox::warning(this, "CIN invalide", "Veuillez entrer un CIN valide.");
        return;
    }



    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO reservation (date_arrivee, date_depart, statut_reservation, montant, type, cin)
        VALUES (TO_DATE(:date_arrivee, 'YYYY-MM-DD'), TO_DATE(:date_depart, 'YYYY-MM-DD'), :statut_reservation, :montant, :type, :cin)
    )");

    query.bindValue(":date_arrivee", dateArrivee.toString("yyyy-MM-dd"));
    query.bindValue(":date_depart", dateDepart.toString("yyyy-MM-dd"));
    query.bindValue(":statut_reservation", statutReservation);
    query.bindValue(":montant", montant);
    query.bindValue(":type", type);
    query.bindValue(":cin", cin);




    if (query.exec() == false ) {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la réservation dans la base de données. Vérifiez si le CIN existe dans la table clients.");
    } else { //true
        QMessageBox::information(this, "Succès", "Réservation ajoutée avec succès !");

        ui->tableView_reservation->setModel(this->reservation.afficher());
        ui->tableView_reservation->resizeColumnsToContents();

        ui->lineEdit_Montant->clear();
        ui->lineEdit_cin->clear();
        ui->dateEdit_arrive->setDate(QDate::currentDate());
        ui->dateEdit_depart->setDate(QDate::currentDate().addDays(1));
        ui->comboBox_type->setCurrentIndex(0);
        ui->comboBox_status->setCurrentIndex(0);
    }
}

void GestionReservation::on_pushButton_19_clicked()
{
    QSqlQuery query;
        query.prepare("DELETE FROM reservation WHERE num_reservation = :num_reservation");
        query.bindValue(":num_reservation", ui->lineEdit_Supprimer->text().toInt());

        if (query.exec() && query.numRowsAffected() > 0) {
            QMessageBox::information(this, "Succès", "La réservation a été supprimée avec succès.");
            ui->tableView_reservation->setModel(this->reservation.afficher());
            ui->tableView_reservation->resizeColumnsToContents();
        } else {
            QMessageBox::critical(this, "Erreur de suppression", "Échec de la suppression de la réservation. ");
        }
}

void GestionReservation::on_pushButton_rechercher_clicked()
{
    QString idText = ui->lineEdit_recherche->text();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Champs manquant", "Veuillez entrer un numéro de réservation.");
        return;
    }

    bool test;
    int id = idText.toInt(&test);

    if (!test) {
        QMessageBox::warning(this, "Numéro invalide", "Veuillez entrer un numéro de réservation valide.");
        return;
    }

    Reservation reservation = Reservation().recherche(id);



    if (reservation.getNumReservation() == 0) {
        QMessageBox::warning(this, "Réservation introuvable", "Aucune réservation trouvée avec cet identifiant.");
        return;
    }

    ui->lineEdit_Montant_3->setText(QString::number(reservation.getMontant()));
    ui->comboBox_type_2->setCurrentText(reservation.getType());
    ui->comboBox_status_2->setCurrentText(reservation.getStatutReservation());
    ui->dateEdit_arrive_2->setDate(reservation.getDateArrivee());
    ui->dateEdit_depart_2->setDate(reservation.getDateDepart());
    ui->lineEdit_cin_2->setText(QString::number(reservation.getCin()));
}

void GestionReservation::on_pushButton_modifier_clicked()
{
    QString idText = ui->lineEdit_recherche->text();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Champs manquant", "Veuillez entrer un numéro de réservation.");
        return;
    }

    bool ok;
    int id = idText.toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Numéro invalide", "Veuillez entrer un numéro de réservation valide.");
        return;
    }

    QString montantText = ui->lineEdit_Montant_3->text();
    QDate dateArrivee = ui->dateEdit_arrive_2->date();
    QDate dateDepart = ui->dateEdit_depart_2->date();
    QString type = ui->comboBox_type_2->currentText();
    QString statutReservation = ui->comboBox_status_2->currentText();
    QString cinText = ui->lineEdit_cin_2->text();

    if (montantText.isEmpty() || type.isEmpty() || statutReservation.isEmpty() || cinText.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    if (dateArrivee >= dateDepart) {
        QMessageBox::warning(this, "Erreur de date", "La date d'arrivée doit être antérieure à la date de départ.");
        return;
    }

    bool montantOk, cinOk;
    double montant = montantText.toDouble(&montantOk);
    int cin = cinText.toInt(&cinOk);


    if (!montantOk || montant <= 0) {
        QMessageBox::warning(this, "Montant invalide", "Veuillez entrer un montant valide et positif.");
        return;
    }

    if (!cinOk || cin <= 0) {
        QMessageBox::warning(this, "CIN invalide", "Veuillez entrer un CIN valide.");
        return;
    }


    QSqlQuery query;
    query.prepare(R"(
        UPDATE reservation
        SET date_arrivee = TO_DATE(:date_arrivee, 'YYYY-MM-DD'),
            date_depart = TO_DATE(:date_depart, 'YYYY-MM-DD'),
            statut_reservation = :statut_reservation,
            montant = :montant,
            type = :type,
            cin = :cin
        WHERE num_reservation = :num_reservation
    )");

    query.bindValue(":date_arrivee", dateArrivee.toString("yyyy-MM-dd"));
    query.bindValue(":date_depart", dateDepart.toString("yyyy-MM-dd"));
    query.bindValue(":statut_reservation", statutReservation);
    query.bindValue(":montant", montant);
    query.bindValue(":type", type);
    query.bindValue(":cin", cin);
    query.bindValue(":num_reservation", id);

    if (query.exec() && query.numRowsAffected() > 0) {
        QMessageBox::information(this, "Succès", "Réservation modifiée avec succès !");

        ui->tableView_reservation->setModel(this->reservation.afficher());
        ui->tableView_reservation->resizeColumnsToContents();

        ui->lineEdit_recherche->clear();
        ui->lineEdit_Montant_3->clear();
        ui->lineEdit_cin_2->clear();
        ui->dateEdit_arrive_2->setDate(QDate::currentDate());
        ui->dateEdit_depart_2->setDate(QDate::currentDate().addDays(1));
        ui->comboBox_type_2->setCurrentIndex(0);
        ui->comboBox_status_2->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de la réservation. Vérifiez si le CIN existe dans la table clients.");
    }
}



void GestionReservation::on_pushButton_pdf_clicked()
{
    // Initialize an HTML string to build the content
        QString htmlContent;

        // Open a dialog for the user to select the save location
        QString pdfFilePath = QFileDialog::getSaveFileName(
            this, tr("Save PDF"), "", "*.pdf");
        if (QFileInfo(pdfFilePath).suffix().isEmpty()) {
            pdfFilePath.append(".pdf");
        }

        // Configure the printer for PDF output
        QPrinter pdfPrinter(QPrinter::HighResolution);
        pdfPrinter.setOutputFormat(QPrinter::PdfFormat);
        pdfPrinter.setOutputFileName(pdfFilePath);

        // Use QPageLayout to set the paper size to A4
        QPageLayout pageLayout;
        pageLayout.setPageSize(QPageSize(QPageSize::A4));
        pdfPrinter.setPageLayout(pageLayout);

        // Access data from tableView_reservation
        QAbstractItemModel* model = ui->tableView_reservation->model();
        int totalRows = model->rowCount();
        int totalColumns = model->columnCount();

        // Get today's date
        QString todayDate = QDate::currentDate().toString("dd/MM/yyyy");

        // Start building the HTML content
        htmlContent += "<html><head>"
                       "<meta charset='UTF-8'>"
                       "<title>Reservation Report</title>"
                       "<style>"
                       "body { font-family: Verdana; margin: 25px; }"
                       ".header { font-size: 18px; margin-bottom: 15px; }"
                       ".title { text-align: center; }"
                       ".title h1 { margin-bottom: 10px; }"
                       ".title h4 { margin-top: 0; }"
                       "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                       "th, td { border: 1px solid #333; padding: 10px; }"
                       "th { background-color: #f0f0f0; }"
                       "</style>"
                       "</head><body>";

        // Header section
        htmlContent += "<div class='header'>Welcome to PharmaFLOW</div>";

        // Title section
        htmlContent += "<div class='title'><h1>Reservation List</h1>"
                       "<h4>Date: " + todayDate + "</h4></div>";

        // Begin table
        htmlContent += "<table><thead><tr>";

        // Add table headers
        for (int col = 0; col < totalColumns; ++col) {
            if (!ui->tableView_reservation->isColumnHidden(col)) {
                QString headerText = model->headerData(col, Qt::Horizontal).toString();
                htmlContent += "<th>" + headerText + "</th>";
            }
        }
        htmlContent += "</tr></thead><tbody>";

        // Populate table rows
        for (int row = 0; row < totalRows; ++row) {
            htmlContent += "<tr>";
            for (int col = 0; col < totalColumns; ++col) {
                if (!ui->tableView_reservation->isColumnHidden(col)) {
                    QModelIndex index = model->index(row, col);
                    QString cellData = model->data(index).toString().simplified();
                    htmlContent += "<td>" + (cellData.isEmpty() ? "&nbsp;" : cellData) + "</td>";
                }
            }
            htmlContent += "</tr>";
        }

        // Close table and HTML tags
        htmlContent += "</tbody></table></body></html>";

        // Create a document to render the HTML content
        QTextDocument document;
        document.setHtml(htmlContent);

        // Print the document to the specified PDF file
        document.print(&pdfPrinter);
}

void GestionReservation::on_pushButton_recherchers_clicked()
{
    QString donnee = ui->lineEdit_rechercher->text();

    ui->tableView_reservation->setModel(this->reservation.rechercher(donnee));
    ui->tableView_reservation->resizeColumnsToContents();
}

void GestionReservation::on_pushButton_27_clicked()
{
    QString selectedStat = ui->comboBox_stat->currentText();

        if (selectedStat == "Type Reservation")
        {
            // Pie chart logic
            QPieSeries *series = new QPieSeries();
            QSqlQuery query;
            query.prepare("SELECT TYPE, COUNT(*) FROM RESERVATION GROUP BY TYPE");
            query.exec();

            int totalReservations = 0;
            QHash<QString, int> typeCounts;

            while (query.next())
            {
                QString type = query.value(0).toString();
                int nombreReservations = query.value(1).toInt();
                totalReservations += nombreReservations;
                typeCounts.insert(type, nombreReservations);
            }

            for (auto it = typeCounts.constBegin(); it != typeCounts.constEnd(); ++it)
            {
                QString type = it.key();
                int nombreReservations = it.value();
                double percentage = (nombreReservations * 100.0) / totalReservations;
                QPieSlice *slice = series->append(type, nombreReservations);
                slice->setLabel(QString("%1\n%2%").arg(type).arg(percentage, 0, 'f', 1));
            }

            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle("Statistiques - Répartition des réservations par type");
            series->setLabelsVisible();
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            chartView->resize(1280, 720);
            chartView->show();
        }
        else if (selectedStat == "Montant Reservation")
        {
            // Bar chart logic with ranges
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Réservations");
            QSqlQuery query;
            query.prepare("SELECT MONTANT FROM RESERVATION");
            query.exec();

            int count0_250 = 0;
            int count250_750 = 0;
            int countAbove750 = 0;

            while (query.next())
            {
                int montant = query.value(0).toInt();
                if (montant <= 250)
                    count0_250++;
                else if (montant <= 750)
                    count250_750++;
                else
                    countAbove750++;
            }

            *set << count0_250 << count250_750 << countAbove750;

            series->append(set);

            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle("Statistiques - Répartition des réservations par tranches de montant");
            chart->setAnimationOptions(QChart::SeriesAnimations);

            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(QStringList() << "0-250" << "250-750" << ">750");
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);

            QValueAxis *axisY = new QValueAxis();
            axisY->setTitleText("Nombre de réservations");
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);

            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            chartView->resize(1280, 720);
            chartView->show();
        }
        else if (selectedStat == "Status Reservation")
        {
            // Line chart logic
            QLineSeries *series = new QLineSeries();
            QSqlQuery query;
            query.prepare("SELECT STATUT_RESERVATION, COUNT(*) FROM RESERVATION GROUP BY STATUT_RESERVATION");
            query.exec();

            int index = 0;
            while (query.next())
            {
                QString status = query.value(0).toString();
                int count = query.value(1).toInt();
                series->append(index, count);
                series->setName(status);
                index++;
            }

            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle("Statistiques - Répartition des réservations par statut");
            chart->createDefaultAxes();

            // Access the X and Y axes using QChart::axes()
            auto axesX = chart->axes(Qt::Horizontal);
            auto axesY = chart->axes(Qt::Vertical);

            // Set titles for the axes, if they exist
            if (!axesX.isEmpty()) {
                axesX.first()->setTitleText("Statut");
            }
            if (!axesY.isEmpty()) {
                axesY.first()->setTitleText("Nombre de réservations");
            }

            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            chartView->resize(1280, 720);
            chartView->show();
        }
}
