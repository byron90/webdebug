#include <QtGui/QApplication>
#include "wdebug.h"
#include "wddatar.h"

WDDatar datar ;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    wdebug w;
    w.show();
    
    return a.exec();
}
