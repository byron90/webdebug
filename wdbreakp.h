#ifndef WDBREAKP_H
#define WDBREAKP_H

//breakpoint manager
//usage: check breakpoints
//		 wait for breakpoint operation

#include <QString>
#include "wdglobal.h"

class WDBreakp
{
public:
    WDBreakp();

    //check breakpoints that have been set
    bool checkBreak(QString &qstrMethod);
    //wait for breakpoint operation
    void waitForOper(PDATANODE pdnode);
};

#endif // WDBREAKP_H
