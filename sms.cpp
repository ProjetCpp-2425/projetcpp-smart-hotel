#include "sms.h"
#include <QMessageBox>
#include <QDebug>
#include <QUrlQuery>


QString twilio_phone_number = "+17755359143";


SmsManager::SmsManager(QObject *parent) : QObject(parent) {

}

void SmsManager::onReplyFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "SMS envoyé avec succès";
    } else {
        qDebug() << "Erreur lors de l'envoi du SMS:" << reply->errorString();
    }


    reply->deleteLater();
}


void SmsManager::sendSMS(const QString &to, const QString &message) {
    // Vérifier si le numéro de destination et le message sont vides
    if (to.isEmpty() || message.isEmpty()) {
        qDebug() << "Le numéro de destination ou le message est vide.";
        QMessageBox::warning(nullptr, "Erreur", "Le numéro ou le message est vide.");
        return;
    }

    // Création du gestionnaire de requêtes réseau
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // L'URL de l'API Twilio
    QUrl url("https://api.twilio.com/2010-04-01/Accounts/AC956f9e29ad437a8117b12f33da2d93e5/Messages.json");
    QNetworkRequest request(url);

    // Remplacer ici par vos informations personnelles
    QString credentials = "AC956f9e29ad437a8117b12f33da2d93e5:c7c00354535a870aa95760a945862bec";  // SID et Auth Token
    QByteArray base64Credentials = credentials.toUtf8().toBase64();
    request.setRawHeader("Authorization", "Basic " + base64Credentials);  // Authentification en Base64

    // Paramètres du message (Numéro d'expéditeur, destinataire, message)
    QUrlQuery params;
    params.addQueryItem("From", "+17755359143");  // Numéro Twilio (celui de l'image)
    params.addQueryItem("To", to);  // Numéro du destinataire
    params.addQueryItem("Body", message);  // Contenu du message

    // Ajouter un header pour le type de contenu
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Envoi de la requête POST
    QNetworkReply *reply = manager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());

    // Traitement de la réponse
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "SMS envoyé avec succès :" << reply->readAll();
            QMessageBox::information(nullptr, "Succès", "Message envoyé avec succès.");
        } else {
            qDebug() << "Échec de l'envoi du SMS : " << reply->errorString();
            QMessageBox::critical(nullptr, "Erreur", "Échec de l'envoi du message : " + reply->errorString());
        }

        // Nettoyage
        reply->deleteLater();
        manager->deleteLater();
    });
}
