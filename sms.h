#ifndef SMS_H
#define SMS_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class SmsManager : public QObject
{
    Q_OBJECT

public:
    explicit SmsManager(QObject *parent = nullptr);
    void sendSMS(const QString &number, const QString &message);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
};

#endif // SMS_H
