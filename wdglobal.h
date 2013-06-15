#ifndef WDGLOBAL_H
#define WDGLOBAL_H

#include <QObject>
#include "wddef.h"

class WDGlobal : public QObject
{
    Q_OBJECT
public:
    explicit WDGlobal(QObject *parent = 0);

    //init global data
    //***logger
    void initLogger(QString &qstrFile);
    //***block url
    void initBlockUrl(QString &qstrFile);
    //***auto response
    void initAutoResponse(QString &qstrFile);

    //common func used to log
    void writeLog(QString &qstrLogContent);
    //***close logger
    void closeLogger();

    //global data
    //***proxy
    PPROXYCONF mpProxyConf;
    //***http response template's file path
    QString mqstrTmpRespPath;
    //***logger
    PLOGGER mpLogger;
    //***struct to manage block url
    PBLOCKURL mpBlockUrl;
    //***struct to manage auto responsing
    PAUTOR mpAutoResp;
    //***struct to manage hiding code
    PHIDECODES mpHideCodes;
    //***struct to manage block resp type
    PBLOCKTYPE mpBlockType;
    //***struct to manage breakpoint
    PBREAKP mpBreakPoint;
    
signals:
    
public slots:
    
};

#endif // WDGLOBAL_H
