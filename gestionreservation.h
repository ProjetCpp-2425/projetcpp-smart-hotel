#ifndef GESTIONRESERVATION_H
#define GESTIONRESERVATION_H

#include <QMainWindow>
#include "reservation.h"
#include "arduino.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GestionReservation; }
QT_END_NAMESPACE

class GestionReservation : public QMainWindow
{
    Q_OBJECT




public:
    GestionReservation(QWidget *parent = nullptr);
    ~GestionReservation();
    void updateChart();
    void onTabChanged(int index);
    void updateCalendrier();
    void readReservationData(int id);
    bool ajouterHistorique(int clientId, QString &evenement, QString &details);


signals:
    void motionDetected(const QString &room);

private slots:
    void on_pushButton_ajouter_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_rechercher_clicked();

    void on_pushButton_modifier_clicked();

    void on_pushButton_pdf_clicked();

    void on_pushButton_recherchers_clicked();

    void showReservationTooltip(QDate &date);

    void onDateSelected(const QDate &date);

    void TriChanged();

    void readArduinoContinuously();
    void showMotionMessage(const QString &room);


    void on_pushButton_goto_confirmation_clicked();

    //void on_pushButton_vocal_clicked();


    void on_pushButton_19_destroyed(QObject *arg1);

    void on_pushButton_vocal_clicked();

    void on_pushButton_readArduino_clicked();

private:
    Ui::GestionReservation *ui;
    Reservation reservation;


    Arduino A;



    QThread *arduinoThread;
    QDateTime timenow;
    QDateTime derniereDetectMouvement;





};
#endif // GESTIONRESERVATION_H
