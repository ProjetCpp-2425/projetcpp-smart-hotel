#ifndef NOTEDIALOG_H
#define NOTEDIALOG_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

class notesdialog : public QWidget
{
    Q_OBJECT

public:
    // Constructor that accepts both roomId and note
    explicit notesdialog(QString roomId, QString note, QWidget *parent = nullptr);

private slots:
    void saveNote();
    int generateUniqueId();


private:
    QString roomId;         // Room ID for which the note is being added
    QString note;           // The note for the room
    QPlainTextEdit *notesEdit;  // Text editor for displaying and editing notes
    void loadNotes();      // Function to load existing notes for the room
};

#endif // NOTEDIALOG_H
