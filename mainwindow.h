#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include <QVector>
#include <QMainWindow>
#include <QMap>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getTelephone() const; // Méthode pour obtenir le numéro de téléphone

private slots:
    void on_ajouterButton_clicked();
    void on_modifierButton_clicked();
    void on_supprimerButton_clicked();
    void afficherClients();

    void on_chercherButton_clicked();
    void exporterEnCsv();
    void on_chatbutton_clicked();
    void on_afficherStatistiquesButton_clicked();    void enregistrerArrivee();              // Enregistrer l'arrivée d'un client
    void afficherHistorique();             // Afficher l'historique des visites


    void on_trier_clicked();


    void on_arriveeButton_clicked();


    void on_Obtenir_clicked();

    void on_Imprimer_clicked();

    void on_Obtenir_2_clicked();

private:
    Ui::MainWindow *ui;
    QVector<Client> clients;
    QMap<QString, int> historiqueClients; // Historique des visites (CIN -> nombre de visites)

    int trouverClient(const QString &cin);
};

#endif // MAINWINDOW_H
