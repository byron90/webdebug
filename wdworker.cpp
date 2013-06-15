#include <QApplication>
#include <QRegExp>
#include "wdworker.h"
#include "wddatar.h"
//#include "wdglobal.h"
#include "wdevents.h"
#include "wdebug.h"
#include "wdfilter.h"
#include "wdbreakp.h"

extern WDDatar datar ;
extern wdebug *pwdMainWin ;
extern WDGlobal globals;

WDWorker::WDWorker(int iSocketDes)
{
    miSocket = iSocketDes ;
}


//delete heap space that pdnode alloc
void deletePdnode(PDATANODE pdnode)
{
    if(pdnode->pcRespRaw)
        delete pdnode->pcRespRaw;
    if(pdnode->pcRequRaw)
        delete pdnode->pcRequRaw;
    if(pdnode->pRequSum)
    {
        if(pdnode->pRequSum->pcHost)
            delete pdnode->pRequSum->pcHost;
        if(pdnode->pRequSum->pcUrl)
            delete pdnode->pRequSum->pcUrl;
        delete pdnode->pRequSum;
    }
    if(pdnode->pRespSum)
    {
        if(pdnode->pRespSum->pcType)
            delete pdnode->pRespSum->pcType;
        delete pdnode->pRespSum;
    }
    delete pdnode;
}

//core worker
void WDWorker::run()
{
    //filter class
    WDFilter fltFilter;
    //breakpoint class
    WDBreakp brkBreakPnt;
    //new visit node
    PDATANODE pdnode= new DATANODE ;
    pdnode->pcRequRaw = NULL; pdnode->pcRespRaw = NULL;
    pdnode->pRequSum = NULL; pdnode->pRespSum = NULL;
    mpSocket = new WDSocket(pdnode) ;
    mpSocket->setSocketDescriptor(miSocket) ;
    //read request package
    if(!mpSocket->mGetRequest())
    {
        deletePdnode(pdnode);
        delete mpSocket;
        return;
    }
    datar.mAnalyRequ(pdnode) ; //analyze request
    //********************************************* Filter BlockUrl & AutoResp
    //merge host and url
    QString qstrHost(pdnode->pRequSum->pcHost);
    QString qstrUrl(pdnode->pRequSum->pcUrl);
    QString qstrAbsUrl = qstrHost + qstrUrl;
    qstrAbsUrl.replace(QRegExp("([^:])//"), "\\1/");
    //check block url
    if(fltFilter.blockUrl(qstrAbsUrl))
    {
        deletePdnode(pdnode);
        delete mpSocket;
        return;
    }
    //check auto response
    QString qstrRetData = fltFilter.autoResponse(qstrAbsUrl);
    if(qstrRetData!="")
    {
        //return self specific http response to browser
        QByteArray qbyaRetData = qstrRetData.toAscii();
        mpSocket->write(qbyaRetData, qbyaRetData.length());
        mpSocket->waitForBytesWritten(30000);
        deletePdnode(pdnode);
        delete mpSocket;
        return;
    }
    //********************************************* end filter
    //********************************************* breakpoint
    if(brkBreakPnt.checkBreak(QString(pdnode->pRequSum->caMethod)))
        //set breakpoint
        brkBreakPnt.waitForOper(pdnode);
    //*********************************************
    //send the request to web server
    mpSocket->mSendRequest() ;
    //receive the response from web server
    if(!mpSocket->mRecvResponse())
    {
        deletePdnode(pdnode);
        delete mpSocket;
        return ;
    }
    datar.mAnalyResp(pdnode) ;	//analyze response
    //******************************************** Filter BlockType
    if(fltFilter.blockType(qstrAbsUrl, QString(pdnode->pRespSum->pcType)))
    {
        deletePdnode(pdnode);
        delete mpSocket;
        return;
    }
    //********************************************
    //return the response to client
    mpSocket->mRetResponse() ;
    //save the pdnode to list
    int iPos = datar.mSaveOne(pdnode) ;
    //******************************************** Filter HideCode
    if(fltFilter.hideCodes(qstrAbsUrl, QString(pdnode->pRespSum->caCode)))
    {
        delete mpSocket;
        return;
    }
    //******************************************** end filter
    //post message to UI
    ENewVisit * pclsEvnt = new ENewVisit(iPos) ;
    qApp->postEvent((QObject*)(pwdMainWin->pwinMonitor), pclsEvnt);
    mpSocket->close() ;
    delete mpSocket;
}
