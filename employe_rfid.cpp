/*#include "employe_rfid.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


EmployeRFID::EmployeRFID(QObject *parent) : QObject(parent)
{
}

// Vérifie la carte RFID dans la base de données
bool EmployeRFID::verifierCarte(const QString &codeCarte, int &cin, QString &nom, QString &prenom)
{
    QSqlQuery query;
    query.prepare("SELECT cin, nom, prenom FROM employe WHERE code_carte = :codeCarte");
    query.bindValue(":codeCarte", codeCarte);

    if (query.exec() && query.next()) {
        cin = query.value(0).toInt();
        nom = query.value(1).toString();
        prenom = query.value(2).toString();
        return true;
    }

    return false;
}

// Traiter l'événement RFID
void EmployeRFID::traiterRFID(const QString &rfid, int cin)
{
    qDebug() << "Carte RFID lue: " << rfid << " CIN: " << cin;

    // Requête SQL pour retrouver l'employé à partir du numéro RFID et CIN
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM FROM EMPLOYE WHERE RFID = :rfid AND CIN = :cin");
    query.bindValue(":rfid", rfid);
    query.bindValue(":cin", cin);

    if (query.exec()) {
        if (query.next()) {
            QString nom = query.value(0).toString();
            QString prenom = query.value(1).toString();
            emit afficherMessage("Employé trouvé: " + nom + " " + prenom);
        } else {
            emit afficherMessage("Aucun employé trouvé pour ce RFID et CIN.");
        }
    } else {
        emit afficherMessage("Erreur de base de données : " + query.lastError().text());
    }
}

bool EmployeRFID::associerCarteRFID(int cin, const QString &rfid)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Employe_Carte_RFID (CIN, RFID_ID) VALUES (:cin, :rfid)");
    query.bindValue(":cin", cin);
    query.bindValue(":rfid", rfid);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur lors de l'insertion dans la base de données: " << query.lastError();
        return false;
    }
}

bool EmployeRFID::verifierCarteAssociee(int cin)
{
    QSqlQuery query;
    query.prepare("SELECT RFID_ID FROM Employe_Carte_RFID WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec()) {
        if (query.next()) {
            QString rfid = query.value(0).toString();
            qDebug() << "Carte RFID associée: " << rfid;
            return true;
        }
    }

    return false;
}

// Enregistre une opération dans la table HORAIRES
void EmployeRFID::enregistrerHoraire(int cin, const QString &type, const QDateTime &timestamp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO horaires (id, cin, type, timestamp) VALUES (horaires_seq.NEXTVAL, :cin, :type, :timestamp)");
    query.bindValue(":cin", cin);
    query.bindValue(":type", type);
    query.bindValue(":timestamp", timestamp.toString("yyyy-MM-dd HH:mm:ss"));

    if (!query.exec()) {
        QString errorMsg = QString("Erreur lors de l'enregistrement de l'horaire : %1").arg(query.lastError().text());
        emit afficherMessage(errorMsg);
    }
}
*/
