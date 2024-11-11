#include "gestionreservation.h"
#include <QMessageBox>
#include <QApplication>
#include "connection.h"

int main(int argc, char *argv[])
{
    Connection c;
    bool test=c.createconnect();

    QApplication a(argc, argv);
    GestionReservation w;

    if(test)
    {

       QMessageBox::information(nullptr,QObject::tr("database is open"),
                                QObject::tr("connection successful \n"
                                            "Click Cancel to exit"),QMessageBox::Cancel);
     }
    else{
        QMessageBox::critical(nullptr,QObject::tr("database is not open"),
                                 QObject::tr("connection failed \n"
                                                 "Click Cancel to exit"),QMessageBox::Cancel);
        return  0;
    }



    w.show();
    return a.exec();
}
