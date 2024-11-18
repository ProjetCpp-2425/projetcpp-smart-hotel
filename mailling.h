#ifndef MAILLING_H
#define MAILLING_H

#include <QString>
#include <qDebug>
#include <QProcess>


class mailling
{
public:
    mailling();
    bool sendEmail(const QString &to, const QString &subject, const QString &body);

private:
    QString smtpServer = "smtp.gmail.com";
    int smtpPort = 465; // For SSL
    QString emailUser = "trabelsiasma20@gmail.com";
    QString emailPassword = "qrdn sxof mjje ezst";
};

#endif // MAILLING_H
