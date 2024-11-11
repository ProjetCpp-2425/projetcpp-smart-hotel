#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chambre.h"
#include <QTableView>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void refreshTableView();
    void on_buttonRechercher_clicked();
    void on_buttonTrier_4_clicked();
    void on_pushButton_pdf_clicked();




private:
    Ui::MainWindow *ui;
    QTableView *tableView;
    QSqlQueryModel *model;
    Chambre chambre;
};

#endif // MAINWINDOW_H
