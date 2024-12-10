#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transaction.h"
#include "mailling.h"
#include "notesdialog.h"
#include "arduino.h"
#include <QMessageBox>
#include <QStackedWidget>
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
#include <QDebug>
#include <QAction>
#include <QDoubleValidator>
#include <QFile>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslConfiguration>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDate>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QPrinter>
#include <QPageLayout>

#include <QTextDocument>
#include <QFileDialog>
#include <QDate>
#include <QFileInfo>
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
#include <QMap>
#include <QString>
#include <QRegularExpression>

#include <QRegularExpression>
#include <QDate>

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

#include "service.h"

#include <QSqlTableModel>
#include <QTableView>
 // Add this include to resolve the error
#include <QPdfWriter>

#include <QUrlQuery>
#include "sms.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), T(0, "", "", "", "", 0.0), model(new QSqlQueryModel(this)),
    service(new Service()), // Initialize the Service object
               smsManager(new SmsManager(this))
{
    ui->setupUi(this);
        updateChambreStats();
        QPixmap totallroom("C:/Users/USER/Desktop/roomtot.png");
        ui->label_2->setPixmap(totallroom);
        ui->label_2->setScaledContents(true);
        QPixmap available("C:/Users/USER/Desktop/cle.png");
        ui->label_29->setPixmap(available);
        ui->label_29->setScaledContents(true);
    QPixmap stat("C:/Users/USER/Desktop/img.png");
    ui->label_101->setPixmap(stat);
    ui->label_101->setScaledContents(true);
    QPixmap login("C:/Users/USER/Desktop/img3.jpg");
    ui->label_3->setPixmap(login);
    ui->label_3->setScaledContents(true);
    QPixmap pdf("C:/Users/USER/Desktop/img4.png");
    ui->label_99->setPixmap(pdf);
    ui->label_99->setScaledContents(true);
    QPixmap stat2("C:/Users/USER/Desktop/img5.png");
    ui->label_6->setPixmap(stat2);
    ui->label_6->setScaledContents(true);
    QPixmap calcull("C:/Users/USER/Desktop/img2.png");
    ui->label_6->setPixmap(calcull);
    ui->label_6->setScaledContents(true);
    QPixmap ftour("C:/Users/USER/Desktop/ftoursbeh.jpg");
    ui->label_28->setPixmap(ftour);
    ui->label_28->setScaledContents(true);
    QPixmap slata("C:/Users/USER/Desktop/slata.jpg");
    ui->label_33->setPixmap(slata);
    ui->label_33->setScaledContents(true);
    QPixmap makrouna("C:/Users/USER/Desktop/makrouna.jpg");
    ui->label_35->setPixmap(makrouna);
    ui->label_35->setScaledContents(true);
    QPixmap burger("C:/Users/USER/Desktop/burger.jpeg");
    ui->label_70->setPixmap(burger);
    ui->label_70->setScaledContents(true);
    QPixmap chorba("C:/Users/USER/Desktop/chorba.jpeg");
    ui->label_68->setPixmap(chorba);
    ui->label_68->setScaledContents(true);
    QPixmap lasagne("C:/Users/USER/Desktop/lasagne.jpeg");
    ui->label_69->setPixmap(lasagne);
    ui->label_69->setScaledContents(true);

    ui->tableView_reservation_2->setModel(this->reservation.afficher());
    ui->tableView_reservation_2->resizeColumnsToContents();

    // Initialize the Arduino object and connect it to the dataReceived signal
    arduino = new Arduino(this);
    connect(arduino, &Arduino::dataReceived, this, &MainWindow::handleArduinoData);

    if (!arduino->connectArduino()) {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à Arduino.");
    }

    // Setup system tray icon
    setupTrayIcon();

    // Set up input validation for montant
    QDoubleValidator *montantValidator = new QDoubleValidator(0, 1000000, 2, this);
    montantValidator->setLocale(QLocale::English);
    ui->lineEdit_3->setValidator(montantValidator);
//dashbord bouton
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::navigateToDashboard);
    // Call function to refresh the table view with data
    refreshTableView();

    // Set maximum expense threshold
    maxDepense = 1000.0;
    //service
    connect(ui->pushButton_afficher, &QPushButton::clicked, this, &MainWindow::refreshTableView);
    connect(ui->pushButton_34, &QPushButton::clicked, this, &MainWindow::on_pushButton_34_clicked);

    // Connect button actions to their respective slots
    connect(ui->pushButton_afficher_3, &QPushButton::clicked, this, &MainWindow::refreshTableView2);
    connect(ui->buttonRechercher_3, &QPushButton::clicked, this, &MainWindow::on_buttonRechercher_3_clicked);
    connect(ui->pushButton_pdf_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_pdf_4_clicked);
    connect(ui->pushButton_QR_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_QR_4_clicked);

    connect(ui->pushButton_plus_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_plus_4_clicked);
    connect(ui->pushButton_mois_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_mois_4_clicked);
    connect(ui->pushButton_multiplication_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_multiplication_4_clicked);
    connect(ui->pushButton_diviser_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_diviser_4_clicked);
    afficherClients();
    connect(ui->afficherStatistiquesButton, &QPushButton::clicked, this, &MainWindow::on_afficherStatistiquesButton_clicked);

    connect(ui->chercherButton, &QPushButton::clicked, this, &MainWindow::on_chercherButton_clicked);
    connect(ui->exportCsvButton_2, &QPushButton::clicked, this, &MainWindow::exportCsvButton_2);
    connect(ui->arriveeButton, &QPushButton::clicked, this, &MainWindow::enregistrerArrivee);
    connect(ui->historiqueButton, &QPushButton::clicked, this, &MainWindow::afficherHistorique);
    connect(ui->modifierButton, &QPushButton::clicked, this, &MainWindow::on_modifierButton_clicked);
    connect(ui->chatbutton, &QPushButton::clicked, this, &MainWindow::on_chatbutton_clicked);



    // Connect saving and quitting actions
    QAction *saveAction = new QAction(this);
    saveAction->setShortcut(QKeySequence::Save);  // Ctrl + S
    connect(saveAction, &QAction::triggered, this, &MainWindow::on_save);
    addAction(saveAction);

    QAction *quitAction = new QAction(this);
    quitAction->setShortcut(QKeySequence::Quit);  // Ctrl + Q
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    addAction(quitAction);

    // Connect additional functionality buttons
    connect(ui->pushButton_47, SIGNAL(clicked()), this, SLOT(on_pushButton_calculerTVA_clicked()));

    connect(ui->pushButton_13, &QPushButton::clicked, this, &MainWindow::on_pushButton_stat_clicked);
    connect(ui->pushButton_15, &QPushButton::clicked, this, &MainWindow::on_pushButton_15_clicked);
    connect(ui->pushButton_44, &QPushButton::clicked, this, &MainWindow::on_pushButton_44_clicked);
    connect(ui->comboBox_18, &QComboBox::currentTextChanged, this, &MainWindow::on_comboBox_18_selectionChanged);

    connect(ui->pushButton_12, &QPushButton::clicked, this, [=]() {


        generatePdfForPeriod(ui->dateEdit_3->date(), ui->dateEdit_4->date());
    });
    // Set up placeholders and masked input
    ui->lineEdit_cin2->setPlaceholderText("Entrez votre CIN");
    ui->lineEdit_mdp2->setPlaceholderText("Entrez votre mot de passe");
    ui->lineEdit_mdp2->setEchoMode(QLineEdit::Password);

    // Set up employee management
    ui->tableView->setModel(employe.afficher());
    ui->dateEdit_7->setDate(QDate::currentDate());

   /* serial = new QSerialPort(this);
    serial->setPortName("COM5"); // Remplacez par le port de votre Arduino
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

    if (!serial->open(QIODevice::ReadOnly)) {
        qDebug() << "Erreur : Impossible d'ouvrir le port série.";
    }*/

    connect(ui->pushButton_40, &QPushButton::clicked, this, &MainWindow::on_pushButton_40_clicked);


    // Connect CAPTCHA generation and validation
    connect(ui->refreshCaptchaButton, &QPushButton::clicked, this, &MainWindow::regenerateCaptcha);
    connect(ui->validateCaptchaButton, &QPushButton::clicked, this, &MainWindow::validateCaptcha);

//connect tri
    connect(ui->comboBox_5, SIGNAL(currentIndexChanged(int)),
               this, SLOT(on_comboBox_trier_currentIndexChanged(int)));
//basculement
    connect(ui->pushButton_login, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    //bouton logout
        connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    //recherche employe
    connect(ui->pushButton_39, &QPushButton::clicked, this, &MainWindow::on_pushButton_39_clicked);

        // Vous pouvez aussi appeler la fonction refreshTableViewrecherche pour afficher les données initiales
        refreshTableViewrecherche();


    // Generate initial CAPTCHA
    regenerateCaptcha();


    //Gestion Reservation ****



    ui->dateEdit_arrive_3->setDate(QDate::currentDate());
    ui->dateEdit_depart_3->setDate(QDate::currentDate());

    connect(ui->tabWidget_2, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    connect(ui->comboBox_stat_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateChart);
    connect(ui->comboBox_tri_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::TriChanged);

    connect(ui->tabWidget_2, &QTabWidget::currentChanged, this, &MainWindow::updateCalendrier);
    updateCalendrier();

    connect(ui->calendarWidget_2, &QCalendarWidget::clicked, this, &MainWindow::onDateSelected);

    A.is_connected = 0;

    int ret=A.connect_arduino("com7"); // lancer la connexion à arduino
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
    //arduinoThread = new QThread(this);
    //connect(arduinoThread, &QThread::started, this, &MainWindow::readArduinoContinuously);
    connect(this, &MainWindow::motionDetected, this, &MainWindow::showMotionMessage);
    //arduinoThread = new QThread(this);
    QTimer *arduinoTimer = new QTimer(this);

    connect(arduinoTimer, &QTimer::timeout, this, &MainWindow::readArduinoContinuously);
    if (A.is_connected) {
        arduinoTimer->start(1000); // Check Arduino every 1000ms
    }

    connect(ui->pushButton_readArduino_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_readArduino_2_clicked);
    connect(ui->pushButton_20, &QPushButton::clicked, this, &MainWindow::on_pushButton_20_clicked);



    ui->tableView_reservation_2->setModel(this->reservation.afficher());
    ui->tableView_reservation_2->resizeColumnsToContents();

    timenow = QDateTime::currentDateTime();
    derniereDetectMouvement = QDateTime::currentDateTime();

    arduino = new Arduino(this);

                // Connecting the signal to the appropriate slot
                connect(arduino, &Arduino::clientFound, this, &MainWindow::onClientFound);

                int baudRate = 9600; // Default baud rate
                if (arduino->openConnection2(baudRate)) {
                    qDebug() << "Arduino connecté avec succès!";
                } else {
                    qDebug() << "Échec de la connexion à l'Arduino.";
                }


}

MainWindow::~MainWindow()
{
    delete service; // Release allocated memory
    delete smsManager;
    delete ui;
    arduino->disconnectArduino();

}
//dashboard navig
void MainWindow::navigateToDashboard()
{
    // Remplacez `0` par l'index de la page du dashboard dans le QStackedWidget
    ui->stackedWidget->setCurrentIndex(0);

    // Si vous préférez utiliser le widget directement
    // ui->stackedWidget->setCurrentWidget(ui->dashboardPage);
}
//dashbord room total
void MainWindow::updateChambreStats()
{
    int totalChambres = 10;  // Exemple statique
    int chambresDisponibles = 5;  // Exemple statique

    ui->label_72->setText(QString::number(totalChambres) + " chambres au total");
    ui->label_73->setText(QString::number(chambresDisponibles) + " disponibles");
}




//arduinoemploye
void MainWindow::onClientFound(const QString &RFID) {
    qDebug() << "UID reçu : " << RFID;

    // Nettoyer l'UID en supprimant les espaces
    QString cleanedRFID = RFID.trimmed();
    qDebug() << "UID nettoyé : " << cleanedRFID;

    // Requête SQL pour récupérer les informations de l'employé par RFID
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM FROM EMPLOYE WHERE RFID = :RFID");
    query.bindValue(":RFID", cleanedRFID);

    if (query.exec()) {
        if (query.next()) {
            // Client trouvé
            QString clientInfo = QString("CIN : %1\nNOM : %2")
                                        .arg(query.value(0).toString())  // CIN
                                        .arg(query.value(1).toString()); // NOM
            QMessageBox::information(this, "Client trouvé", clientInfo);

            // Préparer le message pour l'écran LCD
            QString nom = query.value(1).toString();     // NOM
            QString prenom = query.value(2).toString();  // PRENOM

            // Limitez les longueurs des chaînes pour l'affichage LCD
            nom = nom.left(16);
            prenom = prenom.left(16);

            QString lcdMessage = QString("%1,%2\n").arg(nom).arg(prenom);
            qDebug() << "Message envoyé à Arduino : " << lcdMessage;

            // Envoyez le message à l'Arduino
            arduino->writeData2(lcdMessage.toUtf8());

        } else {
            // Aucun employé trouvé pour ce RFID
            qDebug() << "Aucun employé trouvé pour ce RFID";
            QMessageBox::warning(this, "Employé introuvable", "Aucun employé correspondant trouvé pour cet UID.");

            // Envoyer "UNKNOWN" à l'Arduino
            arduino->writeData2("Aucun employe trouve \n");
        }
    } else {
        // Afficher l'erreur SQL dans le cas où la requête échoue
        qDebug() << "Erreur SQL : " << query.lastError().text();
        QMessageBox::critical(this, "Erreur SQL", "Erreur lors de la requête.");

        // Envoyer "UNKNOWN" à l'Arduino en cas d'erreur de requête
        arduino->writeData2("Arduino en cas \n");
    }
}

void MainWindow::onLoginClicked() {
    QString cin = ui->lineEdit_cin2->text();
    QString mot_de_passe = ui->lineEdit_mdp2->text();

    if (authenticate(cin, mot_de_passe)) {
        qDebug() << "Authentification réussie!";

        // Navigation dans le stackedWidget_2
        if (mot_de_passe.startsWith("Tran")) {
            ui->stackedWidget_2->setCurrentIndex(2); // Page 10
            qDebug() << "Navigation vers la page 10 du stackedWidget_2";

            // Navigation dans le stackedWidget à l'intérieur de la Page 10 (Page 3)
            ui->stackedWidget_2->widget(2)->findChild<QStackedWidget*>()->setCurrentIndex(2); // Page 3 du stackedWidget
            qDebug() << "Navigation vers la page 3 du stackedWidget de la page 10";
        } else if (mot_de_passe.startsWith("Cl")) {
            ui->stackedWidget_2->setCurrentIndex(2); // Page 9
            qDebug() << "Navigation vers la page 9 du stackedWidget_2";

            // Navigation dans le stackedWidget à l'intérieur de la Page 9 (Page 5)
            ui->stackedWidget_2->widget(2)->findChild<QStackedWidget*>()->setCurrentIndex(4); // Page 5 du stackedWidget
            qDebug() << "Navigation vers la page 5 du stackedWidget de la page 9";
        } else if (mot_de_passe.startsWith("Cham")) {
            ui->stackedWidget_2->setCurrentIndex(2); // Page 8
            qDebug() << "Navigation vers la page 8 du stackedWidget_2";

            // Navigation dans le stackedWidget à l'intérieur de la Page 8 (Page 4)
            ui->stackedWidget_2->widget(2)->findChild<QStackedWidget*>()->setCurrentIndex(3); // Page 4 du stackedWidget
            qDebug() << "Navigation vers la page 4 du stackedWidget de la page 8";
        } else if (mot_de_passe.startsWith("Emp")) {
            ui->stackedWidget_2->setCurrentIndex(2); // Page 7
            qDebug() << "Navigation vers la page 7 du stackedWidget_2";

            // Navigation dans le stackedWidget à l'intérieur de la Page 7
            // Remplacez cette ligne par la page spécifique que vous souhaitez afficher si nécessaire.
            ui->stackedWidget_2->widget(2)->findChild<QStackedWidget*>()->setCurrentIndex(6); // Par exemple, page 1 du stackedWidget de la page 7
            qDebug() << "Navigation vers la page 1 du stackedWidget de la page 7";
        } else if (mot_de_passe.startsWith("Ser")) {
            ui->stackedWidget_2->setCurrentIndex(2); // Page 6
            qDebug() << "Navigation vers la page 6 du stackedWidget_2";

            // Navigation dans le stackedWidget à l'intérieur de la Page 6 (Page 6)
            ui->stackedWidget_2->widget(2)->findChild<QStackedWidget*>()->setCurrentIndex(5); // Page 6 du stackedWidget
            qDebug() << "Navigation vers la page 6 du stackedWidget de la page 6";
        } else if (mot_de_passe.startsWith("Res")) {
            ui->stackedWidget_2->setCurrentIndex(2); // Page 2
            qDebug() << "Navigation vers la page 2 du stackedWidget_2";

            // Navigation dans le stackedWidget à l'intérieur de la Page 2 (Page 2)
            ui->stackedWidget_2->widget(2)->findChild<QStackedWidget*>()->setCurrentIndex(1); // Page 2 du stackedWidget de la page 2
            qDebug() << "Navigation vers la page 2 du stackedWidget de la page 2";
        } else {
            QMessageBox::information(this, "Info", "Mot de passe valide, mais sans redirection spécifique.");
        }
    }
}
void MainWindow::onLogoutClicked() {
    // Naviguer vers la page 9 (interface de login) dans stackedWidget_2
    ui->stackedWidget_2->setCurrentIndex(0); // Page 9 correspondant à l'interface de login
    qDebug() << "Utilisateur déconnecté, retour à la page de login.";
}




void MainWindow::refreshTableView()
{

    model->setQuery("SELECT id_transaction, type_transaction, date_transaction, etat_paiement, mode_paiement, montant_total FROM transaction");


    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mode"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Montant"));



    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
     sendMailIfMaxDepense();
}


void MainWindow::on_pushButton_14_clicked()
{
    int id = ui->lineEdit_2->text().toInt();
    QString type = ui->comboBox->currentText();
    QString dateTime = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString etat = ui->comboBox_3->currentText();
    QString mode = ui->comboBox_2->currentText();
    double montant = ui->lineEdit_3->text().toDouble();

    Transaction T(id, type, dateTime,etat, mode,  montant);

    bool test = T.ajouter();
    if (test) {
        QMessageBox::information(this, "Succès", "Transaction ajoutée.");
        refreshTableView();  // Actualiser l'affichage
        sendMailIfMaxDepense();
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
        refreshTableView();
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
        refreshTableView();
        sendMailIfMaxDepense();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification.");
    }
}
void MainWindow::on_pushButton_44_clicked()
{

    QSqlQueryModel *modelTableView2 = new QSqlQueryModel(this);

    QString searchField = ui->comboBox_17->currentText();
    QString searchValue = ui->lineEdit_26->text().trimmed();

    if (searchField == "date du transaction") {
        // Vérifier si la date est valide
        QDate date = QDate::fromString(searchValue, "yyyy-MM-dd");
        if (!date.isValid()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer une date valide au format yyyy-MM-dd.");
            return;
        }


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

    QString selectedCriteria = ui->comboBox_18->currentText();


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


    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors du tri des transactions : " + model->lastError().text());
    } else {
        // Mettre à jour l'affichage du QTableView
        ui->tableView_2->setModel(model);
        ui->tableView_2->resizeColumnsToContents();
    }
}
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
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Q && (event->modifiers() & Qt::ControlModifier)) {
        // Close the application when Ctrl + Q is pressed
        QApplication::quit();
    } else {
        // Handle other key events (optional)
        QMainWindow::keyPressEvent(event);  // Call the base class event handler
    }
}

void MainWindow::on_save()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Save Error", "Could not open file for writing.");
        return;
    }

    QTextStream out(&file);
    out << "Example data to save.\n";  // Use Qt::endl instead of std::endl
    file.close();

    QMessageBox::information(this, "Save", "File saved successfully.");
}



void MainWindow::on_pushButton_ajouter_3_clicked()
{
    QString idText = ui->lineEdit_id_chambre_3->text();
    QString prixText = ui->lineEdit_prix_3->text();

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
    if (ui->radioButton_normal_3->isChecked()) {
        type = "normal";
    } else if (ui->radioButton_double_3->isChecked()) {
        type = "double";
    } else if (ui->radioButton_suite_3->isChecked()) {
        type = "suite";
    }

    QString categorie = ui->comboBox_categorie_3->currentText();

    chambre.setIdChambre(id);
    chambre.setTarif(prix);
    chambre.setType(type);
    chambre.setCategorie(categorie);
    chambre.setStatutChambre("reserve");
    bool success = chambre.ajouter();

    if (success) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\nClick Cancel to exit."),
                                 QMessageBox::Cancel);
         refreshTableView2();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\nClick Cancel to exit."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_modifier_3_clicked()
{
    QString idText = ui->lineEdit_id_chambre_3->text();
    QString prixText = ui->lineEdit_prix_3->text();

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
    if (ui->radioButton_normal_3->isChecked()) {
        type = "normal";
    } else if (ui->radioButton_double_3->isChecked()) {
        type = "double";
    } else if (ui->radioButton_suite_3->isChecked()) {
        type = "suite";
    }

    QString categorie = ui->comboBox_categorie_3->currentText();
    QString statut_chambre = "Unavailable";

    bool success = chambre.modifier(id, categorie, type, statut_chambre, prix);

    if (success) {
        QMessageBox::information(this, tr("Modification réussie"),
                                 tr("La chambre a été modifiée avec succès."));
         refreshTableView2();
    } else {
        QMessageBox::critical(this, tr("Erreur de modification"),
                              tr("L'ID de la chambre n'existe pas. La modification n'a pas été effectuée."));
    }
}

void MainWindow::on_pushButton_supprimer_3_clicked()
{
    int id = ui->lineEdit_id_chambre_3->text().toInt();
    bool success = chambre.supprimer(id);

    if (success) {
        QMessageBox::information(this, tr("Suppression réussie"),
                                 tr("La chambre a été supprimée avec succès."));
       refreshTableView2();
    } else {
        QMessageBox::critical(this, tr("Erreur de suppression"),
                              tr("L'ID de la chambre n'existe pas. La suppression n'a pas été effectuée."));
    }
}

void MainWindow::refreshTableView2()
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

    ui->tableView_7->setModel(model);
    ui->tableView_7->resizeColumnsToContents();
}

void MainWindow::on_buttonRechercher_3_clicked()
{
    QString input = ui->lineEdit_recherche_5->text().trimmed();
    QString modeRecherche = ui->comboBox_33->currentText().trimmed();
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
        ui->tableView_8->setModel(model);
        ui->tableView_8->resizeColumnsToContents();
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
        ui->tableView_8->setModel(model);
        ui->tableView_8->resizeColumnsToContents();
    }
    else if (modeRecherche == "Rechercher_par_Type") {
        if (input.isEmpty()) {
            QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un type de chambre : normal, double, ou suite.");
            return;
        }

        // Validate input: only allow "normal", "double", or "suite"
        QStringList validTypes = {"normal", "double", "suite"};
        if (!validTypes.contains(input, Qt::CaseInsensitive)) {
            QMessageBox::warning(this, "Type invalide", "Le type de chambre doit être : normal, double, ou suite.");
            return;
        }

        QSqlQueryModel *model = new QSqlQueryModel();
        QSqlQuery query;
        query.prepare("SELECT id_chambre, categorie, type, statut_chambre, tarif "
                      "FROM chambre WHERE LOWER(type) = :type");
        query.bindValue(":type", input.toLower());

        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur SQL", "Erreur lors de l'exécution de la requête : " + query.lastError().text());
            return;
        }

        if (query.size() == 0) {
            QMessageBox::information(this, "Aucune correspondance", "Aucune chambre trouvée pour ce type.");
            return;
        }

        model->setQuery(std::move(query));
        ui->tableView_8->setModel(model);
        ui->tableView_8->resizeColumnsToContents();
    }

    else {
        QMessageBox::warning(this, "Mode inconnu", "Mode de recherche inconnu");
    }
}

void MainWindow::on_buttonTrier_6_clicked()
{
    QString modeTri = ui->comboBox_11->currentText().trimmed();
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
    else if (modeTri == "Trier_par_Id") {
        query.prepare("SELECT id_chambre, categorie, type, statut_chambre, tarif "
                      "FROM chambre ORDER BY id_chambre ASC");
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

        ui->tableView_8->setModel(model);
        ui->tableView_8->resizeColumnsToContents();

        qDebug() << "Data displayed in the table.";
    }
}


void MainWindow::on_pushButton_pdf_4_clicked()
{
    // Query for statistics
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

    // File dialog to save the PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "*.pdf");
    if (fileName.isEmpty()) return; // User canceled the dialog
    if (!fileName.endsWith(".pdf")) fileName += ".pdf";
//repon

    // Initialize PDF writer
    QPdfWriter pdfWriter(fileName);
 /*   pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMargins(20, 20, 20, 20));
*/
    QPainter painter(&pdfWriter);

    // Title Section
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(200, 100, "Assigning Shortcuts to Menu Actions");

    // Shortcuts Content
    painter.setFont(QFont("Arial", 12));
    int y = 300; // Start position for the shortcuts section
    int lineSpacing = 300; // Line spacing for shortcuts

    painter.drawText(100, y, "Ctrl + C: Copy selected text/data.");
    y += lineSpacing;
    painter.drawText(100, y, "Ctrl + V: Paste text/data.");
    y += lineSpacing;
    painter.drawText(100, y, "Ctrl + S: Save changes.");
    y += lineSpacing;
    painter.drawText(100, y, "Ctrl + Q: Quit application.");
    y += lineSpacing * 2; // Extra spacing before statistics

    // Type Statistics Section
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(200, y, "Statistiques par Type");
    y += lineSpacing;

    painter.setFont(QFont("Arial", 12));
    for (auto it = typePercentages.begin(); it != typePercentages.end(); ++it) {
        QString line = QString("%1: %2%").arg(it.key()).arg(QString::number(it.value(), 'f', 2));
        painter.drawText(100, y, line);
        y += lineSpacing; // Increment y-position for the next line
    }

    // End PDF
    painter.end();

    QMessageBox::information(this, "PDF Created", "The PDF has been successfully created.");
}








void MainWindow::on_pushButton_statistique_4_clicked()
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


void MainWindow::on_pushButton_QR_4_clicked()
{

    // Répertoire pour enregistrer les QR codes
    QString outputDir = QString("%1/QR_Codes").arg(QDir::currentPath());
    if (!QDir(outputDir).exists()) {
        QDir().mkdir(outputDir);
        qDebug() << "Répertoire créé pour les QR codes : " << outputDir;
    }

    // Vérifier si qrencode.exe existe
    QString qrencodePath = "C:/Users/USER/Documents/integrationfinale/tools/qrencode.exe";
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

void MainWindow::on_pushButton_list_4_clicked()
{
    QString roomId = ui->lineEdit_id3_4->text();

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
    query.bindValue(":room_id", roomIdInt);

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

void MainWindow::on_pushButton_print_4_clicked()
{
    QString roomId = ui->lineEdit_id3_4->text();

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

    // Fetch the notes for the specified room ID
    QSqlQuery noteQuery;
    noteQuery.prepare("SELECT note FROM notes_history WHERE room_id = :room_id");
    noteQuery.bindValue(":room_id", roomIdInt);

    if (!noteQuery.exec()) {
        QMessageBox::critical(this, "SQL Error", noteQuery.lastError().text());
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(noteQuery);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Info", "No notes found for this room.");
        return;
    }

    // Set the model to the table view
    ui->tableView_list_4->setModel(model);

    // Make the 'note' column take the full width
    ui->tableView_list_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void MainWindow::on_pushButton_plus_4_clicked()
{
    // Get values from lineEdit_champ1 and lineEdit_champ2
    double champ1 = ui->lineEdit_champ1_4->text().toDouble();
    double champ2 = ui->lineEdit_champ2_4->text().toDouble();

    // Perform addition
    double result = champ1 + champ2;

    // Display the result in lineEdit_resultat
    ui->lineEdit_resultat_4->setText(QString::number(result));
}

void MainWindow::on_pushButton_mois_4_clicked()
{
    // Get values from lineEdit_champ1 and lineEdit_champ2
    double champ1 = ui->lineEdit_champ1_4->text().toDouble();
    double champ2 = ui->lineEdit_champ2_4->text().toDouble();

    // Perform subtraction
    double result = champ1 - champ2;

    // Display the result in lineEdit_resultat
    ui->lineEdit_resultat_4->setText(QString::number(result));
}
void MainWindow::on_pushButton_multiplication_4_clicked()
{
    // Get values from lineEdit_champ1 and lineEdit_champ2
    double champ1 = ui->lineEdit_champ1_4->text().toDouble();
    double champ2 = ui->lineEdit_champ2_4->text().toDouble();

    // Perform multiplication
    double result = champ1 * champ2;

    // Display the result in lineEdit_resultat
    ui->lineEdit_resultat_4->setText(QString::number(result));
}
void MainWindow::on_pushButton_diviser_4_clicked()
{
    // Get values from lineEdit_champ1 and lineEdit_champ2
    double champ1 = ui->lineEdit_champ1_4->text().toDouble();
    double champ2 = ui->lineEdit_champ2_4->text().toDouble();

    // Check for division by zero
    if (champ2 == 0) {
        QMessageBox::warning(this, "Error", "Cannot divide by zero.");
        return;
    }

    // Perform division
    double result = champ1 / champ2;

    // Display the result in lineEdit_resultat
    ui->lineEdit_resultat_4->setText(QString::number(result));
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
    // Récupérer les valeurs mises à jour des champs
    QString cin = ui->cinLineEdit->text();
    QString nom = ui->nomLineEdit->text();
    QString prenom = ui->prenomLineEdit->text();
    QDate dateNaissance = ui->dateNaissanceLineEdit->date();
    QString email = ui->emailLineEdit->text();
    QString telephone = ui->telephoneLineEdit->text();

    // Vérifier si les champs sont remplis
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Préparer une requête SQL pour la modification
    QSqlQuery query;
    query.prepare("UPDATE client SET NOM = :nom, PRENOM = :prenom, DATENAISSANCE = :dateNaissance, "
                  "EMAIL = :email, TELEPHONE = :telephone WHERE CIN = :cin");

    // Lier les valeurs
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":cin", cin);

    // Exécuter la requête
    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Les informations du client ont été modifiées avec succès.");
        // Actualiser la TableView ou l'affichage
        afficherClients();
        // Réinitialiser le formulaire
        resetForm();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification : " + query.lastError().text());
    }
}

// Fonction pour réinitialiser le formulaire après modification
void MainWindow::resetForm() {
    ui->cinLineEdit->setEnabled(true);
    ui->cinLineEdit->clear();
    ui->nomLineEdit->clear();
    ui->prenomLineEdit->clear();
    ui->dateNaissanceLineEdit->setDate(QDate::currentDate());
    ui->emailLineEdit->clear();
    ui->telephoneLineEdit->clear();
    ui->modifierButton->setText("Modifier");
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
    ui->tableView_3->setModel(model);

    // Customize the table view (optional)
    ui->tableView_3->resizeColumnsToContents();
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_3->setSelectionMode(QAbstractItemView::SingleSelection);

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



void MainWindow::exportCsvButton_2()
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
    ui->tableView_3->setModel(model);

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
/*void MainWindow::readSerialData()
{
    QByteArray data = serial->readAll();
    QStringList parts = QString(data).split(",");

    if (parts.size() == 2) {
        QString cin = parts[0].trimmed();
        QString type = parts[1].trimmed();

        QSqlQuery query;
        query.prepare("INSERT INTO horaires (cin, type) VALUES (:cin, :type)");
        query.bindValue(":cin", cin);
        query.bindValue(":type", type);

        if (!query.exec()) {
            qDebug() << "Erreur lors de l'insertion des données : " << query.lastError().text();
        } else {
            qDebug() << "Données insérées : CIN =" << cin << ", Type =" << type;
        }
    }
}
*/
//statistiqueeee
void MainWindow::on_pushButton_38_clicked()
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

//reopen

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
    // Créer une instance de QSqlQuery pour exécuter la requête SQL
    QSqlQuery query;

    // Préparer la requête pour obtenir le mot de passe depuis la base de données avec le CIN donné
    query.prepare("SELECT mot_de_passe FROM employe WHERE cin = :cin");
    query.bindValue(":cin", cin);  // Lier la valeur du CIN à la requête

    // Exécuter la requête
    if (query.exec()) {
        // Si la requête s'est bien exécutée, vérifier si un mot de passe est trouvé
        if (query.next()) {
            // Récupérer le mot de passe stocké dans la base de données
            QString dbPassword = query.value(0).toString();

            // Vérifier si le mot de passe entré par l'utilisateur correspond à celui stocké dans la base
            if (dbPassword == mot_de_passe) {
                // Si les mots de passe correspondent, retourner true (authentification réussie)
                return true;
            } else {
                // Si les mots de passe ne correspondent pas, afficher un message d'avertissement
                QMessageBox::warning(this, "Erreur", "Mot de passe incorrect.");
                return false;
            }
        } else {
            // Si aucun CIN n'est trouvé, afficher un message d'erreur
            QMessageBox::warning(this, "Erreur", "CIN introuvable.");
            return false;
        }
    } else {
        // Si la requête échoue, afficher un message d'erreur
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
void MainWindow::on_pushButton_login_2_clicked()
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
void MainWindow::on_pushButton_40_clicked()
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

    // Récupérer les données de la table performance
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

void MainWindow::on_comboBox_trier_currentIndexChanged(int index)
{
    QString selectedCriteria = ui->comboBox_5->currentText();  // Récupérer le critère sélectionné

    QString queryStr;

    if (selectedCriteria == "Salaire") {
        queryStr = "SELECT * FROM employe ORDER BY salaire ASC";  // Tri par salaire croissant
    } else if (selectedCriteria == "Date d'embauche") {
        queryStr = "SELECT * FROM employe ORDER BY date_embauche ASC";  // Tri par date d'embauche croissant
    } else if (selectedCriteria == "Poste") {
        queryStr = "SELECT * FROM employe ORDER BY poste ASC";  // Tri par poste croissant
    } else {
        QMessageBox::warning(this, "Erreur", "Critère de tri invalide.");
        return;
    }

    QSqlQuery query;
    query.prepare(queryStr);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur de requête", "Une erreur est survenue lors de l'exécution de la requête.");
        return;
    }

    // Créer un modèle pour afficher les résultats
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(query);

    // Vérifier si des résultats ont été trouvés
    if (model->rowCount() > 0) {
        ui->tableView_4->setModel(model);  // Afficher les résultats dans le QTableView
        ui->tableView_4->resizeColumnsToContents();  // Ajuster la taille des colonnes
    } else {
        QMessageBox::information(this, "Aucun résultat", "Aucun employé trouvé pour ce critère.");
    }
}

// Function to handle searching for an employee by CIN
// Dans le .cpp
void MainWindow::on_pushButton_39_clicked()
{
    QString searchValue = ui->lineEdit_23->text().trimmed();  // Récupérer le CIN

    // Vérifier si CIN est vide
    if (searchValue.isEmpty()) {
        QMessageBox::warning(this, "Entrée manquante", "Veuillez entrer un CIN.");
        return;
    }

    // Vérifier que le critère de recherche est CIN
    if (ui->comboBox_4->currentIndex() != 0) {
        QMessageBox::warning(this, "Critère de recherche", "Veuillez sélectionner 'Recherche par CIN' dans le combo box.");
        return;
    }

    // Préparer la requête
    QString queryStr = QString("SELECT * FROM employe WHERE cin = '%1'").arg(searchValue);

    // Créer et exécuter la requête
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(queryStr);

    // Vérifier si la requête a échoué
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur SQL", model->lastError().text());
        return;
    }

    // Vérifier si des résultats ont été trouvés
    if (model->rowCount() > 0) {
        ui->tableView_4->setModel(model);  // Afficher les résultats dans le tableview
        ui->tableView_4->resizeColumnsToContents();  // Ajuster la taille des colonnes
    } else {
        QMessageBox::information(this, "Aucun résultat", "Aucun employé trouvé pour ce CIN.");
        delete model;
    }
}



void MainWindow::refreshTableViewrecherche() {
    // Récupérer toutes les données de la table employe et les afficher dans le QTableView
    QSqlQueryModel* model = employe.afficher();
    ui->tableView_4->setModel(model);  // Définir le modèle du QTableView
    ui->tableView_4->resizeColumnsToContents();  // Ajuster la taille des colonnes automatiquement
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
void MainWindow::on_pushButton_35_clicked() {
    int cin = ui->lineEdit_23->text().toInt();
    QString nom = ui->lineEdit_19->text();
    QString prenom = ui->lineEdit_20->text();
    QString question = ui->lineEdit_27->text();
    QString sexe = ui->lineEdit_24->text();
    QString poste = ui->comboBox_16->currentText();
    QDate date_embauche = ui->dateEdit_7->date();
    float salaire = ui->lineEdit_22->text().toFloat();
    QString mot_de_passe = ui->lineEdit_21->text();

    // Validate inputs
    if (!validateInputs(cin, nom, prenom, question, sexe, poste, salaire, mot_de_passe)) {
        return;
    }

    // Set values to the employee object
    employe.setCin(cin);
    employe.setNom(nom);
    employe.setPrenom(prenom);
    employe.setQuestion(question);
    employe.setSexe(sexe);
    employe.setPoste(poste);
    employe.setDate_embauche(date_embauche);
    employe.setSalaire(salaire);
    employe.setMot_de_passe(mot_de_passe);

    // Try to add employee
    if (employe.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès.");

        // Refresh the model in the table view
        QSqlQueryModel* model = employe.afficher();
        ui->tableView_4->setModel(model);

        // Optionally, clear the line edits after successful addition
        ui->lineEdit_23->clear();
        ui->lineEdit_19->clear();
        ui->lineEdit_20->clear();
        ui->lineEdit_27->clear();
        ui->lineEdit_24->clear();
        ui->lineEdit_22->clear();
        ui->lineEdit_21->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé.");
    }
}


// Modify employee function
void MainWindow::on_pushButton_36_clicked() {
    int cin = ui->lineEdit_23->text().toInt();
    QString nom = ui->lineEdit_19->text();
    QString prenom = ui->lineEdit_20->text();
    QString question = ui->lineEdit_27->text();
    QString sexe = ui->lineEdit_24->text();
    QString poste = ui->comboBox_16->currentText();
    QDate date_embauche = ui->dateEdit_7->date();
    float salaire = ui->lineEdit_22->text().toFloat();
    QString mot_de_passe = ui->lineEdit_21->text();

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
void MainWindow::on_pushButton_37_clicked() {
    int id = ui->lineEdit_23->text().toInt();

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
void MainWindow::on_pushButton_ajouter_clicked() {
    // Récupérer les informations saisies par l'utilisateur
    int id_service = ui->lineEdit_id->text().toInt();  // ID du service
    QString service_demande = ui->comboBox_17->currentText();  // Service demandé (combobox)
    int numero = ui->lineEdit_numero->text().toInt();  // Numéro du service
    QDate datee = ui->dateEdit_date->date();  // Date du service (date picker)
    float tarif = ui->lineEdit_tarif->text().toFloat();  // Tarif du service

    // Créer l'objet Service sans le champ 'nom'
    Service newService(id_service, service_demande, numero, datee, tarif);

    // Tenter d'ajouter le service à la base de données
    if (newService.ajouter()) {
        // Si l'ajout est réussi
        QMessageBox::information(this, "Succès", "Service ajouté avec succès.");

        // Réinitialiser les champs du formulaire
        ui->lineEdit_id->clear();  // Effacer le champ ID
        ui->lineEdit_numero->clear();  // Effacer le champ numéro
        ui->lineEdit_tarif->clear();  // Effacer le champ tarif
        ui->dateEdit_date->setDate(QDate::currentDate());  // Remettre la date à aujourd'hui
        ui->comboBox_17->setCurrentIndex(0);  // Remettre la sélection du combobox à la première valeur

    } else {
        // Si l'ajout échoue
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du service.");
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
            refreshTableViewser();

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
    // Créer un objet QSqlQuery pour exécuter la requête SQL
    QSqlQuery query;

    // Préparer la requête SQL pour récupérer les données de la table "services"
    query.prepare("SELECT id_service, service_demande, numero, datee, tarif FROM services");

    // Exécuter la requête
    if (!query.exec()) {
        // Si l'exécution échoue, afficher un message d'erreur
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
        return;
    }

    // Créer un modèle QSqlQueryModel pour afficher les résultats
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    // Définir les en-têtes pour chaque colonne du modèle
    model->setHeaderData(0, Qt::Horizontal, "ID Service");
    model->setHeaderData(1, Qt::Horizontal, "Service Demand");
    model->setHeaderData(2, Qt::Horizontal, "Number");
    model->setHeaderData(3, Qt::Horizontal, "Date");
    model->setHeaderData(4, Qt::Horizontal, "Tariff");

    // Appliquer le modèle à la tableView
    ui->tableView_5->setModel(model);
    ui->tableView_5->resizeColumnsToContents();  // Ajuster la taille des colonnes
}


void MainWindow::refreshTableViewser() {
    // Créer un objet QSqlQuery pour exécuter la requête SQL
    QSqlQuery query;
    query.prepare("SELECT id_service, service_demande, numero, datee, tarif FROM services");

    // Exécuter la requête
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
        return;
    }

    // Créer un modèle QSqlQueryModel pour afficher les résultats
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    // Définir les en-têtes pour chaque colonne du modèle
    model->setHeaderData(0, Qt::Horizontal, "ID Service");
    model->setHeaderData(1, Qt::Horizontal, "Service Demand");
    model->setHeaderData(2, Qt::Horizontal, "Number");
    model->setHeaderData(3, Qt::Horizontal, "Date");
    model->setHeaderData(4, Qt::Horizontal, "Tariff");
    // Appliquer le modèle à la tableView
    ui->tableView_5->setModel(model);
    ui->tableView_5->resizeColumnsToContents();  // Ajuster la taille des colonnes
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
    QString selectedOption = ui->comboBox_20->currentText();

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
    ui->tableView_6->setModel(model);
    ui->tableView_6->resizeColumnsToContents();  // Ajuster la taille des colonnes

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
    QString searchType = ui->comboBox_21->currentText();

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
    ui->tableView_6->setModel(model);
    ui->tableView_6->resizeColumnsToContents();  // Ajuster la taille des colonnes

    // Optionnel : vous pouvez définir les en-têtes de colonnes si nécessaire
    model->setHeaderData(0, Qt::Horizontal, "ID Service");
    model->setHeaderData(1, Qt::Horizontal, "Service Demandé");
    model->setHeaderData(2, Qt::Horizontal, "Numéro");
    model->setHeaderData(3, Qt::Horizontal, "Date");
    model->setHeaderData(4, Qt::Horizontal, "Tarif");
}

void MainWindow::on_pushButton_34_clicked()
{
    QString numero = ui->lineEdit_25->text();
    QString message = ui->lineEdit_26->text();

    if (numero.isEmpty() || message.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    // Ensure smsManager is properly initialized
    if (smsManager != nullptr) {
        smsManager->sendSMS(numero, message);
    } else {
        QMessageBox::warning(this, "Erreur", "Le gestionnaire SMS n'est pas initialisé.");
    }
}














//gestion Reservation
void MainWindow::on_pushButton_ajouterRes_clicked()
{
    QString montantText = ui->lineEdit_Montant_4->text();
    QDate dateArrivee = ui->dateEdit_arrive_3->date();
    QDate dateDepart = ui->dateEdit_depart_3->date();
    QString type = ui->comboBox_type_3->currentText();
    QString statutReservation = ui->comboBox_status_3->currentText();
    QString cinText = ui->lineEdit_cin_3->text();

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
        ui->tableView_reservation_2->setModel(this->reservation.afficher());
        ui->tableView_reservation_2->resizeColumnsToContents();

        // Clear the fields
        ui->lineEdit_Montant_4->clear();
        ui->lineEdit_cin_3->clear();
        ui->dateEdit_arrive_3->setDate(QDate::currentDate());
        ui->dateEdit_depart_3->setDate(QDate::currentDate().addDays(1));
        ui->comboBox_type_3->setCurrentIndex(0);
        ui->comboBox_status_3->setCurrentIndex(0);

        updateCalendrier();
    }
}


void MainWindow::on_pushButton_20_clicked()
{
    int numReservation = ui->lineEdit_Supprimer_2->text().toInt();

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
        ui->tableView_reservation_2->setModel(this->reservation.afficher());
        ui->tableView_reservation_2->resizeColumnsToContents();
        updateCalendrier();
        ui->lineEdit_Supprimer_2->clear();

    } else {
        QMessageBox::critical(this, "Erreur de suppression", "Échec de la suppression de la réservation.");
    }
}


void MainWindow::on_pushButton_rechercherRes_clicked()
{
    QString idText = ui->lineEdit_recherche_3->text();

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

    ui->lineEdit_Montant_5->setText(QString::number(reservation.getMontant()));
    ui->comboBox_type_4->setCurrentText(reservation.getType());
    ui->comboBox_status_4->setCurrentText(reservation.getStatutReservation());
    ui->dateEdit_arrive_4->setDate(reservation.getDateArrivee());
    ui->dateEdit_depart_4->setDate(reservation.getDateDepart());
    ui->lineEdit_cin_4->setText(QString::number(reservation.getCin()));
}

void MainWindow::on_pushButton_modifierRes_clicked()
{
    QString idText = ui->lineEdit_recherche_3->text();

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

    QString montantText = ui->lineEdit_Montant_5->text();
    QDate dateArrivee = ui->dateEdit_arrive_4->date();
    QDate dateDepart = ui->dateEdit_depart_4->date();
    QString type = ui->comboBox_type_4->currentText();
    QString statutReservation = ui->comboBox_status_4->currentText();
    QString cinText = ui->lineEdit_cin_4->text();

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
        ui->tableView_reservation_2->setModel(this->reservation.afficher());
        ui->tableView_reservation_2->resizeColumnsToContents();

        ui->lineEdit_recherche->clear();
        ui->lineEdit_Montant_5->clear();
        ui->lineEdit_cin_4->clear();
        ui->dateEdit_arrive_4->setDate(QDate::currentDate());
        ui->dateEdit_depart_4->setDate(QDate::currentDate().addDays(1));
        ui->comboBox_type_4->setCurrentIndex(0);
        ui->comboBox_status_4->setCurrentIndex(0);
        updateCalendrier();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de la réservation. Vérifiez si le CIN existe dans la table clients.");
    }
}




void MainWindow::on_pushButton_pdfRes_clicked()
{
    // Get the reservation number from lineEdit_goto_confirmation
    QString numReservationText = ui->lineEdit_goto_confirmation_2->text();
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




void MainWindow::on_pushButton_recherchers_clicked()
{
    QString donnee = ui->lineEdit_rechercher_2->text();

    ui->tableView_reservation_2->setModel(this->reservation.rechercher(donnee));
    ui->tableView_reservation_2->resizeColumnsToContents();
}



void MainWindow::onTabChanged(int index)
{
    // Check if the currently selected tab is tab_4
    if (ui->tabWidget_2->widget(index) == ui->tab_29) {
        updateChart();
    }
}

void MainWindow::updateChart()
{
    // Clear any existing chart to avoid duplicates
    if (ui->verticalLayout_stat_2->count() > 0) {
        QLayoutItem *item = ui->verticalLayout_stat_2->takeAt(0);
        delete item->widget();
        delete item;
    }

    QString selectedStat = ui->comboBox_stat_2->currentText();
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
    ui->verticalLayout_stat_2->addWidget(chartView);
}

void MainWindow::on_pushButton_goto_confirmation_clicked()
{
    QString numReservationText = ui->lineEdit_goto_confirmation_2->text();

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
           ui->lineEdit_dateArrive_3->setText(query.value("DATE_ARRIVEE").toString());
           ui->lineEdit_dateDepart_3->setText(query.value("DATE_DEPART").toString());
           ui->lineEdit_Type_3->setText(query.value("TYPE").toString());
           ui->lineEdit_Status_3->setText(query.value("STATUT_RESERVATION").toString());
           ui->lineEdit_Montant_6->setText(query.value("MONTANT").toString());
           ui->lineEdit_Cin_3->setText(query.value("CIN").toString());

           ui->tabWidget_2->setCurrentWidget(ui->tab_30);

       } else {
           // Display an error message if the reservation is not found
           QMessageBox::warning(this, "Not Found", "No reservation found with the specified number.");
       }
}

#include <QTextCharFormat>

void MainWindow::updateCalendrier() {
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
                    ui->calendarWidget_2->setDateTextFormat(date, defaultFormat);
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
        ui->calendarWidget_2->setDateTextFormat(dateArrivee, format);
    }
}




void MainWindow::onDateSelected(const QDate &date)
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



/*void MainWindow::readReservationData(int id)
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

/*void MainWindow::on_pushButton_vocal_clicked()
{
    // Show a message box to indicate the function is running
    QMessageBox::information(this, "Function Triggered", "The function on_pushButton_vocal_clicked has been called.");

    int id = ui->lineEdit_vocal->text().toInt();
    readReservationData(id);
}*/


void MainWindow::TriChanged()
{
    QString critere = ui->comboBox_tri_2->currentText();

    ui->tableView_reservation_2->setModel(this->reservation.afficherAvecTri(critere));
    ui->tableView_reservation_2->resizeColumnsToContents();
}


void MainWindow::on_pushButton_vocal_clicked()
{

}


void MainWindow::on_pushButton_readArduino_clicked()
{
    // Récupérer l'ID client depuis le champ de saisie
        QString clientId = ui->lineEdit_IDCLIENT_2->text();

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
        ui->textBrowser_2->setPlainText(sortieLogs);
}

void MainWindow::readArduinoContinuously()
{
    timenow = QDateTime::currentDateTime();

    if (!A.is_connected) {
        return; // Stop processing if Arduino is not connected
    }

    QString textrecu = "aucun Mouvement";
    QByteArray text = A.read_from_arduino();
    textrecu = QString(text).trimmed();

    if ((textrecu == "D:1" || textrecu == "D:2") && derniereDetectMouvement.msecsTo(timenow) > 5000) {
        derniereDetectMouvement = QDateTime::currentDateTime();

        // Extract ID from the received text (e.g., "D:1" -> 1)
        int id_det = textrecu.mid(2).toInt();

        // Query the database to get the chamber's status
        QSqlQuery query;
        query.prepare("SELECT STATUT_CHAMBRE FROM CHAMBRES WHERE ID_DET = :id_det");
        query.bindValue(":id_det", id_det);

        if (query.exec() && query.next()) {
            QString statut = query.value(0).toString();

            // Check if the chamber status is 'libre' (available)
            if (statut == "libre") {
                // Create an alert pop-up
                QMessageBox::warning(
                    this,
                    "Alerte de Mouvement",
                    QString("Attention! Mouvement détecté dans la chambre %1, mais elle est actuellement libre!").arg(id_det)
                );
            } else {
                // If the chamber is not 'libre', show a normal info message
                QMessageBox::information(
                    this,
                    "Detection De Mouvement",
                    QString("Il y a mouvement dans la chambre %1. Statut: %2").arg(id_det).arg(statut)
                );
            }
        }
        else {
            QMessageBox::warning(
                this,
                "Erreur",
                QString("Impossible de récupérer le statut de la chambre pour ID_DET: %1").arg(id_det)
            );
        }
    }

    qDebug() << textrecu;
}

void MainWindow::showMotionMessage(const QString &room)
{
    QMessageBox::warning(this, "Detection De Mouvement", "il ya mouvement dans la  :  " + room + ".");
}


bool MainWindow::ajouterHistorique(int clientId, QString &evenement, QString &details)
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



void MainWindow::on_pushButton_readArduino_2_clicked()
{
    // Récupérer l'ID client depuis le champ de saisie
        QString clientId = ui->lineEdit_IDCLIENT_2->text();

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
        ui->textBrowser_2->setPlainText(sortieLogs);
}

void MainWindow::on_pushButton_recherchers_2_clicked()
{
    QString donnee = ui->lineEdit_rechercher_2->text();

    ui->tableView_reservation_2->setModel(this->reservation.rechercher(donnee));
    ui->tableView_reservation_2->resizeColumnsToContents();
}
