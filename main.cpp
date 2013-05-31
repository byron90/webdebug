#include <QtGui/QApplication>
#include "wdebug.h"
#include "wddatar.h"
#include "wdglobal.h"

//config datas
QString qstrBlockUrlPath = ".\\urls_block.txt";
QString qstrAutoRespPath = ".\\auto_response.txt";

//global datas
WDDatar datar ;
wdebug * pwdMainWin ;
WDGlobal globals;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //global datas
    globals.initBlockUrl(qstrBlockUrlPath);
    globals.initAutoResponse(qstrAutoRespPath);
    //main window
    wdebug w;
    pwdMainWin = &w ;
    w.mStartServer() ;
    w.show();
    return a.exec();
}
