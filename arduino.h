#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    bool connectArduino();  // Pour établir la connexion
    void disconnectArduino();  // Pour fermer la connexion

signals:
    void dataReceived(QString);  // Signal émis lorsqu'une donnée est reçue

private:
    QSerialPort *serial;  // Objet de communication série

private slots:
    void readData();  // Pour lire les données reçues
};

#endif // ARDUINO_H


