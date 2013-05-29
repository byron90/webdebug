#ifndef WDEVENTS_H
#define WDEVENTS_H

//custom event classes
#include <QEvent>

//new visit available
#define NEW_VISIT_MON QEvent::User+100
class ENewVisit:public QEvent
{
public:
    ENewVisit(int iPos);

    //int pos of the new visit node
    int miPos ;
};

#endif // WDEVENTS_H
