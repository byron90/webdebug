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
    //***block url
    void initBlockUrl(QString &qstrFile);
    //***auto response
    void initAutoResponse(QString &qstrFile);

    //global data
    //***struct to manage block url
    PBLOCKURL mpBlockUrl;
    //***struct to manage auto responsing
    PAUTOR mpAutoResp;
    //***struct to manage hiding code
    PHIDECODES mpHideCodes;
    //***struct to manage block resp type
    PBLOCKTYPE mpBlockType;
    
signals:
    
public slots:
    
};

#endif // WDGLOBAL_H
