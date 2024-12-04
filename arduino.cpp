#include "arduino.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Arduino::Arduino(QObject *parent)
    : QObject(parent), serialPort(new QSerialPort(this)), connected(false)
{
    connect(serialPort, &QSerialPort::readyRead, this, &Arduino::readData);
}

Arduino::~Arduino()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

bool Arduino::openConnection(const QString &portName, int baudRate)
{
    if (connected) {
        qDebug() << "Déjà connecté à un port série.";
        return true;
    }

    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        connected = true;
        qDebug() << "Connexion série établie sur" << portName;
        return true;
    } else {
        qDebug() << "Échec de la connexion série sur" << portName;
        return false;
    }
}

void Arduino::closeConnection()
{
    if (serialPort->isOpen()) {
        serialPort->close();
        connected = false;
        qDebug() << "Connexion série fermée.";
    }
}

bool Arduino::isConnected() const
{
    return connected;
}

void Arduino::readData()
{
    QByteArray data = serialPort->readAll();
    QString message(data);

    message = message.trimmed();
    message.replace("\r", "").replace("\n", "");

    QRegularExpression uidRegex("UID de la carte :\\s*([0-9A-F]{8})");
    QRegularExpressionMatch match = uidRegex.match(message);

    if (match.hasMatch()) {
        QString formattedUid = match.captured(1).toUpper();
        emit clientFound(formattedUid);
    } else {
        qDebug() << "UID non trouvé dans la chaîne reçue : " << message;
        if (message.contains("UID de la carte :")) {
            QString partialUid = message.split("UID de la carte :").last();
            if (partialUid.length() >= 8) {
                QString extractedUid = partialUid.left(8).toUpper();
                emit clientFound(extractedUid);
            }
        }
    }
}

void Arduino::writeData(const QByteArray &data)
{
    if (serialPort->isOpen() && isConnected()) {
        serialPort->write(data);
        serialPort->flush();
    } else {
        qDebug() << "Le port série n'est pas ouvert ou connecté.";
    }
}
