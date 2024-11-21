#include "mainwindow.h"
#include "connection.h" // Assuming this is your database connection class
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Initialize the database connection
    Connection c;
    bool isConnected = c.createconnect();

    // Check the connection status
    if (!isConnected) {
        QMessageBox::critical(nullptr, QObject::tr("Database Error"),
                              QObject::tr("Failed to connect to the database.\nClick Cancel to exit."),
                              QMessageBox::Cancel);
        return -1; // Exit the application if the database connection fails
    }

    QMessageBox::information(nullptr, QObject::tr("Database Connected"),
                             QObject::tr("Database connection successful.\nClick OK to continue."),
                             QMessageBox::Ok);

    // Launch the main application window
    MainWindow w;
    w.show();

    return a.exec();
}
