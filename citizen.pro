#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T20:08:58
#
#-------------------------------------------------

QT       += core sql gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = citizen
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


SOURCES += \
		main.cpp \
		mainwindow.cpp \
	userdialog.cpp \
    searchform.cpp \
    userqueueform.cpp

HEADERS += \
		mainwindow.h \
	userdialog.h \
    searchform.h \
    userqueueform.h \
    sqlconnection.h

FORMS += \
		mainwindow.ui \
	userdialog.ui \
    searchform.ui \
    userqueueform.ui

RESOURCES += \
	main.qrc

TRANSLATIONS = translations/citizen_ru.ts

DISTFILES += \
	connection.ini \
	init.sql \
	translations/citizen_ru.ts \
	translations/citizen_ru.qm \
	translations/qtbase_ru.qm \
	translations/qthelp_ru.qm
