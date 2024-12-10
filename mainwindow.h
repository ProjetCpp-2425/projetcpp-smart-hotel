#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "client.h"
#include <QVector>
#include <QDate>
#include <QSqlQuery>
#include <QMessageBox>
#include <QImage>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRandomGenerator>
#include <QPainter>
#include <QtSerialPort/QSerialPort>
#include <QSqlDatabase>
#include "employe.h"
#include <QMap>
#include <QString>
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QDateEdit>
#include <QtCharts>
#include <QSystemTrayIcon>
#include "arduino.h"
#include "transaction.h"
#include "chambre.h"
#include <QTableView>
#include "service.h"  // Include the Service class header
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "sms.h"
#include "reservation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Transaction-related methods
    void calculerTVA();
    std::vector<Transaction> chargerTransactionsDepuisBase();
    void refreshTableView();
    void sendMailIfMaxDepense();
    QString getTelephone() const; // Méthode pour obtenir le numéro de téléphone
//service
    void on_changeStatutButton_clicked();
    void displayAsciiChart(const QList<Service> &services); // Example placeholder function
    void displayServices();
//gestion reservation
    void updateChart();
    void onTabChanged(int index);
    void updateCalendrier();
    void readReservationData(int id);
    bool ajouterHistorique(int clientId, QString &evenement, QString &details);


    // Room management
    void addNoteManually(QString roomId, QString note);

protected:
    void keyPressEvent(QKeyEvent *event);
signals:
    void motionDetected(const QString &room);
private slots:
     void navigateToDashboard();  // Slot pour naviguer vers le dashboard
    //logout
     void onLogoutClicked();
    //basculement
    void onLoginClicked();
    // Transaction-related slots
    void on_pushButton_14_clicked();
    void on_pushButton_16_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_44_clicked();
    void on_comboBox_18_selectionChanged();
    void generatePdfForPeriod(const QDate &startDate, const QDate &endDate);
    void on_pushButton_stat_clicked();

    void on_pushButton_calculerTVA_clicked();

    // Arduino
    void handleArduinoData(QString data);
    void readyRead();
    void connected();

    // Room management slots
    void on_pushButton_ajouter_3_clicked();
    void on_pushButton_modifier_3_clicked();
    void on_pushButton_supprimer_3_clicked();
    void refreshTableView2();

    void on_buttonTrier_6_clicked();
    void on_buttonRechercher_3_clicked();
    void on_pushButton_statistique_4_clicked();
    void on_pushButton_pdf_4_clicked();

    void on_pushButton_QR_4_clicked();
    void on_pushButton_list_4_clicked();
    void on_pushButton_print_4_clicked();
    void on_pushButton_plus_4_clicked();
    void on_pushButton_mois_4_clicked();
    void on_pushButton_multiplication_4_clicked();
    void on_pushButton_diviser_4_clicked();
    void on_save();

    // System tray
    void onTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
   void on_ajouterButton_clicked();
    void on_modifierButton_clicked();
    void resetForm();
    void on_supprimerButton_clicked();
    void afficherClients();

    void on_chercherButton_clicked();
    void exportCsvButton_2();
    void on_chatbutton_clicked();
    void on_afficherStatistiquesButton_clicked();    void enregistrerArrivee();              // Enregistrer l'arrivée d'un client
    void afficherHistorique();             // Afficher l'historique des visites



    void on_trier_clicked();


    void on_arriveeButton_clicked();


    void on_Obtenir_clicked();

    void on_Imprimer_clicked();

    void on_Obtenir_2_clicked();
    // Login-related slots
    void on_pushButton_login_clicked();          // Slot for the "Login" button
    void on_pushButton_login_2_clicked(); // Slot for the "Forgot Password" button

    // Employee management slots
    void on_pushButton_35_clicked();//ajouter
    void on_pushButton_36_clicked();//modifier
    void on_pushButton_37_clicked();//supression
   void on_pushButton_39_clicked();
   void refreshTableViewrecherche();
    void on_pushButton_40_clicked();//pdf
void on_pushButton_38_clicked();
  void on_comboBox_trier_currentIndexChanged(int index);


    void validateCaptcha();
    void regenerateCaptcha();
    //service
    void on_pushButton_ajouter_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_afficher_clicked();
    void refreshTableViewser();
    void on_pushButton_34_clicked();
    void on_pushButton_statistique_clicked();
    void on_pushButton_pdf_clicked();
    void on_pushButton_trier_clicked();
    void on_pushButton_recherche_clicked();
   // void readSerialData(); // Slot pour traiter les données du port série

//gestionreservation
    void on_pushButton_ajouterRes_clicked();
    void on_pushButton_20_clicked();

    void on_pushButton_rechercherRes_clicked();
    void on_pushButton_modifierRes_clicked();
    void on_pushButton_pdfRes_clicked();
    void on_pushButton_recherchers_clicked();
    void onDateSelected(const QDate &date);
    void TriChanged();
    void readArduinoContinuously();
    void showMotionMessage(const QString &room);
    void on_pushButton_goto_confirmation_clicked();
    //void on_pushButton_vocal_clicked();
    void on_pushButton_vocal_clicked();
    void on_pushButton_readArduino_clicked();



    void on_pushButton_readArduino_2_clicked();

    void on_pushButton_recherchers_2_clicked();

private:
    Ui::MainWindow *ui;
    //dashbord total room et room availble
  void updateChambreStats();

    // Transaction management
    Transaction T;
    QDateEdit *dateEdit_3;
    QSqlQueryModel *model;  // Model for QTableView
    double maxDepense = 1000.0;
//reservation
    Reservation reservation;
    Arduino A;
    QThread *arduinoThread;
    QDateTime timenow;
    QDateTime derniereDetectMouvement;
//service
    Service *service; // Pointer to Service class
    SmsManager *smsManager; // Declare the SmsManager pointer
    // Room management
    Chambre chambre;
    QTableView *tableView;

    // System tray
    QSystemTrayIcon *trayIcon;
    void setupTrayIcon();
//couleur du bouton
     void changerCouleurBouton(QPushButton* boutonClique);
    // Arduino
    Arduino *arduino;
      QSerialPort *serial;
    QVector<Client> clients;
    QMap<QString, int> historiqueClients; // Historique des visites (CIN -> nombre de visites)

    int trouverClient(const QString &cin);
    // Login validation and authentication
    bool validateLoginInputs(const QString &cin, const QString &mot_de_passe); // Validate login fields
   bool authenticate(const QString &cin, const QString &mot_de_passe);       // Authenticate user

    // Employee management validation
    bool validateInputs(int cin, const QString &nom, const QString &prenom,
                        const QString &question, const QString &sexe,
                        const QString &poste, float salaire,
                        const QString &mot_de_passe);

    int generatedCaptcha; // Stocker la chaîne CAPTCHA

//arduino feriel et bondka


    QString arduinoPortName;      // Nom du port série
       void setupArduino();          // Méthode d'initialisation Arduino
       void readFromArduino();       // Lecture des données Arduino
       void handleUID(const QString &UIDC);
       void handleData(const QByteArray &data);
       // Traitement des données Arduino (exemple : UID)
       // void checkClientByUID(const QString &UIDC);
       void onClientFound(const QString &RFID);



        // Objet pour gérer le port série
    QSqlDatabase db;           // Objet pour gérer la connexion à la base de données

    Employe employe;           // Instance of Employe class

    void initializeSerialPort(); // Initialiser le port série
    void initializeDatabase();   // Initialiser la connexion à Oracle
    void insertDataToDatabase(const QString &cin, const QString &type); // Insérer des données dans Oracle

};
#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>

class StatistiqueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatistiqueWidget(QWidget *parent = nullptr) : QWidget(parent) {}

    void setStatistics(int cleaningCount, int foodCount) {
        this->cleaningCount = cleaningCount;
        this->foodCount = foodCount;
        update(); // Redraw the widget
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int widgetWidth = width();
        int widgetHeight = height();

        // Calculate the maximum count for scaling purposes
        int maxCount = qMax(cleaningCount, foodCount);

        // Set up the colors for each category
        QBrush cleaningBrush(Qt::blue);
        QBrush foodBrush(Qt::green);

        // Draw Cleaning bar
        int cleaningBarWidth = (float)cleaningCount / maxCount * widgetWidth;
        painter.setBrush(cleaningBrush);
        painter.drawRect(50, widgetHeight / 3, cleaningBarWidth, 30);

        // Draw Food bar
        int foodBarWidth = (float)foodCount / maxCount * widgetWidth;
        painter.setBrush(foodBrush);
        painter.drawRect(50, widgetHeight / 1.5, foodBarWidth, 30);

        // Add text labels
        painter.setPen(QPen(Qt::black));
        painter.drawText(50, widgetHeight / 3 - 10, "Cleaning: " + QString::number(cleaningCount));
        painter.drawText(50, widgetHeight / 1.5 - 10, "Food: " + QString::number(foodCount));
    }

private:
    int cleaningCount = 0;
    int foodCount = 0;

};


#endif // MAINWINDOW_H
