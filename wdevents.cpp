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

EFHits::EFHits(int iType, QString &qstrUrl, QString &qstrNote):
    QEvent(Type(NEW_HIT_FLT))
{
    miType = iType;
    mqstrUrl = qstrUrl;
    mqstrNote = qstrNote;
}
