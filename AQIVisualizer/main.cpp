#include "stdafx.h"
#include "AQIVisualizer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AQIVisualizer w;
    w.show();
    return a.exec();
}
