#include "arduino.h"
#include <QDebug>
#include <QMessageBox>

Arduino::Arduino(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}

bool Arduino::connectArduino()
{
    // Vérifiez que le port COM est correct et non utilisé
    qDebug() << "Tentative de connexion au port série...";

    serial->setPortName("COM4");  // Changez selon le port utilisé par votre Arduino
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly)) {
        qDebug() << "Connexion à Arduino réussie.";
        connect(serial, &QSerialPort::readyRead, this, &Arduino::readData);
        return true;
    } else {
        qDebug() << "Erreur de connexion à Arduino :" << serial->errorString();
        return false;
    }
}

void Arduino::disconnectArduino()
{
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Déconnexion de l'Arduino.";
    }
}
void Arduino::readData()
{
    while (serial->canReadLine()) {  // Vérifie si une ligne complète est disponible
        QByteArray data = serial->readLine().trimmed();
        QString message = QString::fromUtf8(data);

        // Créer une fenêtre pop-up pour afficher le message
        QMessageBox msgBox;
        msgBox.setWindowTitle("Données Reçues");
        msgBox.setText("Message reçu d'Arduino :\n" + message);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();

        // Émettre le signal pour d'autres traitements si nécessaire
        emit dataReceived(message);
    }
}
