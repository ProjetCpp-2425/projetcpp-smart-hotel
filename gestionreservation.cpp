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

#include <QTimer>
#include <QThread>



GestionReservation::GestionReservation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GestionReservation)
{

    ui->setupUi(this);

    ui->tableView_reservation->setModel(this->reservation.afficher());
    ui->tableView_reservation->resizeColumnsToContents();

    ui->dateEdit_arrive->setDate(QDate::currentDate());
    ui->dateEdit_depart->setDate(QDate::currentDate());

    connect(ui->tabWidget_2, &QTabWidget::currentChanged, this, &GestionReservation::onTabChanged);
    connect(ui->comboBox_stat, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GestionReservation::updateChart);
    connect(ui->comboBox_tri, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GestionReservation::TriChanged);

    connect(ui->tabWidget_2, &QTabWidget::currentChanged, this, &GestionReservation::updateCalendrier);

    updateCalendrier();

    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &GestionReservation::onDateSelected);

    //connect(ui->pushButton_vocal, &QPushButton::clicked, this, &GestionReservation::on_pushButton_vocal_clicked);

    A.is_connected = 0;

    int ret=A.connect_arduino("COM7"); // lancer la connexion à arduino


    switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
            A.is_connected = 1;
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
            A.is_connected = 0;
           break;
        case(-1):qDebug() << "arduino is not available";
            A.is_connected = 0;

    }

    connect(ui->pushButton_readArduino,&QPushButton::clicked,this,&GestionReservation::on_pushButton_readArduino_clicked);

    //arduinoThread = new QThread(this);
    QTimer *arduinoTimer = new QTimer(this);

    connect(arduinoTimer, &QTimer::timeout, this, &GestionReservation::readArduinoContinuously);
    if (A.is_connected) {
        arduinoTimer->start(1000); // Check Arduino every 1000ms
    }

    //connect(arduinoThread, &QThread::started, this, &GestionReservation::readArduinoContinuously);
    connect(this, &GestionReservation::motionDetected, this, &GestionReservation::showMotionMessage);

    timenow = QDateTime::currentDateTime();
    derniereDetectMouvement = QDateTime::currentDateTime();


}

GestionReservation::~GestionReservation()
{
    if (arduinoThread) {
            arduinoThread->quit();
            arduinoThread->wait();
            delete arduinoThread;
    }
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

    if (test == false || montant <= 0) {
        QMessageBox::warning(this, "Montant invalide", "Veuillez entrer un montant valide et positif.");
        return;
    }

    bool cinOk;
    int cin = cinText.toInt(&cinOk);
    if (!cinOk || cin <= 0) {
        QMessageBox::warning(this, "CIN invalide", "Veuillez entrer un CIN valide.");
        return;
    }

    // Prepare SQL query to insert reservation
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

    // Execute reservation query
    if (query.exec() == false) {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la réservation dans la base de données. Vérifiez si le CIN existe dans la table clients.");
    } else { // Reservation was added successfully
        QMessageBox::information(this, "Succès", "Réservation ajoutée avec succès !");

        // Log the action into HISTORIQUE
        QString evenement = "Ajout de reservation";
        QString actionDetails = QString("CIN: %1, Type: %2, Montant: %3, Date Arrivée: %4, Date Départ: %5")
                                    .arg(cin)
                                    .arg(type)
                                    .arg(montant)
                                    .arg(dateArrivee.toString("yyyy-MM-dd"))
                                    .arg(dateDepart.toString("yyyy-MM-dd"));
        // Assuming the `clientId` is the same as the `cin` (adjust accordingly if necessary)
        if (ajouterHistorique(cin, evenement, actionDetails)) {
            qDebug() << "Historique ajouté avec succès.";
        } else {
            qDebug() << "Erreur lors de l'ajout de l'historique.";
        }

        // Update UI with new reservation list
        ui->tableView_reservation->setModel(this->reservation.afficher());
        ui->tableView_reservation->resizeColumnsToContents();

        // Clear the fields
        ui->lineEdit_Montant->clear();
        ui->lineEdit_cin->clear();
        ui->dateEdit_arrive->setDate(QDate::currentDate());
        ui->dateEdit_depart->setDate(QDate::currentDate().addDays(1));
        ui->comboBox_type->setCurrentIndex(0);
        ui->comboBox_status->setCurrentIndex(0);

        updateCalendrier();
    }
}


void GestionReservation::on_pushButton_19_clicked()
{
    int numReservation = ui->lineEdit_Supprimer->text().toInt();

    if (numReservation <= 0) {
        QMessageBox::warning(this, "Champs manquant", "Veuillez entrer un numéro de réservation valide.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM reservation WHERE num_reservation = :num_reservation");
    query.bindValue(":num_reservation", numReservation);

    if (query.exec() && query.numRowsAffected() > 0) {
        QMessageBox::information(this, "Succès", "La réservation a été supprimée avec succès.");

        // Log deletion in HISTORIQUE
        QString evenement = "Suppression de réservation";
        QString actionDetails = QString("Réservation ID: %1 supprimée").arg(numReservation);
        if (ajouterHistorique(123456, evenement, actionDetails)) { // Pass 0 as CIN since it's unknown for deletion
            qDebug() << "Historique ajouté avec succès.";
        } else {
            qDebug() << "Erreur lors de l'ajout de l'historique.";
        }

        // Update the table and reset UI
        ui->tableView_reservation->setModel(this->reservation.afficher());
        ui->tableView_reservation->resizeColumnsToContents();
        updateCalendrier();
        ui->lineEdit_Supprimer->clear();

    } else {
        QMessageBox::critical(this, "Erreur de suppression", "Échec de la suppression de la réservation.");
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

        // Log modification in HISTORIQUE
        QString evenement = "Modification de réservation";
        QString actionDetails = QString("ID: %1, CIN: %2, Type: %3, Montant: %4, Date Arrivée: %5, Date Départ: %6")
                                    .arg(id)
                                    .arg(cin)
                                    .arg(type)
                                    .arg(montant)
                                    .arg(dateArrivee.toString("yyyy-MM-dd"))
                                    .arg(dateDepart.toString("yyyy-MM-dd"));
        if (ajouterHistorique(cin, evenement, actionDetails)) {
            qDebug() << "Historique ajouté avec succès.";
        } else {
            qDebug() << "Erreur lors de l'ajout de l'historique.";
        }

        // Update the table and reset UI fields
        ui->tableView_reservation->setModel(this->reservation.afficher());
        ui->tableView_reservation->resizeColumnsToContents();

        ui->lineEdit_recherche->clear();
        ui->lineEdit_Montant_3->clear();
        ui->lineEdit_cin_2->clear();
        ui->dateEdit_arrive_2->setDate(QDate::currentDate());
        ui->dateEdit_depart_2->setDate(QDate::currentDate().addDays(1));
        ui->comboBox_type_2->setCurrentIndex(0);
        ui->comboBox_status_2->setCurrentIndex(0);
        updateCalendrier();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de la réservation. Vérifiez si le CIN existe dans la table clients.");
    }
}




void GestionReservation::on_pushButton_pdf_clicked()
{
    // Get the reservation number from lineEdit_goto_confirmation
    QString numReservationText = ui->lineEdit_goto_confirmation->text();
    if (numReservationText.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a reservation number.");
        return;
    }

    int numReservation = numReservationText.toInt();
    QSqlQuery query;

    // Query to fetch reservation data
    query.prepare("SELECT DATE_ARRIVEE, DATE_DEPART, TYPE, STATUT_RESERVATION, MONTANT, CIN "
                  "FROM RESERVATION WHERE NUM_RESERVATION = :num_reservation");
    query.bindValue(":num_reservation", numReservation);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Not Found", "No reservation found with the specified number.");
        return;
    }

    // Fetch data from query
    QString dateArrivee = query.value("DATE_ARRIVEE").toString();
    QString dateDepart = query.value("DATE_DEPART").toString();
    QString type = query.value("TYPE").toString();
    QString statutReservation = query.value("STATUT_RESERVATION").toString();
    QString montant = query.value("MONTANT").toString();
    QString cin = query.value("CIN").toString();

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

    // Get today's date
    QString todayDate = QDate::currentDate().toString("dd/MM/yyyy");

    // Start building the HTML content as a receipt with larger font sizes
    QString htmlContent;
    htmlContent += "<html><head>"
                   "<meta charset='UTF-8'>"
                   "<title>Reservation Receipt</title>"
                   "<style>"
                   "body { font-family: Verdana; margin: 25px; font-size: 14pt; }"
                   ".header { font-size: 20pt; font-weight: bold; text-align: center; margin-bottom: 20px; }"
                   ".info { font-size: 16pt; margin-top: 20px; }"
                   ".info p { margin: 10px 0; }"
                   ".footer { font-size: 14pt; text-align: center; margin-top: 30px; color: #555; }"
                   "</style>"
                   "</head><body>";

    // Header section
    htmlContent += "<div class='header'>Reservation Receipt</div>";

    // Date and Reservation Number
    htmlContent += "<div class='info'><p><strong>Date:</strong> " + todayDate + "</p>"
                   "<p><strong>Reservation Number:</strong> " + numReservationText + "</p></div>";

    // Reservation Details
    htmlContent += "<div class='info'><h3>Reservation Details</h3>"
                   "<p><strong>CIN:</strong> " + cin + "</p>"
                   "<p><strong>Arrival Date:</strong> " + dateArrivee + "</p>"
                   "<p><strong>Departure Date:</strong> " + dateDepart + "</p>"
                   "<p><strong>Type:</strong> " + type + "</p>"
                   "<p><strong>Status:</strong> " + statutReservation + "</p>"
                   "<p><strong>Amount:</strong> $" + montant + "</p></div>";


    // Footer section
    htmlContent += "<div class='footer'>Thank you for choosing checkinn. We hope to see you again!</div>";

    // Close HTML tags
    htmlContent += "</body></html>";

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



void GestionReservation::onTabChanged(int index)
{
    // Check if the currently selected tab is tab_4
    if (ui->tabWidget_2->widget(index) == ui->tab_4) {
        updateChart();
    }
}

void GestionReservation::updateChart()
{
    // Clear any existing chart to avoid duplicates
    if (ui->verticalLayout_stat->count() > 0) {
        QLayoutItem *item = ui->verticalLayout_stat->takeAt(0);
        delete item->widget();
        delete item;
    }

    QString selectedStat = ui->comboBox_stat->currentText();
    QChart *chart = new QChart();
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

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

        chart->addSeries(series);
        chart->setTitle("Statistiques - Répartition des réservations par type");
        series->setLabelsVisible();
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

        chart->addSeries(series);
        chart->setTitle("Statistiques - Répartition des réservations par statut");
        chart->createDefaultAxes();

        // Set axis titles using the appropriate axes
        if (!chart->axes(Qt::Horizontal).isEmpty()) {
            chart->axes(Qt::Horizontal).first()->setTitleText("Statut");
        }
        if (!chart->axes(Qt::Vertical).isEmpty()) {
            chart->axes(Qt::Vertical).first()->setTitleText("Nombre de réservations");
        }
    }

    // Add the chart view to the layout
    ui->verticalLayout_stat->addWidget(chartView);
}

void GestionReservation::on_pushButton_goto_confirmation_clicked()
{
    QString numReservationText = ui->lineEdit_goto_confirmation->text();

       if (numReservationText.isEmpty()) {
           QMessageBox::warning(this, "Input Error", "Please enter a reservation number.");
           return;
       }

       int numReservation = numReservationText.toInt();
       QSqlQuery query;

       // Prepare the query to fetch data from RESERVATION table based on NUM_RESERVATION
       query.prepare("SELECT DATE_ARRIVEE, DATE_DEPART, TYPE, STATUT_RESERVATION, MONTANT, CIN "
                     "FROM RESERVATION WHERE NUM_RESERVATION = :num_reservation");
       query.bindValue(":num_reservation", numReservation);

       // Execute the query
       if (query.exec() && query.next()) {
           // Set data to the respective line edits if the reservation is found
           ui->lineEdit_dateArrive_2->setText(query.value("DATE_ARRIVEE").toString());
           ui->lineEdit_dateDepart_2->setText(query.value("DATE_DEPART").toString());
           ui->lineEdit_Type_2->setText(query.value("TYPE").toString());
           ui->lineEdit_Status_2->setText(query.value("STATUT_RESERVATION").toString());
           ui->lineEdit_Montant_2->setText(query.value("MONTANT").toString());
           ui->lineEdit_Cin_2->setText(query.value("CIN").toString());

           ui->tabWidget_2->setCurrentWidget(ui->tab_5);

       } else {
           // Display an error message if the reservation is not found
           QMessageBox::warning(this, "Not Found", "No reservation found with the specified number.");
       }
}

#include <QTextCharFormat>

void GestionReservation::updateCalendrier() {
    // Reset all date formats to the default
    QTextCharFormat defaultFormat;
    defaultFormat.setBackground(Qt::white); // Default background color
    QDate today = QDate::currentDate();

    // Loop through all dates currently visible on the calendar
    for (int year = today.year() - 1; year <= today.year() + 1; ++year) {
        for (int month = 1; month <= 12; ++month) {
            for (int day = 1; day <= QDate(year, month, 1).daysInMonth(); ++day) {
                QDate date(year, month, day);
                if (date.isValid()) {
                    ui->calendarWidget->setDateTextFormat(date, defaultFormat);
                }
            }
        }
    }

    // Query the database to get reservation data
    QSqlQuery query;
    query.prepare("SELECT DATE_ARRIVEE, STATUT_RESERVATION FROM RESERVATION");

    if (!query.exec()) {
        qDebug() << "Error fetching reservation data:" << query.lastError().text();
        return;
    }

    // Apply custom formats for reservation dates
    while (query.next()) {
        QDate dateArrivee = query.value("DATE_ARRIVEE").toDate();
        QString statut = query.value("STATUT_RESERVATION").toString();

        QTextCharFormat format;
        if (statut == "confirmer") {
            format.setBackground(Qt::green);  // Green for "confirmer"
        } else if (statut == "annuler") {
            format.setBackground(Qt::red);  // Red for "annuler"
        }

        // Apply the formatting to the calendar widget
        ui->calendarWidget->setDateTextFormat(dateArrivee, format);
    }
}




void GestionReservation::onDateSelected(const QDate &date)
{
    QSqlQuery query;
    query.prepare(
        "SELECT NUM_RESERVATION, DATE_ARRIVEE, DATE_DEPART, STATUT_RESERVATION, MONTANT, CIN, TYPE "
        "FROM RESERVATION WHERE DATE_ARRIVEE = TO_DATE(:date, 'YYYY-MM-DD')"
    );
    query.bindValue(":date", date.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des données des réservations :" << query.lastError().text();
        return;
    }

    QString message;
    while (query.next()) {
        QString numReservation = query.value("NUM_RESERVATION").toString();
        QString dateArrivee = query.value("DATE_ARRIVEE").toString();
        QString dateDepart = query.value("DATE_DEPART").toString();
        QString statut = query.value("STATUT_RESERVATION").toString();
        QString montant = query.value("MONTANT").toString();
        QString cin = query.value("CIN").toString();
        QString type = query.value("TYPE").toString();

        message += QString(
            "Réservation #%1\n"
            "Date d'arrivée : %2\n"
            "Date de départ : %3\n"
            "Statut : %4\n"
            "Montant : %5\n"
            "CIN : %6\n"
            "Type : %7\n\n"
        ).arg(numReservation, dateArrivee, dateDepart, statut, montant, cin, type);
    }

    if (!message.isEmpty()) {
        QMessageBox::information(this, "Réservations pour " + date.toString("dd/MM/yyyy"), message);
    } else {
        QMessageBox::information(this, "Aucune réservation", "Il n'y a pas de réservations pour la date sélectionnée.");
    }
}



/*void GestionReservation::readReservationData(int id)
{
    qDebug() << "The on_pushButton_vocal_clicked function has been triggered.";

    QSqlQuery query;
    query.prepare("SELECT * FROM RESERVATION WHERE NUM_RESERVATION = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.next()) {
            QString dateArrivee = query.value("DATE_ARRIVEE").toString();
            QString dateDepart = query.value("DATE_DEPART").toString();
            QString statut = query.value("STATUT_RESERVATION").toString();
            double montant = query.value("MONTANT").toDouble();
            QString cin = query.value("CIN").toString();
            QString type = query.value("TYPE").toString();

            QString textToSpeak = "Bonjour, je vous appelle concernant votre réservation. Voici les détails : ";
            textToSpeak += "Votre réservation porte le numéro " + QString::number(id) + ". ";
            textToSpeak += "Votre date d'arrivée est prévue pour le " + dateArrivee + ", et vous quitterez l'établissement le " + dateDepart + ". ";
            textToSpeak += "Le statut actuel de votre réservation est : " + statut + ". ";
            textToSpeak += "Le montant total à régler est de " + QString::number(montant, 'f', 2) + " euros. ";
            textToSpeak += "Votre CIN enregistré est " + cin + ". ";
            textToSpeak += "Le type de réservation sélectionné est : " + type + ". ";
            textToSpeak += "Si vous avez des questions ou souhaitez apporter des modifications, n'hésitez pas à nous contacter. Merci et excellente journée !";

            QTextToSpeech *speech = new QTextToSpeech(this);
            QStringList engines = speech->availableEngines();

            if (engines.isEmpty()) {
                qDebug() << "No available TTS engines!";
                QMessageBox::critical(nullptr, QObject::tr("Erreur TTS"),
                                      QObject::tr("Aucun moteur de synthèse vocale disponible. Veuillez installer un moteur TTS."),
                                      QMessageBox::Ok);
                return;
            } else {
                qDebug() << "Using TTS engine:" << engines.first();
            }

            speech->say(textToSpeak);
        } else {
            QMessageBox::warning(nullptr, QObject::tr("Aucune réservation"),
                                 QObject::tr("Aucune réservation trouvée pour l'identifiant spécifié.\n"
                                             "Veuillez vérifier l'ID et réessayer."), QMessageBox::Ok);
        }
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur de réservation"),
                              QObject::tr("Une erreur s'est produite lors de la récupération des détails de la réservation.\n"
                                          "Erreur : %1").arg(query.lastError().text()), QMessageBox::Ok);
    }
}
*/

/*void GestionReservation::on_pushButton_vocal_clicked()
{
    // Show a message box to indicate the function is running
    QMessageBox::information(this, "Function Triggered", "The function on_pushButton_vocal_clicked has been called.");

    int id = ui->lineEdit_vocal->text().toInt();
    readReservationData(id);
}*/


void GestionReservation::TriChanged()
{
    QString critere = ui->comboBox_tri->currentText();

    ui->tableView_reservation->setModel(this->reservation.afficherAvecTri(critere));
    ui->tableView_reservation->resizeColumnsToContents();
}


void GestionReservation::on_pushButton_vocal_clicked()
{

}


void GestionReservation::on_pushButton_readArduino_clicked()
{
    // Récupérer l'ID client depuis le champ de saisie
        QString clientId = ui->lineEdit_IDCLIENT->text();

        // Vérifier si l'ID client est valide
        if (clientId.isEmpty()) {
            QMessageBox::warning(this, "Entrée invalide", "Veuillez saisir un ID client valide.");
            return;
        }

        // Préparer la requête SQL
        QSqlQuery query;
        query.prepare("SELECT ID, EVENEMENT, DETAILS, TO_CHAR(DATES, 'YYYY-MM-DD HH24:MI:SS') AS LOG_DATE, CLIENT_ID "
                      "FROM HISTORIQUE WHERE CLIENT_ID = :clientId ORDER BY DATES DESC");
        query.bindValue(":clientId", clientId);

        // Exécuter la requête
        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur Base de Données", "Échec de la récupération des logs : " + query.lastError().text());
            return;
        }

        // Créer la sortie des logs sous forme de texte
        QString sortieLogs;

        while (query.next()) {
            QString id = query.value("ID").toString();
            QString evenement = query.value("EVENEMENT").toString();
            QString details = query.value("DETAILS").toString();
            QString dateHeure = query.value("LOG_DATE").toString();
            QString clientId = query.value("CLIENT_ID").toString();

            // Formater le log en texte
            sortieLogs += QString("ID: %1\nÉvénement: %2\nDétails: %3\nDate: %4\nClient ID: %5\n\n")
                              .arg(id)
                              .arg(evenement)
                              .arg(details)
                              .arg(dateHeure)
                              .arg(clientId);
        }

        // Vérifier si des logs ont été trouvés
        if (sortieLogs.isEmpty()) {
            sortieLogs = "Aucun log trouvé pour l'ID client spécifié.";
        }

        // Afficher les logs dans un QTextBrowser ou QTextEdit
        ui->textBrowser->setPlainText(sortieLogs);
}

void GestionReservation::readArduinoContinuously()
{
    timenow = QDateTime::currentDateTime();

    if (!A.is_connected  ) {
        return; // Stop processing if Arduino is not connected
    }

    QString textrecu="aucun Mouvement";

    QByteArray text = A.read_from_arduino();

    textrecu = QString(text).trimmed();


    if (textrecu == "D:1" && derniereDetectMouvement.msecsTo(timenow) > 5000) {
        derniereDetectMouvement = QDateTime::currentDateTime();

        QMessageBox::warning(this, "Detection De Mouvement", "il ya mouvement dans la  :chambre 1.");


    } else if (textrecu == "D:2"&&derniereDetectMouvement.msecsTo(timenow) > 5000) {
        derniereDetectMouvement = QDateTime::currentDateTime();

        QMessageBox::warning(this, "Detection De Mouvement", "il ya mouvement dans la  :  chambre 2.");

    }
    qDebug() << textrecu;
}






















void GestionReservation::showMotionMessage(const QString &chambre)
{
    QMessageBox::warning(this, "Detection De Mouvement", "il ya mouvement dans la  :  " + chambre + ".");
}


bool GestionReservation::ajouterHistorique(int clientId, QString &evenement, QString &details)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO HISTORIQUE
        ("CLIENT_ID", "EVENEMENT", "DETAILS", "DATES")
        VALUES (:client_id, :evenement, :details, TO_DATE(:date_action, 'YYYY-MM-DD HH24:MI:SS'))
    )");

    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    query.bindValue(":client_id", clientId);
    query.bindValue(":evenement", evenement);
    query.bindValue(":details", details);
    query.bindValue(":date_action", currentDateTime);

    if (!query.exec()) {
        qDebug() << "Error inserting into HISTORIQUE:" << query.lastError().text();
        return false;
    }
    return true;
}
