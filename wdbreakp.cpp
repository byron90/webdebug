#include <QApplication>
#include "wdbreakp.h"
#include "wdevents.h"
#include "wdebug.h"

extern WDGlobal globals;
extern wdebug *pwdMainWin;

WDBreakp::WDBreakp()
{
}

//return true if breakpoint is set
//else false
bool WDBreakp::checkBreak(QString &qstrMethod)
{
    bool bRes = false;
    //GET
    if(qstrMethod=="GET")
    {
        globals.mpBreakPoint->qmtxLock.lock();
        bRes = globals.mpBreakPoint->bGet;
        globals.mpBreakPoint->qmtxLock.unlock();
    }
    //POST
    else if(qstrMethod=="POST")
    {
        globals.mpBreakPoint->qmtxLock.lock();
        bRes = globals.mpBreakPoint->bPost;
        globals.mpBreakPoint->qmtxLock.unlock();
    }
    return bRes;
}

//wait for user to operate the breakpoint
//and post message to UI
void WDBreakp::waitForOper(PDATANODE pdnode)
{
    PBREAKMSG pBreakMsg = new BREAKMSG;
    pBreakMsg->pdnode = pdnode;
    //post message to break ui
    EBreakp *pclsEvnt = new EBreakp(pBreakMsg);
    qApp->postEvent((QObject*)pwdMainWin->pwinBreakPnt, pclsEvnt);
    //wait for operated
    QMutex qmtxLock;
    qmtxLock.lock();
    pBreakMsg->qwaitBlock.wait(&qmtxLock);
    qmtxLock.unlock();
}
