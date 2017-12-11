#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator helpTranslator;
    if (helpTranslator.load("qthelp", ":/translations"))
    {
        a.installTranslator(&helpTranslator);
    }

    QTranslator baseTranslator;
    if (baseTranslator.load("qtbase", ":/translations"))
    {
        a.installTranslator(&baseTranslator);
    }

    QTranslator appTranslator;

    if (appTranslator.load(QLocale(), QLatin1String("citizen"), QString("_"), ":/translations"))
    {
        a.installTranslator(&appTranslator);
    }
    else
    {
        exit(1);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
