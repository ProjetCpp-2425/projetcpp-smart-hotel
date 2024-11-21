#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "service.h"  // Include the Service class header
#include <vector>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void on_changeStatutButton_clicked();
    void displayAsciiChart(const QList<Service> &services); // Example placeholder function
    void displayServices();

private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_afficher_clicked();
    void refreshTableView();

    void on_pushButton_statistique_clicked();
    void on_pushButton_pdf_clicked();
    void on_pushButton_trier_clicked();
    void on_pushButton_recherche_clicked();
    void sendSMS(const QString &clientPhoneNumber, const QString &messageBody);

    void notifySMS(const QString &TELEPHONE, const QString &message);

    void on_notifications_clicked();

    bool validateForm();



private:
    Ui::MainWindow *ui;
    Service *service; // Pointer to Service class
};

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>

class StatistiqueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatistiqueWidget(QWidget *parent = nullptr) : QWidget(parent) {}

    void setStatistics(int cleaningCount, int foodCount) {
        this->cleaningCount = cleaningCount;
        this->foodCount = foodCount;
        update(); // Redraw the widget
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int widgetWidth = width();
        int widgetHeight = height();

        // Calculate the maximum count for scaling purposes
        int maxCount = qMax(cleaningCount, foodCount);

        // Set up the colors for each category
        QBrush cleaningBrush(Qt::blue);
        QBrush foodBrush(Qt::green);

        // Draw Cleaning bar
        int cleaningBarWidth = (float)cleaningCount / maxCount * widgetWidth;
        painter.setBrush(cleaningBrush);
        painter.drawRect(50, widgetHeight / 3, cleaningBarWidth, 30);

        // Draw Food bar
        int foodBarWidth = (float)foodCount / maxCount * widgetWidth;
        painter.setBrush(foodBrush);
        painter.drawRect(50, widgetHeight / 1.5, foodBarWidth, 30);

        // Add text labels
        painter.setPen(QPen(Qt::black));
        painter.drawText(50, widgetHeight / 3 - 10, "Cleaning: " + QString::number(cleaningCount));
        painter.drawText(50, widgetHeight / 1.5 - 10, "Food: " + QString::number(foodCount));
    }

private:
    int cleaningCount = 0;
    int foodCount = 0;
};



#endif // MAINWINDOW_H
