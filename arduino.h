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
    int is_connected;
    //feriel et bondka
    bool connect_Arduino22();  // Pour établir la connexion
    void disconnect_Arduino22();  // Pour fermer la connexion
//gestionreservation
    int connect_arduino(const QString &portName);
    int close_arduino(); // permet de femer la connexion
    int write_to_arduino( QByteArray ); // envoyer des données vers arduino
    QByteArray read_from_arduino();  //recevoir des données de la carte Arduino
    QSerialPort* getserial();   // accesseur
    QString getarduino_port_name();
    bool openConnection2(int baudRate);
    void closeConnection2();  // Ferme la connexion série
    bool isConnected2() const;  // Vérifie si la connexion est active
    void writeData2(const QByteArray &data);  // Envoie des données au port série

signals:
    void dataReceived(QString);
    void clientFound(const QString &RFID);// Signal émis lorsqu'une donnée est reçue
    //feriel et bondka
 void dataReceived22(QString);
private:
    QSerialPort *serial;  // Objet de communication série
    static const quint16 arduino_uno_vendor_id=9025;
    static const quint16 arduino_uno_producy_id=67;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;  // contenant les données lues à partir d'Arduino
    QSerialPort *serialPort;
 bool connected;
private slots:
    void readData();  // Pour lire les données reçues
     void readData2();
     //feriel et bondka
     void read_Data22();  // Lit les données reçues du port série
};

#endif // ARDUINO_H
