#include "wdevents.h"

ENewVisit::ENewVisit(int iPos):
    QEvent(Type(NEW_VISIT_MON))
{
    miPos = iPos ;
}

ERespAvail::ERespAvail():
    QEvent(Type(RESP_AVAIL_COMP))
{
}
