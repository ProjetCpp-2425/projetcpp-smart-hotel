/*#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSqlQuery>
#include <QMessageBox>
#include "employe.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_login_clicked();          // Slot for the "Login" button
    void on_pushButton_forgotPassword_clicked(); // Slot for the "Forgot Password" button

private:
    bool validateLoginInputs(const QString &cin, const QString &mot_de_passe); // Validation of fields
    bool authenticate(const QString &cin, const QString &mot_de_passe);
    // Authentication with the database


    Ui::MainWindow *ui;   // User interface
    Employe employe;       // Instance of Employe class
};

#endif // MAINWINDOW_H
*/
