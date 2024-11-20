#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QPainter>
#include <QImage>

class Performance : public QWidget
{
    Q_OBJECT

public:
    Performance(QWidget *parent = nullptr, QSqlDatabase db = QSqlDatabase());

private:
    QSqlDatabase database;

    QLineEdit *lineEditNomPrenom;
    QCheckBox *checkBoxRatings[5]; // 5 CheckBoxes pour les notes (1 à 5)
    QPlainTextEdit *plainTextEditCommentaire;
    QLabel *captchaLabel;
    QLineEdit *lineEditCaptcha;
    QPushButton *submitButton;
    QPushButton *cancelButton;

    QString generatedCaptcha;

    QString generateRandomString(int length);
    QImage generateCaptchaImage(const QString &captchaText);

private slots:
    void submitEvaluation();
    void regenerateCaptcha();
};

#endif // PERFORMANCE_H
