#ifndef WDEVENTS_H
#define WDEVENTS_H

//custom event classes
#include <QEvent>
#include <QString>
#include "wddef.h"

//new visit available --monitor
#define NEW_VISIT_MON QEvent::User+100
class ENewVisit:public QEvent
{
public:
    ENewVisit(int iPos);

    //int pos of the new visit node
    int miPos ;
};

//response available --composer
#define RESP_AVAIL_COMP QEvent::User+101
class ERespAvail:public QEvent
{
public:
    ERespAvail();
};

//filter hits --filter
#define NEW_HIT_FLT QEvent::User+102
class EFHits:public QEvent
{
public:
    EFHits(int iType, QString &qstrUrl, QString &qstrNote);

    //data
    int miType;	//filter type
    QString mqstrUrl;	//filter url
    QString mqstrNote;	//notes
};

//thread runs to breakpoint --breakpoint
#define RUN_TO_BRK QEvent::User+103
class EBreakp:public QEvent
{
public:
    EBreakp(PBREAKMSG pMsg);

    //data
    PBREAKMSG mpMsg;	//msg parameters struct
};


#endif // WDEVENTS_H
