#include "arduino.h"
#include <QDebug>
#include <QThread>
#include <QRegularExpression>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

Arduino::Arduino(QObject *parent) : QObject(parent), serialPort(new QSerialPort(this)), connected(false)
{
    serial = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, this, &Arduino::readData);
    connect(serialPort, &QSerialPort::readyRead, this, &Arduino::readData2);

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

int Arduino::connect_arduino(const QString &portName)
{
    if (serial->isOpen()) {
        serial->close();
        serial->setBaudRate(QSerialPort::Baud9600);
        QThread::msleep(1000);
    }

    arduino_port_name = portName;
    serial->setPortName(arduino_port_name);

    if (serial->open(QSerialPort::ReadWrite)) {
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        QThread::msleep(1000);

        return 0;
    } else {
        qDebug() << "Failed to open serial port:" << serial->errorString();
        return 1;
    }
}


int Arduino::close_arduino()

{

    if(serial->isOpen()){
            serial->close();
            return 0;
        }
    return 1;


}

QByteArray Arduino::read_from_arduino()
{
    if (serial->isOpen() && serial->bytesAvailable() > 0) {
        return serial->readAll(); // Return immediately available data
    }
    return QByteArray(); // Return an empty array if no data
}


int Arduino::write_to_arduino( QByteArray d)

{

    if(serial->isWritable()){
        serial->write(d);
        return 1;// envoyer des donnés vers Arduino
    }else{
        qDebug() << "Couldn't write to serial!";
        return 0;
    }


}
QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *Arduino::getserial()
{
   return serial;
}
#include <QSerialPortInfo>

bool Arduino::openConnection2(int baudRate)
{
    if (connected) {
        qDebug() << "Déjà connecté à un port série.";
        return true;
    }

    QString forcedPortName = "COM8";  // Force the port to COM8

    qDebug() << "Tentative de connexion forcée sur le port" << forcedPortName;

    serialPort->setPortName(forcedPortName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        connected = true;
        qDebug() << "Connexion série établie sur" << forcedPortName;
        return true;
    } else {
        qDebug() << "Échec de connexion sur" << forcedPortName;
    }

    return false;
}
void Arduino::closeConnection2()
{
    if (serialPort->isOpen()) {
        serialPort->close();
        connected = false;
        qDebug() << "Connexion série fermée.";
    }
}

bool Arduino::isConnected2() const
{
    return connected;
}

void Arduino::readData2() {
    // Lit toutes les données disponibles sur le port série.
    //Stocke les données lues.
    QByteArray data = serialPort->readAll();
    QString message(data);

    //Supprime les espaces au début et à la fin de la chaîne,
    // Nettoyer la chaîne pour enlever les caractères inutiles
    message = message.trimmed();
    message.replace("\r", "").replace("\n", "");  // Supprime les retours chariot et les nouvelles lignes

    // Vérifiez si le message contient l'UID complet
    QRegularExpression uidRegex("UID de la carte :\\s*([0-9A-F]{8})");
    QRegularExpressionMatch match = uidRegex.match(message);

    if (match.hasMatch()) {
        QString formattedUid = match.captured(1).toUpper();  // Extraire l'UID et le formater en majuscules
        qDebug() << "UID reçu de l'Arduino : " << formattedUid;
        emit clientFound(formattedUid);
    } else {
        // Affiche un message de débogage et tente de trouver une partie valide de l'UID
        qDebug() << "UID non trouvé dans la chaîne reçue : " << message;
        // Vous pouvez également essayer d'extraire une partie de l'UID s'il y a des fragments
        if (message.contains("UID de la carte :")) {
            QString partialUid = message.split("UID de la carte :").last();
            if (partialUid.length() >= 8) {
                QString extractedUid = partialUid.left(8).toUpper();
                qDebug() << "UID partiel extrait : " << extractedUid;
                emit clientFound(extractedUid);
            }

        }
    }
}

void Arduino::writeData2(const QByteArray &data)
{
    if (serialPort->isOpen() && isConnected2()) {
        serialPort->write(data);
        serialPort->flush();
    } else {
        qDebug() << "Le port série n'est pas ouvert ou connecté.";
    }
}
//feriel et bondka
bool Arduino::connect_Arduino22()
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
void Arduino::disconnect_Arduino22()
{
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Déconnexion de l'Arduino.";
    }
}
void Arduino::read_Data22()
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


