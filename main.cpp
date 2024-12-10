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


    // 3. Si la connexion à la base de données est réussie, afficher la fenêtre principale
    MainWindow w;
    w.show();

    return a.exec();  // Lancer la boucle d'événements
}
