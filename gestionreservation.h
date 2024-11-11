#ifndef GESTIONRESERVATION_H
#define GESTIONRESERVATION_H

#include <QMainWindow>
#include "reservation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GestionReservation; }
QT_END_NAMESPACE

class GestionReservation : public QMainWindow
{
    Q_OBJECT

public:
    GestionReservation(QWidget *parent = nullptr);
    ~GestionReservation();

private slots:
    void on_pushButton_ajouter_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_rechercher_clicked();

    void on_pushButton_modifier_clicked();

    void on_pushButton_pdf_clicked();

    void on_pushButton_recherchers_clicked();

    void on_pushButton_27_clicked();

private:
    Ui::GestionReservation *ui;
    Reservation reservation;

};
#endif // GESTIONRESERVATION_H
