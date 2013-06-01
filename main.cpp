#include <QtGui/QApplication>
#include <QDateTime>
#include "wdebug.h"
#include "wddatar.h"
#include "wdglobal.h"

//config datas
QString qstrBlockUrlPath = ".\\urls_block.txt";
QString qstrAutoRespPath = ".\\auto_response.txt";
QString qstrLoggerPath = ".\\logger.txt";
QString qstrTmpRespPath = ".\\template\\response.txt";

//global datas
WDDatar datar ;
wdebug * pwdMainWin ;
WDGlobal globals;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //global datas
    globals.mqstrTmpRespPath = qstrTmpRespPath;
    globals.initLogger(qstrLoggerPath);
    globals.initBlockUrl(qstrBlockUrlPath);
    globals.initAutoResponse(qstrAutoRespPath);
    //log start time
    QString qstrFormat("yyyy-MM-dd hh:mm:ss");
    QString qstrStart = "start time: " + QDateTime::currentDateTime().toString(qstrFormat);
    qstrStart += "\n";
    globals.mpLogger->pqdocLog->write(qstrStart.toAscii());
    //main window
    wdebug w;
    pwdMainWin = &w ;
    w.mStartServer() ;
    w.show();
    int iRes = a.exec();
    //log end time
    qstrFormat = "yyyy-MM-dd hh:mm:ss";
    qstrStart = "end time: " + QDateTime::currentDateTime().toString(qstrFormat);
    qstrStart += "\n";
    globals.mpLogger->pqdocLog->write(qstrStart.toAscii());
    //close logger
    globals.closeLogger();
    return iRes;
}
