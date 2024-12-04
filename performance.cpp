#include "performance.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDate>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include "statistique.h"
#include <QInputDialog>

Performance::Performance(QWidget *parent, QSqlDatabase db) : QWidget(parent), database(db)
{
    // Initialisation des widgets
    lineEditNomPrenom = new QLineEdit(this);
    plainTextEditCommentaire = new QPlainTextEdit(this);

    for (int i = 0; i < 5; ++i) {
        checkBoxRatings[i] = new QCheckBox(QString::number(i + 1), this);
    }

    captchaLabel = new QLabel(this);
    lineEditCaptcha = new QLineEdit(this);
    submitButton = new QPushButton("Soumettre", this);
    cancelButton = new QPushButton("Annuler", this);

    // Génération du CAPTCHA
    generatedCaptcha = generateRandomString(6);
    QImage captchaImage = generateCaptchaImage(generatedCaptcha);
    captchaLabel->setPixmap(QPixmap::fromImage(captchaImage));

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel("Nom et Prénom de l'employé :"));
    mainLayout->addWidget(lineEditNomPrenom);

    mainLayout->addWidget(new QLabel("Donnez une note pour chaque question (1 à 5) :"));
    QHBoxLayout *ratingsLayout = new QHBoxLayout();
    for (int i = 0; i < 5; ++i) {
        ratingsLayout->addWidget(checkBoxRatings[i]);
    }
    mainLayout->addLayout(ratingsLayout);

    mainLayout->addWidget(new QLabel("Commentaire :"));
    mainLayout->addWidget(plainTextEditCommentaire);

    mainLayout->addWidget(new QLabel("Confirmez le CAPTCHA :"));
    mainLayout->addWidget(captchaLabel);
    mainLayout->addWidget(lineEditCaptcha);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Connexions
    connect(submitButton, &QPushButton::clicked, this, &Performance::submitEvaluation);
    connect(cancelButton, &QPushButton::clicked, this, &Performance::close);
}


QString Performance::generateRandomString(int length)
{
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString result;
    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        result.append(chars.at(index));
    }
    return result;
}

QImage Performance::generateCaptchaImage(const QString &captchaText)
{
    QImage image(200, 60, QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    QFont font = painter.font();
    font.setPointSize(20);
    painter.setFont(font);

    // Dessiner le texte CAPTCHA
    painter.setPen(Qt::black);
    painter.drawText(image.rect(), Qt::AlignCenter, captchaText);

    // Ajouter du bruit
    for (int i = 0; i < 100; ++i) {
        int x = QRandomGenerator::global()->bounded(image.width());
        int y = QRandomGenerator::global()->bounded(image.height());
        image.setPixel(x, y, qRgb(0, 0, 0));
    }

    return image;
}

void Performance::submitEvaluation()
{
    // Vérifier le CAPTCHA
    if (lineEditCaptcha->text().trimmed() != generatedCaptcha) {
        QMessageBox::warning(this, "Erreur CAPTCHA", "Le CAPTCHA est incorrect. Veuillez réessayer.");
        regenerateCaptcha();
        return;
    }

    // Vérifier le champ Nom et Prénom
    QString nomPrenom = lineEditNomPrenom->text().trimmed();
    if (nomPrenom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un nom et un prénom.");
        return;
    }

    QStringList nomPrenomParts = nomPrenom.split(" ");
    if (nomPrenomParts.size() < 2) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un nom et un prénom valides (au moins deux mots).");
        return;
    }

    // Vérifier qu'au moins une note a été sélectionnée
    int score = 0;
    for (int i = 0; i < 5; ++i) {
        if (checkBoxRatings[i]->isChecked()) {
            score += i + 1;
        }
    }

    if (score == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez attribuer au moins une note.");
        return;
    }

    // Vérifier la connexion à la base de données
    if (!database.isOpen()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base de données échouée.");
        return;
    }

    // Sauvegarder dans la base de données
    QString commentaire = plainTextEditCommentaire->toPlainText();
    QSqlQuery query(database);
    query.prepare("INSERT INTO performance (cin_employe, date_evaluation, score, commentaire) "
                  "VALUES ((SELECT cin FROM employe WHERE nom = :nom AND prenom = :prenom), "
                  "SYSDATE, :score, :commentaire)");
    query.bindValue(":nom", nomPrenomParts[0]);
    query.bindValue(":prenom", nomPrenomParts[1]);
    query.bindValue(":score", score);
    query.bindValue(":commentaire", commentaire);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Évaluation enregistrée avec succès.");
        close();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d'enregistrer l'évaluation.\n" + query.lastError().text());
    }
}

void Performance::regenerateCaptcha()
{
    generatedCaptcha = generateRandomString(6);
    QImage captchaImage = generateCaptchaImage(generatedCaptcha);
    captchaLabel->setPixmap(QPixmap::fromImage(captchaImage));
}
