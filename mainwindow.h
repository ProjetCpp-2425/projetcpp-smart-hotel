#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDate>
#include <QString>
#include <QSqlQuery>
#include <QMessageBox>
#include "employe.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Login-related slots
    void on_pushButton_login_clicked();          // Slot for the "Login" button
    void on_pushButton_forgotPassword_clicked(); // Slot for the "Forgot Password" button

    // Employee management slots
    void on_pushButton_ajouter_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_chercher_clicked();
    void on_pushButton_pdf_clicked();
    void on_pushButton_statistique_clicked();
    void on_pushButton_trierParSalaire_clicked();

private:
    // Login validation and authentication
    bool validateLoginInputs(const QString &cin, const QString &mot_de_passe); // Validate login fields
    bool authenticate(const QString &cin, const QString &mot_de_passe);       // Authenticate user

    // Employee management validation
    bool validateInputs(int cin, const QString &nom, const QString &prenom,
                        const QString &question, const QString &sexe,
                        const QString &poste, float salaire,
                        const QString &mot_de_passe);

    Ui::MainWindow *ui;       // User interface
    Employe employe;           // Instance of Employe class
};

#endif // MAINWINDOW_H
