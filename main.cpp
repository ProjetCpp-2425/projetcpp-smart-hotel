#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

#include <QDebug>
<<<<<<< HEAD
#include <QCoreApplication>
#include <QtNetwork>

=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Connection c;
<<<<<<< HEAD



=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
    qDebug() << "Attempting to establish a database connection...";

    bool test=c.createconnect();

    if(test)
    {w.show();
<<<<<<< HEAD
     w.refreshTableView();
=======
>>>>>>> f10aabdcfcd02b447f691da1d2153209c3578cb8
        qDebug() << "Connection successful.";

        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
    {
        qDebug() << "Connection failed.";

        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}

    return a.exec();



}








