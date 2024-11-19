#ifndef STATISTIQUE_H
#define STATISTIQUE_H

#include <QString>

class Statistique {
public:
    Statistique();
    void exportToPdf(const QString &fileName);

private:
    QString generateHtmlContent();
    QString getPaymentStatistics();
    QString getMonthlySupplierStatistics();
    QString getAdditionalStatistics();
    QString getEmployeeStatistics();

};

#endif // STATISTIQUE_H
