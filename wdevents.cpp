#include "wdevents.h"

//monitor
ENewVisit::ENewVisit(int iPos):
    QEvent(Type(NEW_VISIT_MON))
{
    miPos = iPos ;
}

//composer
ERespAvail::ERespAvail():
    QEvent(Type(RESP_AVAIL_COMP))
{
}

//filter
EFHits::EFHits(int iType, QString &qstrUrl, QString &qstrNote):
    QEvent(Type(NEW_HIT_FLT))
{
    miType = iType;
    mqstrUrl = qstrUrl;
    mqstrNote = qstrNote;
}

//breakpoint
EBreakp::EBreakp(PBREAKMSG pMsg):
    QEvent(Type(RUN_TO_BRK))
{
    mpMsg = pMsg;
}
