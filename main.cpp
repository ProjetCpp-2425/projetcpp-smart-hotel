#include <QApplication>
#include "mainwindow.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include "connection.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QCoreApplication>
#include <QDebug>
#include "arduino.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 1. Connexion à la base de données
    Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Failed to connect to the database. Exiting application.";
        return -1;  // Exit if database connection fails
    }

    Arduino arduino;

            // Essayer de se connecter au port série
    if (arduino.openConnection("COM5", QSerialPort::Baud9600)) {
        qDebug() << "Arduino connecté avec succès!";
    } else {
        qDebug() << "Échec de la connexion à l'Arduino.";
    }
    // 2. Initialiser le port série avec COM5 explicitement
   /* QSerialPort serialPort;
    serialPort.setPortName("COM5");  // Assurez-vous que COM5 est le port correct
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort.open(QIODevice::ReadOnly)) {
        qDebug() << "Connexion série réussie avec COM5.";
        QObject::connect(&serialPort, &QSerialPort::readyRead, [&]() {
            QByteArray data = serialPort.readAll();
            qDebug() << "Message reçu :" << QString::fromUtf8(data);
        });
    } else {
        qDebug() << "Erreur : Impossible d'ouvrir le port COM5.";
        qDebug() << "Erreur spécifique :" << serialPort.errorString();
        return -1;  // Exit if serial port connection fails
    }*/

    // 3. Si la connexion à la base de données est réussie, afficher la fenêtre principale
    MainWindow w;
    w.show();

    return a.exec();  // Lancer la boucle d'événements
}
