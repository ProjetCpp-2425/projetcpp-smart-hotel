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



    void on_pushButton_goto_confirmation_clicked();

    //void on_pushButton_vocal_clicked();


    void on_pushButton_19_destroyed(QObject *arg1);

private:
    Ui::GestionReservation *ui;
    Reservation reservation;
    Arduino A;


};
#endif // GESTIONRESERVATION_H
