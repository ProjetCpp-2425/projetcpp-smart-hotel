#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_button_ajouter_clicked();
    void on_button_modifier_clicked();
    void on_button_supprimer_clicked();
    void on_button_afficher_clicked();

private:
    Ui::MainWindow *ui;
    Employe employe;  // Instance de la classe Employe
};

#endif // MAINWINDOW_H
