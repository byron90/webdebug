#include <QApplication>
#include "wdworker.h"
#include "wddatar.h"
#include "wddef.h"
#include "wdevents.h"
#include "wdebug.h"

extern WDDatar datar ;
extern wdebug *pwdMainWin ;

WDWorker::WDWorker(int iSocketDes)
{
    miSocket = iSocketDes ;
}

void WDWorker::run()
{
    PDATANODE pdnode= new DATANODE ;
    mpSocket = new WDSocket(pdnode) ;
    mpSocket->setSocketDescriptor(miSocket) ;
    //read request package
    if(!mpSocket->mGetRequest())
        return ;
    datar.mAddNewRequ(pdnode) ; //save it
    //send the request to web server
    mpSocket->mSendRequest() ;
    //receive the response from web server
    if(!mpSocket->mRecvResponse())
        return ;
    datar.mAddNewResp(pdnode) ;	//save it
    //return the response to client
    mpSocket->mRetResponse() ;
    //save the pdnode to list
    int iPos = datar.mSaveOne(pdnode) ;
    //post message to UI
    ENewVisit * pclsEvnt = new ENewVisit(iPos) ;
    qApp->postEvent((QObject*)(pwdMainWin->pwinMonitor), pclsEvnt);
    mpSocket->close() ;
}
