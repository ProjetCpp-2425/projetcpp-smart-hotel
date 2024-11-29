#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}

bool Arduino::connectArduino()
{
    serial->setPortName("COM6");  // Changez selon le port utilisé par votre Arduino
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly)) {
        connect(serial, &QSerialPort::readyRead, this, &Arduino::readData);
        qDebug() << "Connexion à Arduino réussie.";
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
    }
}

void Arduino::readData()
{
    while (serial->canReadLine()) {  // Vérifie si une ligne complète est disponible
        QByteArray data = serial->readLine().trimmed();
        QString message = QString::fromUtf8(data);
        qDebug() << "Données brutes : " << message;  // Affiche les données dans la console
        emit dataReceived(message);  // Émet le signal avec les données
    }
}

