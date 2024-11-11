#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QDateEdit>
#include "transaction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_14_clicked();  // Ajouter une transaction
    void on_pushButton_16_clicked();  // Supprimer une transaction
    void on_pushButton_15_clicked();// Modifier une transaction
     void on_pushButton_44_clicked();
       void on_comboBox_18_selectionChanged();
        void generatePdf();

private:
    Ui::MainWindow *ui;
    Transaction T;
    QDateEdit *dateEdit_3;
 QSqlQueryModel *model;   // Modèle pour QTableView
     void refreshTableView();// Méthode pour actualiser le QTableView



};
#endif // MAINWINDOW_H
