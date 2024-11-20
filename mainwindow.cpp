#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDate>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include "statistique.h"
#include <QInputDialog>
#include <QtCharts/QChart>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChartView>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this); // Initialisation correcte de l'interface utilisateur

    // Set up placeholders and masked input
    ui->lineEdit_cin2->setPlaceholderText("Entrez votre CIN");
    ui->lineEdit_mdp2->setPlaceholderText("Entrez votre mot de passe");
    ui->lineEdit_mdp2->setEchoMode(QLineEdit::Password);

    // Set up employee management
    ui->tableView->setModel(employe.afficher());
    ui->dateEdit_date_embauche->setDate(QDate::currentDate());

    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::on_pushButton_pdf_clicked);


    // Connect CAPTCHA generation and validation
    connect(ui->refreshCaptchaButton, &QPushButton::clicked, this, &MainWindow::regenerateCaptcha);
    connect(ui->validateCaptchaButton, &QPushButton::clicked, this, &MainWindow::validateCaptcha);

    connect(ui->pushButton_envoyer, &QPushButton::clicked, this, &MainWindow::on_pushButton_envoyer_clicked);


    // Generate initial CAPTCHA
    regenerateCaptcha();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_statistique_clicked()
{
    try {
        // Crée un graphique (QChart) pour afficher les statistiques
        QChart *chart = new QChart();
        QBarSeries *series = new QBarSeries();
        QBarCategoryAxis *axis = new QBarCategoryAxis();

        QBarSet *set = new QBarSet("Nombre d'Employés");
        QStringList postesList;

        // Exécute une requête SQL pour récupérer les statistiques
        QSqlQuery query;
        query.prepare("SELECT poste, COUNT(*) AS count FROM employe GROUP BY poste");

        if (query.exec()) {
            while (query.next()) {
                QString poste = query.value(0).toString(); // Récupère le nom du poste
                int count = query.value(1).toInt();       // Récupère le nombre d'employés pour ce poste

                postesList.append(poste);
                *set << count; // Ajoute le nombre au jeu de barres
            }

            series->append(set);
            chart->addSeries(series);
            chart->setAnimationOptions(QChart::AllAnimations);
            axis->append(postesList);

            // Configure l'axe X et Y
            chart->createDefaultAxes();
            chart->addAxis(axis, Qt::AlignBottom); // Ajoute l'axe X au bas du graphique
            series->attachAxis(axis);             // Lie la série à l'axe X
            chart->legend()->setAlignment(Qt::AlignBottom);

            // Configure l'apparence du graphique
            QPalette pal = qApp->palette();
            pal.setColor(QPalette::Window, QRgb(0x0d4261));
            pal.setColor(QPalette::WindowText, QRgb(0x95212c));
            qApp->setPalette(pal);

            QFont font;
            font.setPixelSize(18);
            chart->setTitleFont(font);
            chart->setTitleBrush(QBrush(Qt::red));
            chart->setTitle("Statistiques : Nombre d'Employés par Poste");

            // Configure la vue du graphique
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            chartView->setChart(chart); // Assurez-vous que 'chartContainer' est un widget dans votre interface
            chartView->showNormal();
            chartView->show();

        } else {
            qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();
        }
    } catch (...) {
        qDebug() << "Une erreur inattendue s'est produite.";
    }
}

void MainWindow::on_pushButton_envoyer_clicked() {
    // Récupérer les données des champs
    QString cin = ui->lineEdit_cin3->text();
    QString nomPrenom = ui->lineEdit_nomprenom_5->text();
    QDate dateDebut = ui->dateEdit_dd->date();
    QDate dateFin = ui->dateEdit_df->date();
    QString typeAbsence = ui->comboBox_abs->currentText();

    // Vérifier si les champs sont remplis
    if (cin.isEmpty() || nomPrenom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Vérifier que la date de fin est après la date de début
    if (dateFin < dateDebut) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être postérieure à la date de début.");
        return;
    }

    // Calculer le nombre de jours d'absence
    int joursAbsence = calculerJoursAbsence(dateDebut, dateFin);

    // Insérer les données dans la base
    QSqlQuery query;
    query.prepare("INSERT INTO absences (cin, nom_prenom, date_debut, date_fin, type_absence, jours_absence) "
                  "VALUES (:cin, :nom_prenom, :date_debut, :date_fin, :type_absence, :jours_absence)");
    query.bindValue(":cin", cin);
    query.bindValue(":nom_prenom", nomPrenom);
    query.bindValue(":date_debut", dateDebut);
    query.bindValue(":date_fin", dateFin);
    query.bindValue(":type_absence", typeAbsence);
    query.bindValue(":jours_absence", joursAbsence);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", QString("Demande envoyée avec succès !\n"
                                                         "Nombre de jours d'absence : %1").arg(joursAbsence));
    } else {
        QMessageBox::critical(this, "Erreur", QString("Échec de l'envoi de la demande.\nErreur : %1")
                                               .arg(query.lastError().text()));
    }
}

int MainWindow::calculerJoursAbsence(const QDate &dateDebut, const QDate &dateFin) {
    // Calculer la différence entre les deux dates
    return dateDebut.daysTo(dateFin) + 1; // Inclure le premier jour
}

void MainWindow::validateCaptcha() {
    int userInput = ui->captchaInput->text().toInt(); // Read user input

    if (userInput == generatedCaptcha) {
        QMessageBox::information(this, "Succès", "Le CAPTCHA est correct !");
    } else {
        QMessageBox::warning(this, "Erreur", "Le CAPTCHA est incorrect. Veuillez réessayer.");
        ui->captchaInput->clear(); // Clear input field
        regenerateCaptcha();       // Generate new CAPTCHA
    }
}

void MainWindow::regenerateCaptcha() {
    // Generate a random 6-digit number
    generatedCaptcha = QRandomGenerator::global()->bounded(100000, 1000000); // Correct inclusive bounds

    // Display the CAPTCHA in the label
    ui->captchaLabel->setText(QString::number(generatedCaptcha));
}


// Validate login inputs (CIN and password)
bool MainWindow::validateLoginInputs(const QString &cin, const QString &mot_de_passe)
{
    bool ok;
    cin.toUInt(&ok);
    if (!ok || cin.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Le CIN doit être un entier composé de 8 chiffres.");
        return false;
    }

    if (mot_de_passe.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe ne peut pas être vide.");
        return false;
    }

    return true;
}

// Authenticate the user with the database
bool MainWindow::authenticate(const QString &cin, const QString &mot_de_passe)
{
    QSqlQuery query;
    query.prepare("SELECT mot_de_passe FROM employe WHERE cin = :cin");
    query.bindValue(":cin", cin);

    if (query.exec()) {
        if (query.next()) {
            QString dbPassword = query.value(0).toString();
            if (dbPassword == mot_de_passe) {
                return true;
            } else {
                QMessageBox::warning(this, "Erreur", "Mot de passe incorrect.");
                return false;
            }
        } else {
            QMessageBox::warning(this, "Erreur", "CIN introuvable.");
            return false;
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la requête.");
        return false;
    }
}

// Slot for "Log in" button
void MainWindow::on_pushButton_login_clicked()
{
    QString cin = ui->lineEdit_cin2->text();
    QString mot_de_passe = ui->lineEdit_mdp2->text();

    if (validateLoginInputs(cin, mot_de_passe) && authenticate(cin, mot_de_passe)) {
        QMessageBox::information(this, "Succès", "Connexion réussie !");
        ui->stackedWidget->setCurrentIndex(1); // Switch to main application page
    }
}

// Slot for "Forgot Password" button
void MainWindow::on_pushButton_forgotPassword_clicked()
{
    QString cin = ui->lineEdit_cin2->text();

    // Validation du CIN
    bool ok;
    cin.toUInt(&ok);
    if (!ok || cin.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Entrez un CIN valide avant de réinitialiser le mot de passe.");
        return;
    }

    // Rechercher la réponse associée à la question de sécurité fixe
    QSqlQuery query;
    query.prepare("SELECT question FROM employe WHERE cin = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "CIN introuvable dans la base de données.");
        return;
    }

    // Définir la question de sécurité fixe
    QString question = "Quelle est la ville où vous êtes né(e) ?";
    QString reponse = query.value("question").toString();

    // Poser la question à l'utilisateur
    QString userAnswer = QInputDialog::getText(this, "Question de sécurité", question, QLineEdit::Normal);
    if (userAnswer.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Vous devez répondre à la question.");
        return;
    }

    // Vérifier la réponse
    if (userAnswer.toLower().trimmed() == reponse.toLower().trimmed()) {
        QMessageBox::information(this, "Succès", "Réponse correcte. Vous pouvez réinitialiser votre mot de passe.");
        // Ajouter la logique pour réinitialiser le mot de passe (ex. ouvrir une nouvelle fenêtre).
    } else {
        QMessageBox::warning(this, "Erreur", "Réponse incorrecte. Impossible de réinitialiser le mot de passe.");
    }
}

/*// Function to handle exporting statistics to PDF
void MainWindow::on_pushButton_statistique_clicked() {
    // Get the filename and path for saving the PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter les Statistiques en PDF", "", "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) {
        return;
    }

    // Create a Statistique object (assuming Statistique class handles the actual export)
    Statistique stats;

    // Export the statistics to the selected PDF file
    stats.exportToPdf(fileName);

    // Show a message confirming the export
    QMessageBox::information(this, "Exportation réussie", "Les statistiques ont été exportées avec succès.");
}*/

// Function to export the table data to a PDF
void MainWindow::on_pushButton_pdf_clicked()
{
    // Ouvrir la boîte de dialogue pour choisir l'emplacement du fichier PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter vers PDF", "", "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) {
        return;
    }

    // Créer un QTextDocument pour construire le contenu du PDF
    QTextDocument doc;

    // Commencer à construire le contenu HTML pour le PDF
    QString htmlContent;

    // Titre du document
    htmlContent += "<h1 style='text-align: center;'>Enquête de Satisfaction des Clients</h1>";
    htmlContent += "<hr/>";  // Séparateur de ligne

    // Créer une table HTML simple pour afficher les statistiques de feedbacks
    htmlContent += "<h2>Résumé des Feedbacks</h2>";
    htmlContent += "<table border='1' cellpadding='5' cellspacing='0' width='100%'>";

    // Ajouter les en-têtes de la table (par exemple : type de feedback, nombre)
    htmlContent += "<tr>"
                    "<th>Type de Feedback</th>"
                    "<th>Nombre de Clients</th>"
                   "</tr>";

    // Exemple de données de feedbacks : types de satisfaction et le nombre de clients pour chaque
    QStringList feedbackTypes = {"Très satisfait", "Satisfait", "Moyennement satisfait", "Insatisfait"};
    QList<int> counts = {50, 30, 10, 5};

    // Ajouter les données de feedback dans la table
    for (int i = 0; i < feedbackTypes.size(); ++i) {
        htmlContent += QString("<tr><td>%1</td><td>%2</td></tr>").arg(feedbackTypes[i]).arg(counts[i]);
    }

    htmlContent += "</table>";

    // Ajouter la section des commentaires des clients
    htmlContent += "<h2>Commentaires des Clients</h2>";
    htmlContent += "<ul>";

    // Exemple de commentaires de clients
    QStringList comments = {
        "Excellente expérience, personnel très accueillant.",
        "Chambre propre, mais le bruit extérieur était dérangeant.",
        "Séjour agréable mais le service à la réception était lent."
    };

    // Ajouter les commentaires dans la liste HTML
    for (const QString &comment : comments) {
        htmlContent += QString("<li>%1</li>").arg(comment);
    }

    htmlContent += "</ul>";

    // Configurer le document avec le contenu HTML
    doc.setHtml(htmlContent);

    // Configurer l'imprimante pour la sortie PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));

    // Imprimer le document dans le fichier PDF
    doc.print(&printer);

    // Afficher un message pour confirmer que l'exportation a réussi
    QMessageBox::information(this, "Exportation Réussie", "Les données de l'enquête de satisfaction ont été exportées en PDF.");
}


void MainWindow::on_pushButton_trierParSalaire_clicked()
{
    // Création de la requête SQL pour récupérer tous les employés triés par salaire
    QString queryStr = "SELECT * FROM employe ORDER BY salaire ASC";  // Tri par salaire croissant

    // Si vous souhaitez un tri décroissant, utilisez "DESC"
    // QString queryStr = "SELECT * FROM employe ORDER BY salaire DESC";

    QSqlQuery query;
    query.prepare(queryStr);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur de requête", "Une erreur est survenue lors de l'exécution de la requête.");
        return;
    }

    // Créez un modèle pour afficher les résultats
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(std::move(query));  // Use std::move here

    // Si des résultats sont trouvés, affichez-les dans le QTableView
    if (model->rowCount() > 0) {
        ui->tableView->setModel(model);
    } else {
        QMessageBox::information(this, "Aucun résultat", "Aucun employé trouvé.");
    }
}


// Function to handle searching for an employee by CIN
void MainWindow::on_pushButton_chercher_clicked()
{

    int cin = ui->lineEdit_cin->text().toInt();

    qDebug() << "CIN entered:" << cin;

    if (cin <= 0) {
        QMessageBox::warning(this, "Entrée invalide", "CIN doit être un entier positif.");
        return;
    }

    // Assuming that `rechercheByCin` returns a model for the search result
    QSqlQueryModel *model = employe.rechercheByCin(cin);

    if (!model) {
        QMessageBox::critical(this, "Erreur de requête", "Une erreur est survenue lors de la recherche de l'employé.");
        return;
    }

    // If records are found, set the model to the table view
    if (model->rowCount() > 0) {
        ui->tableView->setModel(model);
    } else {
        QMessageBox::information(this, "Aucun résultat", QString("Aucun employé trouvé avec CIN: %1").arg(cin));
    }
}

// Function to validate inputs
bool MainWindow::validateInputs(int cin, const QString &nom, const QString &prenom,
                                const QString &question, const QString &sexe,
                                const QString &poste, float salaire,
                                const QString &mot_de_passe) {    // CIN validation
    if (cin < 10000000 || cin > 99999999) {
        QMessageBox::warning(this, "Entrée invalide", "CIN doit être un entier positif composé de 8 chiffres.");
        return false;
    }

    // Name validation
    QRegularExpression nameRegex("^[A-Za-z]{2,50}$");
    if (!nameRegex.match(nom).hasMatch()) {
        QMessageBox::warning(this, "Entrée invalide", "Le nom doit contenir uniquement des lettres et être entre 2 et 50 caractères.");
        return false;
    }

    // Prenom validation
    if (!nameRegex.match(prenom).hasMatch()) {
        QMessageBox::warning(this, "Entrée invalide", "Le prénom doit contenir uniquement des lettres et être entre 2 et 50 caractères.");
        return false;
    }

    // Question validation
    if (question.isEmpty() || question.length() > 100) {
            QMessageBox::warning(this, "Entrée invalide", "La question ne peut pas être vide et doit contenir moins de 100 caractères.");
            return false;
        }

    // Sexe validation
    if (sexe != "M" && sexe != "F") {
        QMessageBox::warning(this, "Entrée invalide", "Le sexe doit être 'M' ou 'F'.");
        return false;
    }

    // Poste validation
    if (poste.isEmpty()) {
        QMessageBox::warning(this, "Entrée invalide", "Le poste ne peut pas être vide.");
        return false;
    }

    // Salaire validation
    if (salaire <= 0) {
        QMessageBox::warning(this, "Entrée invalide", "Le salaire doit être un nombre positif.");
        return false;
    }

    // Mot de passe validation
    if (mot_de_passe.length() < 8) {
        QMessageBox::warning(this, "Entrée invalide", "Le mot de passe doit contenir au moins 8 caractères.");
        return false;
    }

    QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,}$");
    if (!passwordRegex.match(mot_de_passe).hasMatch()) {
        QMessageBox::warning(this, "Entrée invalide", "Le mot de passe doit contenir au moins une lettre majuscule, une lettre minuscule, un chiffre et un caractère spécial.");
        return false;
    }

    return true;
}

// Add employee function
void MainWindow::on_pushButton_ajouter_clicked() {
    int cin = ui->lineEdit_cin->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString question = ui->lineEdit_question->text();
    QString sexe = ui->lineEdit_sexe->text();
    QString poste = ui->comboBox_poste->currentText();
    QDate date_embauche = ui->dateEdit_date_embauche->date();
    float salaire = ui->lineEdit_salaire->text().toFloat();
    QString mot_de_passe = ui->lineEdit_mdp->text();

    if (!validateInputs(cin, nom, prenom, question, sexe, poste, salaire, mot_de_passe)) {
        return;
    }

    employe.setCin(cin);
    employe.setNom(nom);
    employe.setPrenom(prenom);
    employe.setQuestion(question);
    employe.setSexe(sexe);
    employe.setPoste(poste);
    employe.setDate_embauche(date_embauche);
    employe.setSalaire(salaire);
    employe.setMot_de_passe(mot_de_passe);

    if (employe.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès.");
        ui->tableView->setModel(employe.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé.");
    }
}

// Modify employee function
void MainWindow::on_pushButton_modifier_clicked() {
    int cin = ui->lineEdit_cin->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString question = ui->lineEdit_question->text();
    QString sexe = ui->lineEdit_sexe->text();
    QString poste = ui->comboBox_poste->currentText();
    QDate date_embauche = ui->dateEdit_date_embauche->date();
    float salaire = ui->lineEdit_salaire->text().toFloat();
    QString mot_de_passe = ui->lineEdit_mdp->text();

    if (!validateInputs(cin, nom, prenom, question, sexe, poste, salaire, mot_de_passe)) {
        return;
    }

    Employe emp(cin, nom, prenom, question, sexe, poste, date_embauche, salaire, mot_de_passe);
    if (emp.modifier(cin)) {
        ui->tableView->setModel(employe.afficher());
        QMessageBox::information(this, "Modification", "Employé modifié avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de l'employé.");
    }
}

// Delete employee function
void MainWindow::on_pushButton_supprimer_clicked() {
    int id = ui->lineEdit_cin->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Entrée invalide", "CIN doit être un entier positif.");
        return;
    }

    if (employe.supprimer(id)) {
        ui->tableView->setModel(employe.afficher());
        QMessageBox::information(this, "Suppression", "Employé supprimé avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression de l'employé.");
    }
}



void MainWindow::on_pushButton_27_clicked()
{
    Employe e;
        try{
        //Crée un graphique (QChart) pour afficher les statistiques.
            QChart *chart = new QChart();
            QBarSeries *series = new QBarSeries();
           /*Crée une instance d'axe de catégorie (QBarCategoryAxis) pour l'axe X.*/
            QBarCategoryAxis *axis = new QBarCategoryAxis();

            QBarSet *set = new QBarSet(" Point de fidelite");
            QStringList typesList;
            QList<QBarSet *> nbrList;
            std::map<QString , int> list = e.statNbrPerType();
            for(auto itr = list.begin() ; itr != list.end(); itr++) {
                typesList.append(itr->first);
    //            nbrList.append(itr->second);
                *set << itr->second;
                nbrList.append(set);
            }
            qDebug() << typesList;
            series->append(set);
            chart->addSeries(series);
            chart->setAnimationOptions(QChart::AllAnimations);
            axis->append(typesList);
            chart->createDefaultAxes();
            chart->setAxisX(axis, series);
            chart->legend()->setAlignment(Qt::AlignBottom);
            //Crée une vue graphique (QChartView) et active l'anticrénelage pour un rendu plus lisse.
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            QPalette pal = qApp->palette();
            pal.setColor(QPalette::Window, QRgb(0x0d4261));
            pal.setColor(QPalette::WindowText, QRgb(0x95212c));
            qApp->setPalette(pal);
            QFont font;
            font.setPixelSize(40);
            chart->setTitleFont(font);
            chart->setTitleBrush(QBrush(Qt::red));
            chart->setTitle("statistique Point fidelite PER IDC");
            chartView->setChart(chart);
            chartView->showNormal();



        }catch(...){

}
}
