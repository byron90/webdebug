#include "wdevents.h"

ENewVisit::ENewVisit(int iPos):
    QEvent(Type(NEW_VISIT_MON))
{
    miPos = iPos ;
}
