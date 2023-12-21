#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStyle>
#include <stdexcept>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // ПОДКЛЮЧЕНИЕ style.css
    QFile file(":/new/prefix1/style.css");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug("Style.css НЕ подключен");
    }
    else
    {
        qDebug("Style.css подключен");
    }
    a.setStyleSheet(file.readAll());
    // ПОДКЛЮЧЕНИЕ style.css

    w.show();
    return a.exec();
}
