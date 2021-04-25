#include "widget.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>

// https://www.devbean.net/2013/06/qt-study-road-2-database/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();
    return a.exec();
}
