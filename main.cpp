#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cleaning.db");

    if (!db.open()) {
        qDebug() << "Erreur d'ouverture de la base de données:" << db.lastError();
        return -1;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS cleaning ("
               "id_chambre INTEGER PRIMARY KEY, "
               "nom_client TEXT, "
               "date DATE, "
               "frequence TEXT, "
               "services TEXT, "
               "heure_preferee TEXT, "
               "disponible BOOLEAN)");

    MainWindow w;
    w.show();
    return a.exec();
}

