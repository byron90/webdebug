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
    void initBlockUrl(QString &qstrFile);

    //global data
    //***struct to manage block url
    PBLOCKURL mpBlockUrl;
    
signals:
    
public slots:
    
};

#endif // WDGLOBAL_H
