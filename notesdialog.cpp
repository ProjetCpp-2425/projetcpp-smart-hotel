#include "mainwindow.h"
#include "notesdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QPlainTextEdit>
#include <QDebug>

// NotesDialog Implementation
notesdialog::notesdialog(QString roomId, QString note, QWidget *parent)
    : QWidget(parent), roomId(roomId), note(note)
{
    // Setup UI elements
    QVBoxLayout *layout = new QVBoxLayout(this);

    notesEdit = new QPlainTextEdit(this);
    layout->addWidget(notesEdit);

    // Set the note text if provided (useful if we are editing an existing note)
    notesEdit->setPlainText(note);

    QPushButton *saveButton = new QPushButton("Save Note", this);
    layout->addWidget(saveButton);

    QPushButton *closeButton = new QPushButton("Close", this); // Add a "Close" button
    layout->addWidget(closeButton);

    connect(saveButton, &QPushButton::clicked, this, &notesdialog::saveNote);
    connect(closeButton, &QPushButton::clicked, this, &notesdialog::close); // Connect the Close button to close the window

    setLayout(layout);
    setWindowTitle("Notes for Room: " + roomId);
    resize(400, 300);
}

int notesdialog::generateUniqueId()
{
    // Fetch the next ID from a sequence, or use other logic for unique ID generation
    QSqlQuery query;
    query.exec("SELECT COALESCE(MAX(id), 0) + 1 FROM notes_history");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 1; // Default to 1 if no IDs exist
}


void notesdialog::saveNote()
{
    QString newNote = notesEdit->toPlainText().trimmed();

    if (newNote.isEmpty()) {
        QMessageBox::warning(this, "Error", "The note cannot be empty.");
        return;
    }

    QSqlQuery query;

    // Check if roomId is a number (if it's stored as NUMBER in the database)
    bool ok;
    int roomIdInt = roomId.toInt(&ok);
    if (!ok) {
        QMessageBox::critical(this, "Error", "Invalid room ID.");
        return;
    }

    // Try to update the existing note for the given room_id
    query.prepare("UPDATE notes_history SET note = :note WHERE room_id = :room_id");
    query.bindValue(":room_id", roomIdInt);  // Ensure roomId is an integer if necessary
    query.bindValue(":note", newNote);       // Ensure note is a string or CLOB

    if (!query.exec()) {
        QMessageBox::critical(this, "SQL Error", query.lastError().text());
        return;
    }

    // If no rows were updated, insert a new note with a generated ID
    if (query.numRowsAffected() == 0) {
        int generatedId = generateUniqueId();  // Use custom logic to generate unique ID

        query.prepare("INSERT INTO notes_history (id, room_id, note) VALUES (:id, :room_id, :note)");
        query.bindValue(":id", generatedId);
        query.bindValue(":room_id", roomIdInt);
        query.bindValue(":note", newNote);

        if (!query.exec()) {
            QMessageBox::critical(this, "SQL Error", query.lastError().text());
            return;
        }
    }

    QMessageBox::information(this, "Success", "Note saved successfully.");
}

// Function to generate a unique ID

