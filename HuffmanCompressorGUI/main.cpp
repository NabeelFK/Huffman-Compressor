#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile styleFile(":/styles.qss");

    if (styleFile.open(QFile::ReadOnly))
    {
        app.setStyleSheet(styleFile.readAll());
    }

    MainWindow w;
    w.show();

    return app.exec();
}