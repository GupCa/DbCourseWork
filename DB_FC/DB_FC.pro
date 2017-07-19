#-------------------------------------------------
#
# Project created by QtCreator 2017-03-28T15:09:20
#
#-------------------------------------------------

QT       += core gui sql
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DB_FC
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    hiringdialog.cpp \
    loginwindow.cpp \
    addphysical.cpp \
    addabsence.cpp \
    dialogdepartament.cpp \
    dialogmovingstaff.cpp \
    dialogadddepartment.cpp

HEADERS  += mainwindow.h \
    hiringdialog.h \
    loginwindow.h \
    addphysical.h \
    addabsence.h \
    dialogdepartament.h \
    dialogmovingstaff.h \
    dialogadddepartment.h

FORMS    += mainwindow.ui \
    hiringdialog.ui \
    loginwindow.ui \
    addphysical.ui \
    addabsence.ui \
    dialogdepartament.ui \
    dialogmovingstaff.ui \
    dialogadddepartment.ui



INCLUDEPATH = $$PWD/xls/include_cpp
LIBS += -framework LibXL

QMAKE_LFLAGS += -F../../
QMAKE_POST_LINK +=$$quote(mkdir $${TARGET}.app/Contents/Frameworks;cp -R ../../LibXL.framework $${TARGET}.app/Contents/Frameworks/)


