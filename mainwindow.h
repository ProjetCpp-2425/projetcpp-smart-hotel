
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cleaning.h"

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
    void ajouterCleaning();
    void modifierCleaning();
    void supprimerCleaning();

private:
    Ui::MainWindow *ui;
    Cleaning *cleaning;
};

#endif // MAINWINDOW_H
