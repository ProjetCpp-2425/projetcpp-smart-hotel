#include "statistique.h"
#include "employe.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QMessageBox>
#include <QDebug>

Statistique::Statistique() {}

void Statistique::exportToPdf(const QString &fileName) {
    if (fileName.isEmpty()) {
        return;
    }

    // Create a QTextDocument to construct the PDF content
    QTextDocument doc;
    doc.setHtml(generateHtmlContent());

    // Configure the printer for PDF output
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));

    // Print the document to PDF
    doc.print(&printer);

    // Display a message box to confirm the export
    QMessageBox::information(nullptr, "Exportation Réussie", "Les données statistiques ont été exportées en PDF.");
}

QString Statistique::generateHtmlContent() {
    QString htmlContent;

    // Title of the document
    htmlContent += "<h1 style='text-align: center;'>Données Statistiques</h1>";
    htmlContent += "<hr/>";

    // Dynamic Employee Statistics
    htmlContent += "<h2>Statistiques des Employés</h2>";
    htmlContent += getEmployeeStatistics();
    htmlContent += "<hr/>";


    return htmlContent;
}

QString Statistique::getEmployeeStatistics() {
    QString html;

    // Query to get the number of employees in each role
    QSqlQuery query;
    query.prepare("SELECT role, COUNT(*) AS count FROM employe GROUP BY role");
    if (query.exec()) {
        html += "<h3>Nombre d'Employés par Rôle</h3>";
        html += "<table border='1' cellpadding='5' cellspacing='0' width='100%'>"
                "<tr><th>Rôle</th><th>Nombre d'Employés</th></tr>";
        while (query.next()) {
            QString role = query.value(0).toString();
            int count = query.value(1).toInt();
            html += QString("<tr><td>%1</td><td>%2</td></tr>").arg(role).arg(count);
        }
        html += "</table>";
    } else {
        qDebug() << "Error fetching employee roles:" << query.lastError().text();
    }

    // Query to get average salary
    query.prepare("SELECT AVG(salaire) AS avg_salary FROM employe");
    if (query.exec()) {
        query.next();
        float avgSalary = query.value(0).toFloat();
        html += "<h3>Salaire Moyen des Employés</h3>";
        html += QString("<p>Salaire moyen: %1</p>").arg(avgSalary);
    } else {
        qDebug() << "Error fetching average salary:" << query.lastError().text();
    }

    // Query to get gender distribution
    query.prepare("SELECT sexe, COUNT(*) AS count FROM employe GROUP BY sexe");
    if (query.exec()) {
        html += "<h3>Répartition par Sexe</h3>";
        html += "<table border='1' cellpadding='5' cellspacing='0' width='100%'>"
                "<tr><th>Sexe</th><th>Nombre d'Employés</th></tr>";
        while (query.next()) {
            QString sexe = query.value(0).toString();
            int count = query.value(1).toInt();
            html += QString("<tr><td>%1</td><td>%2</td></tr>").arg(sexe).arg(count);
        }
        html += "</table>";
    } else {
        qDebug() << "Error fetching gender distribution:" << query.lastError().text();
    }

    return html;
}

QString Statistique::getPaymentStatistics() {
    QString html;

    // Static data example for payment statistics
    QStringList paymentTypes = {"Carte de Crédit", "Virement Bancaire", "Chèque"};
    QList<int> counts = {15, 25, 10};

    html += "<table border='1' cellpadding='5' cellspacing='0' width='100%'>"
            "<tr><th>Type de Paiement</th><th>Nombre</th></tr>";

    for (int i = 0; i < paymentTypes.size(); ++i) {
        html += QString("<tr><td>%1</td><td>%2</td></tr>").arg(paymentTypes[i]).arg(counts[i]);
    }

    html += "</table>";

    return html;
}

QString Statistique::getMonthlySupplierStatistics() {
    QString html;

    // Static data example for monthly supplier statistics
    QStringList months = {"Jan", "Feb", "Mar", "Apr", "May", "Jun"};
    QList<int> counts = {5, 7, 6, 8, 4, 9};

    html += "<table border='1' cellpadding='5' cellspacing='0' width='100%'>"
            "<tr><th>Mois</th><th>Nombre de Fournisseurs Ajoutés</th></tr>";

    for (int i = 0; i < months.size(); ++i) {
        html += QString("<tr><td>%1</td><td>%2</td></tr>").arg(months[i]).arg(counts[i]);
    }

    html += "</table>";

    return html;
}

QString Statistique::getAdditionalStatistics() {
    QString html;

    // Static additional statistics
    html += "<table border='1' cellpadding='5' cellspacing='0' width='100%'>";
    html += "<tr><td>Valeur Moyenne du Paiement</td><td>1500</td></tr>";
    html += "<tr><td>Méthode de Paiement</td><td>Carte de Crédit</td></tr>";
    html += "</table>";

    return html;
}
