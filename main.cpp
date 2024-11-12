#include <QApplication>
#include "mainwindow.h"
#include <QSqlDatabase>
#include <QMessageBox>

bool createConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("company.db");
    if (!db.open()) {
        QMessageBox::critical(nullptr, "Erreur de Base de Données", "Connexion à la base de données échouée.");
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (!createConnection())
        return -1;

    MainWindow w;
    w.show();

    return a.exec();
}
