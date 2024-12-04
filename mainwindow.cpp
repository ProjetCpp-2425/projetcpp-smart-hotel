#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arduino.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDate>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include "statistique.h"
#include <QInputDialog>
#include <QtCharts/QChart>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChartView>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSqlDatabase>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), arduino(new Arduino(this))
{
    ui->setupUi(this);

    // Set up placeholders and masked input
    ui->lineEdit_cin2->setPlaceholderText("Entrez votre CIN");
    ui->lineEdit_mdp2->setPlaceholderText("Entrez votre mot de passe");
    ui->lineEdit_mdp2->setEchoMode(QLineEdit::Password);

    // Set up employee management
    ui->tableView->setModel(employe.afficher());
    ui->dateEdit_date_embauche->setDate(QDate::currentDate());

    ui->pushButton_pdf->setText("Exporter PDF");


    serial = new QSerialPort(this);
    serial->setPortName("COM5"); // Remplacez par le port de votre Arduino
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    //connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

    if (!serial->open(QIODevice::ReadOnly)) {
        qDebug() << "Erreur : Impossible d'ouvrir le port série.";
    }

    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::on_pushButton_pdf_clicked);


    // Connect CAPTCHA generation and validation
    connect(ui->refreshCaptchaButton, &QPushButton::clicked, this, &MainWindow::regenerateCaptcha);
    connect(ui->validateCaptchaButton, &QPushButton::clicked, this, &MainWindow::validateCaptcha);

    connect(ui->comboBox_trier, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_trier_currentIndexChanged(int)));


    connect(ui->pushButton_envoyer, &QPushButton::clicked, this, &MainWindow::on_pushButton_envoyer_clicked);

    connect(arduino, &Arduino::clientFound, this, &MainWindow::onClientFound);  // Connexion du signal

    //connect(arduino, &Arduino::dataReceived, this, &MainWindow::slotRecevoirMorceau);
    //connect(arduino, &Arduino::dataReceived, this, &MainWindow::slotRecevoirMorceau, Qt::QueuedConnection);


    // Generate initial CAPTCHA
    regenerateCaptcha();

    /*arduino = new Arduino(this);

    if (arduino->openConnection(QSerialPort::Baud9600)) {
            qDebug() << "Connexion Arduino établie.";
        } else {
            qDebug() << "Échec de la connexion à l'Arduino.";
        }
    }*/
    arduino = new Arduino(this);

              // Connexion du signal dataReceived à un slot pour traiter les données
             // connect(arduino, &Arduino::dataReceived, this, &MainWindow::handleData);
              connect(arduino, &Arduino::clientFound, this, &MainWindow::onClientFound);

              // Vous pouvez aussi essayer d'ouvrir la connexion avec l'Arduino ici si nécessaire
              QString portName = "COM5";  // Remplacez par le nom correct du port
              int baudRate = 9600;        // Le taux de baud par défaut  (Définit la vitesse de communication série à 9600 bauds)
              // Appelle la méthode openConnection pour établir une connexion série.
              if (arduino->openConnection(portName, baudRate)) {
                  qDebug() << "Arduino connecté avec succès!";
              } else {
                  qDebug() << "Échec de la connexion à l'Arduino.";
              }
 }


MainWindow::~MainWindow() {
    delete ui;
}

/*void MainWindow::onClientFound(const QString &rfid) {
    qDebug() << "UID reçu : " << rfid;

    // Requête SQL pour récupérer les informations de l'employé par RFID
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM FROM EMPLOYE WHERE RFID = :rfid");
    query.bindValue(":rfid", rfid);

    if (query.exec()) {
        if (query.next()) {
                // Client trouvé
                QString clientInfo = QString("CIN : %1\nNom : %2\nPrénom : %3\nHeure de détection : %4")
                                        .arg(query.value(0).toString())
                                        .arg(query.value(1).toString())
                                        .arg(query.value(2).toString())
                                        .arg(query.value(3).toString());
                QMessageBox::information(this, "Client trouvé", clientInfo);
        } else {
            // Aucun employé trouvé pour ce RFID
            qDebug() << "Aucun employé trouvé pour ce RFID";
            QMessageBox::warning(this, "Employé introuvable", "Aucun employé correspondant trouvé pour cet UID.");
        }
    } else {
        // Afficher l'erreur SQL dans le cas où la requête échoue
        qDebug() << "Erreur SQL : " << query.lastError().text();
        QMessageBox::critical(this, "Erreur SQL", "Erreur lors de la requête.");
    }
}*/

void MainWindow::onClientFound(const QString &RFID) {
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM FROM EMPLOYE WHERE UPPER(RFID) = UPPER(:RFID)");
    query.bindValue(":RFID", RFID); //Cette ligne lie la valeur du paramètre :UIDC

    if (!query.exec()) {
        qDebug() << "Erreur lors de la requête : " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de rechercher le fournisseur."); //Affiche une boîte de dialogue d'erreur avec un message
        //indiquant qu'il est impossible de rechercher le fournisseur.
        return;
    }
//query.next Passe au premier résultat de la requête (s'il existe).
    if (query.next()) {
        QString fournisseurInfo = QString("CIN: %1\nNOM: %2\nPRENOM: %3")//Crée une chaîne formatée qui contient les informations du fournisseur sous forme de texte lisible.
                                  .arg(query.value(0).toString())
                                  .arg(query.value(1).toString())
                                  .arg(query.value(2).toString());
        QMessageBox::information(this, "Fournisseur trouvé", fournisseurInfo);

        //Formatage pour affichage sur un écran LCD

        QString prenom = query.value(1).toString();
        QString nom = query.value(2).toString();

        // Limitation pour un affichage LCD de 16x2
        nom = nom.left(16);
        prenom = prenom.left(16);

        QString lcdMessage = QString("%1,%2\n").arg(nom).arg(prenom);
        qDebug() << "Message envoyé à Arduino : " << lcdMessage;
        arduino->writeData(lcdMessage.toUtf8());//Envoie le message formaté à l'Arduino via le port série .

    } else {
        QMessageBox::warning(this, "Fournisseur introuvable", "Aucun fournisseur trouvé pour cet RFID.");
        arduino->writeData("UNKNOWN\n");
    }
}


void MainWindow::readSerialData() {
    QByteArray data = serialPort->readAll();
    QString rfid = QString(data).trimmed();

    if (!rfid.isEmpty()) {
        qDebug() << "UID reçu: " << rfid;

        // Requête SQL pour récupérer les informations du client
        QSqlQuery query;
        query.prepare("SELECT cin FROM employe WHERE UPPER(rfid) = UPPER(:RFID)");
        query.bindValue(":RFID", rfid);

        if (query.exec() && query.next()) {
            QString cin = query.value("cin").toString();
            QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");

            QString message = QString("CIN: %1\nHeure: %2").arg(cin).arg(currentTime);
            QMessageBox::information(this, "Carte RFID détectée", message);
        } else {
            QMessageBox::warning(this, "Erreur", "Carte non reconnue.");
        }
    }
}


void MainWindow::on_pushButton_statistique_clicked()
{
    try {
        // Crée un graphique (QChart) pour afficher les statistiques
        QChart *chart = new QChart();
        QBarSeries *series = new QBarSeries();
        QBarCategoryAxis *axis = new QBarCategoryAxis();

        QBarSet *set = new QBarSet("Nombre d'Employés");
        QStringList postesList;

        // Exécute une requête SQL pour récupérer les statistiques
        QSqlQuery query;
        query.prepare("SELECT poste, COUNT(*) AS count FROM employe GROUP BY poste");

        if (query.exec()) {
            while (query.next()) {
                QString poste = query.value(0).toString(); // Récupère le nom du poste
                int count = query.value(1).toInt();       // Récupère le nombre d'employés pour ce poste

                postesList.append(poste);
                *set << count; // Ajoute le nombre au jeu de barres
            }

            series->append(set);
            chart->addSeries(series);
            chart->setAnimationOptions(QChart::AllAnimations);
            axis->append(postesList);

            // Configure l'axe X et Y
            chart->createDefaultAxes();
            chart->addAxis(axis, Qt::AlignBottom); // Ajoute l'axe X au bas du graphique
            series->attachAxis(axis);             // Lie la série à l'axe X
            chart->legend()->setAlignment(Qt::AlignBottom);

            // Configure l'apparence du graphique
            QPalette pal = qApp->palette();
            pal.setColor(QPalette::Window, QRgb(0x0d4261));
            pal.setColor(QPalette::WindowText, QRgb(0x95212c));
            qApp->setPalette(pal);

            QFont font;
            font.setPixelSize(18);
            chart->setTitleFont(font);
            chart->setTitleBrush(QBrush(Qt::red));
            chart->setTitle("Statistiques : Nombre d'Employés par Poste");

            // Configure la vue du graphique
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            chartView->setChart(chart); // Assurez-vous que 'chartContainer' est un widget dans votre interface
            chartView->showNormal();
            chartView->show();

        } else {
            qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();
        }
    } catch (...) {
        qDebug() << "Une erreur inattendue s'est produite.";
    }
}

void MainWindow::on_pushButton_envoyer_clicked() {
    // Récupérer les données des champs
    QString cin = ui->lineEdit_cin3->text();
    QString nomPrenom = ui->lineEdit_nomprenom_5->text();
    QDate dateDebut = ui->dateEdit_dd->date();
    QDate dateFin = ui->dateEdit_df->date();
    QString typeAbsence = ui->comboBox_abs->currentText();

    // Vérifier si les champs sont remplis
    if (cin.isEmpty() || nomPrenom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Vérifier que la date de fin est après la date de début
    if (dateFin < dateDebut) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être postérieure à la date de début.");
        return;
    }

    // Calculer le nombre de jours d'absence
    int joursAbsence = calculerJoursAbsence(dateDebut, dateFin);

    // Insérer les données dans la base
    QSqlQuery query;
    query.prepare("INSERT INTO absences (cin, nom_prenom, date_debut, date_fin, type_absence, jours_absence) "
                  "VALUES (:cin, :nom_prenom, :date_debut, :date_fin, :type_absence, :jours_absence)");
    query.bindValue(":cin", cin);
    query.bindValue(":nom_prenom", nomPrenom);
    query.bindValue(":date_debut", dateDebut);
    query.bindValue(":date_fin", dateFin);
    query.bindValue(":type_absence", typeAbsence);
    query.bindValue(":jours_absence", joursAbsence);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", QString("Demande envoyée avec succès !\n"
                                                         "Nombre de jours d'absence : %1").arg(joursAbsence));
    } else {
        QMessageBox::critical(this, "Erreur", QString("Échec de l'envoi de la demande.\nErreur : %1")
                                               .arg(query.lastError().text()));
    }
}

int MainWindow::calculerJoursAbsence(const QDate &dateDebut, const QDate &dateFin) {
    // Calculer la différence entre les deux dates
    return dateDebut.daysTo(dateFin) + 1; // Inclure le premier jour
}

void MainWindow::validateCaptcha() {
    int userInput = ui->captchaInput->text().toInt(); // Read user input

    if (userInput == generatedCaptcha) {
        QMessageBox::information(this, "Succès", "Le CAPTCHA est correct !");
    } else {
        QMessageBox::warning(this, "Erreur", "Le CAPTCHA est incorrect. Veuillez réessayer.");
        ui->captchaInput->clear(); // Clear input field
        regenerateCaptcha();       // Generate new CAPTCHA
    }
}

void MainWindow::regenerateCaptcha() {
    // Generate a random 6-digit number
    generatedCaptcha = QRandomGenerator::global()->bounded(100000, 1000000); // Correct inclusive bounds

    // Display the CAPTCHA in the label
    ui->captchaLabel->setText(QString::number(generatedCaptcha));
}


// Validate login inputs (CIN and password)
bool MainWindow::validateLoginInputs(const QString &cin, const QString &mot_de_passe)
{
    bool ok;
    cin.toUInt(&ok);
    if (!ok || cin.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Le CIN doit être un entier composé de 8 chiffres.");
        return false;
    }

    if (mot_de_passe.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe ne peut pas être vide.");
        return false;
    }

    return true;
}

// Authenticate the user with the database
bool MainWindow::authenticate(const QString &cin, const QString &mot_de_passe)
{
    QSqlQuery query;
    query.prepare("SELECT mot_de_passe FROM employe WHERE cin = :cin");
    query.bindValue(":cin", cin);

    if (query.exec()) {
        if (query.next()) {
            QString dbPassword = query.value(0).toString();
            if (dbPassword == mot_de_passe) {
                return true;
            } else {
                QMessageBox::warning(this, "Erreur", "Mot de passe incorrect.");
                return false;
            }
        } else {
            QMessageBox::warning(this, "Erreur", "CIN introuvable.");
            return false;
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la requête.");
        return false;
    }
}

// Slot for "Log in" button
void MainWindow::on_pushButton_login_clicked()
{
    QString cin = ui->lineEdit_cin2->text();
    QString mot_de_passe = ui->lineEdit_mdp2->text();

    if (validateLoginInputs(cin, mot_de_passe) && authenticate(cin, mot_de_passe)) {
        QMessageBox::information(this, "Succès", "Connexion réussie !");
        ui->stackedWidget->setCurrentIndex(1); // Switch to main application page
    }
}

// Slot for "Forgot Password" button
void MainWindow::on_pushButton_forgotPassword_clicked()
{
    QString cin = ui->lineEdit_cin2->text();

    // Validation du CIN
    bool ok;
    cin.toUInt(&ok);
    if (!ok || cin.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Entrez un CIN valide avant de réinitialiser le mot de passe.");
        return;
    }

    // Rechercher la réponse associée à la question de sécurité fixe
    QSqlQuery query;
    query.prepare("SELECT question FROM employe WHERE cin = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "CIN introuvable dans la base de données.");
        return;
    }

    // Définir la question de sécurité fixe
    QString question = "Quelle est la ville où vous êtes né(e) ?";
    QString reponse = query.value("question").toString();

    // Poser la question à l'utilisateur
    QString userAnswer = QInputDialog::getText(this, "Question de sécurité", question, QLineEdit::Normal);
    if (userAnswer.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Vous devez répondre à la question.");
        return;
    }

    // Vérifier la réponse
    if (userAnswer.toLower().trimmed() == reponse.toLower().trimmed()) {
        QMessageBox::information(this, "Succès", "Réponse correcte. Vous pouvez réinitialiser votre mot de passe.");
        // Ajouter la logique pour réinitialiser le mot de passe (ex. ouvrir une nouvelle fenêtre).
    } else {
        QMessageBox::warning(this, "Erreur", "Réponse incorrecte. Impossible de réinitialiser le mot de passe.");
    }
}

/*// Function to handle exporting statistics to PDF
void MainWindow::on_pushButton_statistique_clicked() {
    // Get the filename and path for saving the PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter les Statistiques en PDF", "", "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) {
        return;
    }

    // Create a Statistique object (assuming Statistique class handles the actual export)
    Statistique stats;

    // Export the statistics to the selected PDF file
    stats.exportToPdf(fileName);

    // Show a message confirming the export
    QMessageBox::information(this, "Exportation réussie", "Les statistiques ont été exportées avec succès.");
}*/

// Function to export the table data to a PDF

void MainWindow::on_pushButton_pdf_clicked()
{
    // Demander à l'utilisateur où enregistrer le fichier PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le fichier PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Annulé", "La génération du PDF a été annulée.");
        return;
    }

    // Préparer le contenu HTML
    QString htmlContent;
    htmlContent.append("<html>");
    htmlContent.append("<head><style>");
    htmlContent.append("table { width: 100%; border-collapse: collapse; margin: 20px 0; font-size: 12px; text-align: left; }");
    htmlContent.append("th, td { border: 1px solid #dddddd; padding: 8px; }");
    htmlContent.append("th { background-color: #f2f2f2; }");
    htmlContent.append("h1 { text-align: center; }");
    htmlContent.append("</style></head>");
    htmlContent.append("<body>");

    htmlContent.append("<h1>Rapport des Performances</h1>");
    htmlContent.append("<table>");
    htmlContent.append("<tr>"
                       "<th>ID Performance</th>"
                       "<th>CIN Employé</th>"
                       "<th>Date Evaluation</th>"
                       "<th>Score</th>"
                       "<th>Commentaire</th>"
                       "</tr>");

    // Récupérer les données de la table `performance`
    QSqlQuery query;
    if (!query.exec("SELECT id_performance, cin_employe, date_evaluation, score, commentaire FROM performance")) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible de récupérer les données : " + query.lastError().text());
        return;
    }

    // Parcourir les résultats et ajouter les données dans le tableau HTML
    while (query.next()) {
        QString idPerformance = query.value(0).toString();
        QString cinEmploye = query.value(1).toString();
        QString dateEvaluation = query.value(2).toDate().toString("dd/MM/yyyy");
        QString score = query.value(3).toString();
        QString commentaire = query.value(4).toString();

        htmlContent.append("<tr>");
        htmlContent.append("<td>" + idPerformance + "</td>");
        htmlContent.append("<td>" + cinEmploye + "</td>");
        htmlContent.append("<td>" + dateEvaluation + "</td>");
        htmlContent.append("<td>" + score + "</td>");
        htmlContent.append("<td>" + commentaire + "</td>");
        htmlContent.append("</tr>");
    }

    htmlContent.append("</table>");
    htmlContent.append("</body>");
    htmlContent.append("</html>");

    // Générer le PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(htmlContent);
    doc.print(&printer);

    QMessageBox::information(this, "Succès", "Le PDF a été généré avec succès !");
}


// MainWindow.cpp
void MainWindow::on_comboBox_trier_currentIndexChanged(int index)
{
    QString queryStr;

    // Vérifier l'index sélectionné dans comboBox_trier pour déterminer le critère de tri
    switch (index) {
        case 0:  // Tri par salaire
            queryStr = "SELECT * FROM employe ORDER BY salaire ASC";  // Tri croissant
            break;
        case 1:  // Tri par date d'embauche
            queryStr = "SELECT * FROM employe ORDER BY date_embauche ASC";  // Tri croissant
            break;
        case 2:  // Tri par poste
            queryStr = "SELECT * FROM employe ORDER BY poste ASC";  // Tri croissant
            break;
        default:
            return;
    }

    QSqlQuery query;
    query.prepare(queryStr);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur de requête", "Une erreur est survenue lors de l'exécution de la requête.");
        return;
    }

    // Créez un modèle pour afficher les résultats
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(std::move(query));

    // Si des résultats sont trouvés, affichez-les dans le QTableView
    if (model->rowCount() > 0) {
        ui->tableView->setModel(model);
    } else {
        QMessageBox::information(this, "Aucun résultat", "Aucun employé trouvé.");
    }
}


// Function to handle searching for an employee by CIN
void MainWindow::on_comboBox_recherche_currentIndexChanged(int index)
{
    QString queryStr;
    QString cin = ui->lineEdit_cin->text();
    QString nom = ui->lineEdit_nom->text();
    QString sexe = ui->lineEdit_sexe->text();

    // Vérifier l'index sélectionné dans comboBox_recherche pour déterminer le critère de recherche
    switch (index) {
        case 0:  // Recherche par CIN
            if (cin.isEmpty()) {
                QMessageBox::warning(this, "Entrée manquante", "Veuillez entrer un CIN.");
                return;
            }
            queryStr = QString("SELECT * FROM employe WHERE cin = %1").arg(cin);
            break;
        case 1:  // Recherche par nom
            if (nom.isEmpty()) {
                QMessageBox::warning(this, "Entrée manquante", "Veuillez entrer un nom.");
                return;
            }
            queryStr = QString("SELECT * FROM employe WHERE nom LIKE '%%1%'").arg(nom);
            break;
        case 2:  // Recherche par sexe
            if (sexe.isEmpty()) {
                QMessageBox::warning(this, "Entrée manquante", "Veuillez entrer un sexe.");
                return;
            }
            queryStr = QString("SELECT * FROM employe WHERE sexe = '%1'").arg(sexe);
            break;
        default:
            return;
    }

    QSqlQuery query;
    query.prepare(queryStr);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur de requête", "Une erreur est survenue lors de la recherche.");
        return;
    }

    // Créez un modèle pour afficher les résultats
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(std::move(query));

    // Si des résultats sont trouvés, affichez-les dans le QTableView
    if (model->rowCount() > 0) {
        ui->tableView->setModel(model);
    } else {
        QMessageBox::information(this, "Aucun résultat", "Aucun employé trouvé.");
    }
}

// Function to validate inputs
bool MainWindow::validateInputs(int cin, const QString &nom, const QString &prenom,
                                const QString &question, const QString &sexe,
                                const QString &poste, float salaire,
                                const QString &mot_de_passe) {    // CIN validation
    if (cin < 10000000 || cin > 99999999) {
        QMessageBox::warning(this, "Entrée invalide", "CIN doit être un entier positif composé de 8 chiffres.");
        return false;
    }

    // Name validation
    QRegularExpression nameRegex("^[A-Za-z]{2,50}$");
    if (!nameRegex.match(nom).hasMatch()) {
        QMessageBox::warning(this, "Entrée invalide", "Le nom doit contenir uniquement des lettres et être entre 2 et 50 caractères.");
        return false;
    }

    // Prenom validation
    if (!nameRegex.match(prenom).hasMatch()) {
        QMessageBox::warning(this, "Entrée invalide", "Le prénom doit contenir uniquement des lettres et être entre 2 et 50 caractères.");
        return false;
    }

    // Question validation
    if (question.isEmpty() || question.length() > 100) {
            QMessageBox::warning(this, "Entrée invalide", "La question ne peut pas être vide et doit contenir moins de 100 caractères.");
            return false;
        }

    // Sexe validation
    if (sexe != "M" && sexe != "F") {
        QMessageBox::warning(this, "Entrée invalide", "Le sexe doit être 'M' ou 'F'.");
        return false;
    }

    // Poste validation
    if (poste.isEmpty()) {
        QMessageBox::warning(this, "Entrée invalide", "Le poste ne peut pas être vide.");
        return false;
    }

    // Salaire validation
    if (salaire <= 0) {
        QMessageBox::warning(this, "Entrée invalide", "Le salaire doit être un nombre positif.");
        return false;
    }

    // Mot de passe validation
    if (mot_de_passe.length() < 8) {
        QMessageBox::warning(this, "Entrée invalide", "Le mot de passe doit contenir au moins 8 caractères.");
        return false;
    }

    QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,}$");
    if (!passwordRegex.match(mot_de_passe).hasMatch()) {
        QMessageBox::warning(this, "Entrée invalide", "Le mot de passe doit contenir au moins une lettre majuscule, une lettre minuscule, un chiffre et un caractère spécial.");
        return false;
    }

    return true;
}

// Add employee function
void MainWindow::on_pushButton_ajouter_clicked() {
    int cin = ui->lineEdit_cin->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString question = ui->lineEdit_question->text();
    QString sexe = ui->lineEdit_sexe->text();
    QString poste = ui->comboBox_poste->currentText();
    QDate date_embauche = ui->dateEdit_date_embauche->date();
    float salaire = ui->lineEdit_salaire->text().toFloat();
    QString mot_de_passe = ui->lineEdit_mdp->text();

    if (!validateInputs(cin, nom, prenom, question, sexe, poste, salaire, mot_de_passe)) {
        return;
    }

    employe.setCin(cin);
    employe.setNom(nom);
    employe.setPrenom(prenom);
    employe.setQuestion(question);
    employe.setSexe(sexe);
    employe.setPoste(poste);
    employe.setDate_embauche(date_embauche);
    employe.setSalaire(salaire);
    employe.setMot_de_passe(mot_de_passe);

    if (employe.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès.");
        ui->tableView->setModel(employe.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé.");
    }
}

// Modify employee function
void MainWindow::on_pushButton_modifier_clicked() {
    int cin = ui->lineEdit_cin->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString question = ui->lineEdit_question->text();
    QString sexe = ui->lineEdit_sexe->text();
    QString poste = ui->comboBox_poste->currentText();
    QDate date_embauche = ui->dateEdit_date_embauche->date();
    float salaire = ui->lineEdit_salaire->text().toFloat();
    QString mot_de_passe = ui->lineEdit_mdp->text();

    if (!validateInputs(cin, nom, prenom, question, sexe, poste, salaire, mot_de_passe)) {
        return;
    }

    Employe emp(cin, nom, prenom, question, sexe, poste, date_embauche, salaire, mot_de_passe);
    if (emp.modifier(cin)) {
        ui->tableView->setModel(employe.afficher());
        QMessageBox::information(this, "Modification", "Employé modifié avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de l'employé.");
    }
}

// Delete employee function
void MainWindow::on_pushButton_supprimer_clicked() {
    int id = ui->lineEdit_cin->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Entrée invalide", "CIN doit être un entier positif.");
        return;
    }

    if (employe.supprimer(id)) {
        ui->tableView->setModel(employe.afficher());
        QMessageBox::information(this, "Suppression", "Employé supprimé avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression de l'employé.");
    }
}



void MainWindow::on_pushButton_27_clicked()
{
    Employe e;
        try{
        //Crée un graphique (QChart) pour afficher les statistiques.
            QChart *chart = new QChart();
            QBarSeries *series = new QBarSeries();
           /*Crée une instance d'axe de catégorie (QBarCategoryAxis) pour l'axe X.*/
            QBarCategoryAxis *axis = new QBarCategoryAxis();

            QBarSet *set = new QBarSet(" Point de fidelite");
            QStringList typesList;
            QList<QBarSet *> nbrList;
            std::map<QString , int> list = e.statNbrPerType();
            for(auto itr = list.begin() ; itr != list.end(); itr++) {
                typesList.append(itr->first);
    //            nbrList.append(itr->second);
                *set << itr->second;
                nbrList.append(set);
            }
            qDebug() << typesList;
            series->append(set);
            chart->addSeries(series);
            chart->setAnimationOptions(QChart::AllAnimations);
            axis->append(typesList);
            chart->createDefaultAxes();
            chart->addAxis(axis, Qt::AlignBottom); // Attach the axis to the chart
            series->attachAxis(axis);
            chart->legend()->setAlignment(Qt::AlignBottom);
            //Crée une vue graphique (QChartView) et active l'anticrénelage pour un rendu plus lisse.
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            QPalette pal = qApp->palette();
            pal.setColor(QPalette::Window, QRgb(0x0d4261));
            pal.setColor(QPalette::WindowText, QRgb(0x95212c));
            qApp->setPalette(pal);
            QFont font;
            font.setPixelSize(40);
            chart->setTitleFont(font);
            chart->setTitleBrush(QBrush(Qt::red));
            chart->setTitle("statistique Point fidelite PER IDC");
            chartView->setChart(chart);
            chartView->showNormal();



        }catch(...){

}
}
