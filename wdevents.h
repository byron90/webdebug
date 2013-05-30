#ifndef WDEVENTS_H
#define WDEVENTS_H

//custom event classes
#include <QEvent>

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

#endif // WDEVENTS_H
