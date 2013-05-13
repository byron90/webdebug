#include "wddatar.h"
#include <QMessageBox>
#include <fstream.h>
#include <stdio.h>

WDDatar::WDDatar()
{
}

WDDatar::~WDDatar()
{
}

//add a new client request to link
PDATANODE WDDatar::mAddNewRequ(char * pcRequ)
{
    if(pcRequ)
    {
        //store data about this request
        DATANODE nodTmp ;
        nodTmp.pcRequRaw = pcRequ ;
        nodTmp.pRequSum = mAnalyRequ(pcRequ) ;
        mMtxLst.lock() ;
        int iLenLst = mLstVisit.count() ;
        mLstVisit.append(nodTmp);
        mMtxLst.unlock() ;
        return &mLstVisit[iLenLst] ;
//        ofstream docOut("requ_sum.txt") ;
//        docOut<<nodTmp.pRequSum->caMethod<<endl ;
//        docOut<<nodTmp.pRequSum->pcHost<<endl ;
//        docOut<<nodTmp.pRequSum->pcUrl<<endl ;
//        docOut.close() ;
    }
    else
    {
        return NULL ;
    }
}

//add a new server response to link
bool WDDatar::mAddNewResp(char * pcResp)
{
    return true ;
}

//analyze raw client request
PREQUSUM WDDatar::mAnalyRequ(char * pcRequ)
{
    QString qstrRequ(pcRequ) ;
    PREQUSUM psumRequ = new REQUSUM ;
    psumRequ->pcHost = NULL ;	psumRequ->pcUrl = NULL ;
    QStringList qslstHeaders = qstrRequ.split(QString("\r\n"), QString::SkipEmptyParts) ;
    //iterate the list to find host header
    QStringList::iterator iteHeader ;
    for(iteHeader=qslstHeaders.begin(); iteHeader!=qslstHeaders.end(); iteHeader++)
    {
        //host
        if(iteHeader->startsWith("Host: "))
        {
            QByteArray byaHost = iteHeader->split(" ")[1].toAscii() ;
            psumRequ->pcHost = new char[byaHost.size()+1] ;
            qstrcpy(psumRequ->pcHost, byaHost.data()) ;
        }
    }
    if(psumRequ->pcHost)
    {
        //find request method and url
        QString qstrStart = qslstHeaders[0] ;
        QStringList qslstFields = qstrStart.split(" ") ;
        qstrcpy(psumRequ->caMethod, qslstFields[0].toAscii().data()) ;//method
        //***split url from abs-path with host value
        QByteArray byaUrl = qslstFields[1].split(psumRequ->pcHost)[1].toAscii() ;
        psumRequ->pcUrl = new char[byaUrl.size()+1] ;
        qstrcpy(psumRequ->pcUrl, byaUrl.data()) ;
    }
    return psumRequ ;
}

//analyze raw server response
//PRESP WDDatar::mAnalyResp(char * pcResp)
//{
//}
