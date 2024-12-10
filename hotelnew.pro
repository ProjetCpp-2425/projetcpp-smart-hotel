QT       += core gui
QT       += core gui sql
QT += printsupport
QT += printsupport sql
QT += charts
QT += sql
QT += network
QT += widgets
QT += core gui serialport




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    chambre.cpp \
    client.cpp \
    connection.cpp \
    employe.cpp \
    mailling.cpp \
    main.cpp \
    mainwindow.cpp \
    notesdialog.cpp \
    performance.cpp \
    qrcodegen.cpp \
    service.cpp \
    sms.cpp \
    statistique.cpp \
    reservation.cpp \
    transaction.cpp

HEADERS += \
    arduino.h \
    chambre.h \
    client.h \
    connection.h \
    employe.h \
    mailling.h \
    mainwindow.h \
    notesdialog.h \
    performance.h \
    qrcodegen.h \
    service.h \
    sms.h \
    statistique.h \
    reservation.h \
    transaction.h


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Source_Reservation.qrc
