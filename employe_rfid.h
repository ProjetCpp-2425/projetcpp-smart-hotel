/*#ifndef EMPLOYE_RFID_H
#define EMPLOYE_RFID_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QMessageBox>

class EmployeRFID : public QObject
{
    Q_OBJECT
public:
    explicit EmployeRFID(QObject *parent = nullptr);

    // Méthodes principales
    void traiterRFID(const QString &rfid, int cin); // Assurez-vous que les types sont corrects
    void enregistrerHoraire(int cin, const QString &type, const QDateTime &timestamp);

public slots:
        bool associerCarteRFID(int cin, const QString &rfid);
        bool verifierCarteAssociee(int cin);

private:
    bool verifierCarte(const QString &codeCarte, int &cin, QString &nom, QString &prenom);
    void afficherEmployeInfo(int cin, const QString &nom, const QString &prenom);

signals:
    void afficherMessage(const QString &message);
};

#endif // EMPLOYE_RFID_H
*/
