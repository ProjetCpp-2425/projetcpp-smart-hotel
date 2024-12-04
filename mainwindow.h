#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDate>
#include <QString>
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
#include <QTextBrowser>
#include <QList>
#include "employe.h"
#include "arduino.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Login-related slots
    void on_pushButton_login_clicked();          // Slot for the "Login" button
    void on_pushButton_forgotPassword_clicked(); // Slot for the "Forgot Password" button

    // Employee management slots
    void on_pushButton_ajouter_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_chercher_clicked();
    void on_pushButton_statistique_clicked();
    void on_comboBox_trier_currentIndexChanged(int index);  // Déclarez le slot ici
    void on_comboBox_recherche_currentIndexChanged(int index);
    void on_pushButton_pdf_clicked();




    void validateCaptcha();
    void regenerateCaptcha();
    void on_pushButton_27_clicked();

    void on_pushButton_envoyer_clicked();

    void readSerialData(); // Slot pour traiter les données du port série

    void afficherMessage(const QString &message);

   // void slotRecevoirMorceau(const QString &morceau);

    void traiterRFID(const QString &uid);

    void onClientFound(const QString &rfid);  // Slot pour recevoir l'UID et afficher les infos

    void slotRecevoirMorceau(const QByteArray &data);  // Slot to handle incoming data



private:
    // Login validation and authentication
    bool validateLoginInputs(const QString &cin, const QString &mot_de_passe); // Validate login fields
    bool authenticate(const QString &cin, const QString &mot_de_passe);       // Authenticate user

    // Employee management validation
    bool validateInputs(int cin, const QString &nom, const QString &prenom,
                        const QString &question, const QString &sexe,
                        const QString &poste, float salaire,
                        const QString &mot_de_passe);

    int generatedCaptcha; // Stocker la chaîne CAPTCHA

    int calculerJoursAbsence(const QDate &dateDebut, const QDate &dateFin); // Calcul des jours d'absence
    //void on_pushButton_pdf_clicked();

    QString concatenerMorceaux(const QList<QString> &morceaux);



    Ui::MainWindow *ui;       // User interface

    QSerialPort *serial;       // Objet pour gérer le port série
    QSqlDatabase db;           // Objet pour gérer la connexion à la base de données


    Employe employe;           // Instance of Employe class

    void initializeSerialPort(); // Initialiser le port série
    void initializeDatabase();   // Initialiser la connexion à Oracle
    void insertDataToDatabase(const QString &cin, const QString &type); // Insérer des données dans Oracle

    Arduino *arduino;  // Déclarez un objet Arduino pour gérer la communication
     QString arduinoPortName;      // Nom du port série
        void setupArduino();          // Méthode d'initialisation Arduino
        void readFromArduino();       // Lecture des données Arduino
        void handleUID(const QString &UIDC);
        void handleData(const QByteArray &data);
        // Traitement des données Arduino (exemple : UID)
        // void checkClientByUID(const QString &UIDC);

        // Liste pour stocker les morceaux de texte
            QList<QString> morceaux;

            // Si vous utilisez un QSerialPort pour la lecture de données RFID, déclarez-le ici
            QSerialPort *serialPort;

         void afficherInformations(const QString &cin, const QString &nom, const QString &prenom, const QString &timestamp);


};

#endif // MAINWINDOW_H
