#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QDateEdit>
#include <QtCharts>
#include <QSystemTrayIcon>

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

       void calculerTVA();
        std::vector<Transaction> chargerTransactionsDepuisBase();

        void refreshTableView();
        void sendMailIfMaxDepense();

private slots:
    void on_pushButton_14_clicked();  // Ajouter une transaction
    void on_pushButton_16_clicked();  // Supprimer une transaction
    void on_pushButton_15_clicked();// Modifier une transaction
     void on_pushButton_44_clicked();
       void on_comboBox_18_selectionChanged();
     void generatePdfForPeriod(const QDate &startDate, const QDate &endDate);
  void on_pushButton_stat_clicked();
    void afficherPageTransactions();
    void afficherPageChambres();
    void checkBudgetExceeded();
          void onTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
     void readyRead();
     void connected();
       void on_pushButton_calculerTVA_clicked();





private:
    Ui::MainWindow *ui;
    Transaction T;
    QDateEdit *dateEdit_3;
 QSqlQueryModel *model;   // Modèle pour QTableView

     QSystemTrayIcon *trayIcon;
       void setupTrayIcon();

        double maxDepense = 1000.0;






};
#endif // MAINWINDOW_H
