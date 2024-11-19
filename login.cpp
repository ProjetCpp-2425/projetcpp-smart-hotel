/*#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      employe()
{
    ui->setupUi(this);

    // Placeholder text for login fields
    ui->lineEdit_cin2->setPlaceholderText("Entrez votre CIN");
    ui->lineEdit_mdp2->setPlaceholderText("Entrez votre mot de passe");
    ui->lineEdit_mdp2->setEchoMode(QLineEdit::Password); // Mask password
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Validate login inputs (CIN and password)
bool MainWindow::validateLoginInputs(const QString &cin, const QString &mot_de_passe)
{
    bool ok;
    cin.toUInt(&ok); // Check if CIN is a valid integer
    if (!ok || cin.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Le CIN doit être un entier composé de 8 chiffres.");
        return false;
    }

    if (mot_de_passe.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe ne peut pas être vide.");
        return false;
    }

    return true;
}

// Authenticate the user with the database
bool MainWindow::authenticate(const QString &cin, const QString &mot_de_passe)
{
    QSqlQuery query;
    query.prepare("SELECT mot_de_passe FROM employe WHERE cin = :cin");
    query.bindValue(":cin", cin);

    if (query.exec()) {
        if (query.next()) {
            QString dbPassword = query.value(0).toString();
            if (dbPassword == mot_de_passe) {
                return true; // Login successful
            } else {
                QMessageBox::warning(this, "Erreur", "Mot de passe incorrect.");
                return false;
            }
        } else {
            QMessageBox::warning(this, "Erreur", "CIN introuvable.");
            return false;
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la requête.");
        return false;
    }
}

// Slot for "Log in" button
void MainWindow::on_pushButton_login_clicked()
{
    QString cin = ui->lineEdit_cin2->text();
    QString mot_de_passe = ui->lineEdit_mdp2->text();

    // Validate inputs
    if (validateLoginInputs(cin, mot_de_passe)) {
        if (authenticate(cin, mot_de_passe)) {
            QMessageBox::information(this, "Succès", "Connexion réussie !");
            // Switch to the main application page
            ui->stackedWidget->setCurrentIndex(1); // Example: Switch to page 1 in stacked widget
        }
    }
}

// Slot for "Forgot Password" button
void MainWindow::on_pushButton_forgotPassword_clicked()
{
    QString cin = ui->lineEdit_cin2->text();

    // Validate CIN before attempting password reset
    bool ok;
    cin.toUInt(&ok);
    if (!ok || cin.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Entrez un CIN valide avant de réinitialiser le mot de passe.");
        return;
    }

    // Example implementation for password reset
    QMessageBox::information(this, "Mot de passe oublié", "Un email a été envoyé avec des instructions pour réinitialiser votre mot de passe.");
}
*/
