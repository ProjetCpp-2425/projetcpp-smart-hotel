#include "mailling.h"

mailling::mailling()
{

}

bool mailling::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    qDebug() << "Before sending email...";

    QString fullBody = "Subject: " + subject + "\n\n" + body;
    QStringList arguments;

    // Construct curl
    arguments << "--url" << "smtps://" + smtpServer + ":" + QString::number(smtpPort)
              << "--ssl-reqd" // Indique que SSL  est requis pour la connexion.
              << "--mail-from" << emailUser
              << "--mail-rcpt" << to
              << "--user" << emailUser + ":" + emailPassword
              << "--insecure" //Ignore les erreurs de certificat SSL
              << "-T" << "-" //Indique que le corps de l'email sera transmis via l'entrée standard (stdin)
              << "-v";
    // comencement du curl
    QProcess curlProcess;
    curlProcess.start("curl", arguments);

    //envoi contenue de email à curl's stdin
    curlProcess.write(fullBody.toUtf8());
    curlProcess.closeWriteChannel(); // Ensure the input is complete


    if (!curlProcess.waitForFinished(5000))
    {
        qDebug() << "Error: Timed out waiting for curl process to finish.";
        return false;
    }


    int exitCode = curlProcess.exitCode();
    qDebug() << "Curl process finished with exit code:" << exitCode;


    if (exitCode == 0)
    {
        qDebug() << "Email sent successfully!";
        return true;
    }
    else
    {
        qDebug() << "Error sending email. Exit code:" << exitCode;
        qDebug() << "Error details:" << curlProcess.readAllStandardError();
        return false;
    }

    qDebug() << "Sending email completed.";
    return false;
}
