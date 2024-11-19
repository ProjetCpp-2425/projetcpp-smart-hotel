#include <QApplication>
#include "mainwindow.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create a connection object
    Connection conn;

    // Attempt to connect to the database when the application starts
    if (!conn.createconnect()) {
        // If the connection fails, display an error and exit
        qDebug() << "Failed to connect to the database. Exiting application.";
        return -1;  // Exit with an error code
    }

    // Proceed to the main window if the connection is successful
    MainWindow w;
    w.show();

    return a.exec();  // Start the event loop
}
