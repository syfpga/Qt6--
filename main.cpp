#include "mainwidget.h"

#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWidget w;
    w.show();
    return a.exec();
}
