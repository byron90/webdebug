#include <QDateTime>
#include "wddatar.h"
#include "wdglobal.h"

extern WDGlobal globals;

WDDatar::WDDatar()
{
}

WDDatar::~WDDatar()
{
}

int WDDatar::mSaveOne(PDATANODE pdnode)
{
    mMtxLst.lock() ;	//lock the mutex
    int iLenLst = mLstVisit.count() ;
    mLstVisit.append(*pdnode);	//add new node
    mMtxLst.unlock() ;
    return iLenLst ;
}

//analyze raw client request
bool WDDatar::mAnalyRequ(PDATANODE pdnode)
{
    QString qstrRequ(pdnode->pcRequRaw) ;
    pdnode->pRequSum = new REQUSUM ;
    PREQUSUM psumRequ = pdnode->pRequSum ;
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
            break ;
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
        QString qstrLog = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        qstrLog += " analyRequ: ok";
        globals.writeLog(qstrLog);
        return true ;
    }
    return false ;
}

//analyze raw server response
bool WDDatar::mAnalyResp(PDATANODE pdnode)
{
    QString qstrResp(pdnode->pcRespRaw) ;
    pdnode->pRespSum = new RESPSUM ;
    PRESPSUM psumResp = pdnode->pRespSum ;
    psumResp->pcType = NULL;
    QStringList qslstHeaders = qstrResp.split(QString("\r\n"), QString::SkipEmptyParts) ;
    //temp: analyze first line to find response code and statement
    QString qstrStatus = qslstHeaders[0] ;
    QStringList qslstStatus = qstrStatus.split(" ", QString::SkipEmptyParts) ;
    //***space in statement string
    if(qslstStatus.length()>3)
    {
        QString qstrStat = "" ;
        int iLenBef = qslstStatus.length() ;
        for(int i=2; i<iLenBef; i++)
        {
            qstrStat += qslstStatus[2] + " " ;
            qslstStatus.removeAt(2);
        }
        qslstStatus.append(qstrStat);
    }
    //***response code
    QByteArray byaCode = qslstStatus[1].toAscii() ;
    if(byaCode.length()>=4)
        return false ;
    qstrcpy(psumResp->caCode, byaCode.data()) ;
    //***code statement
    QByteArray byaStat = qslstStatus[2].toAscii() ;
    qstrcpy(psumResp->caStat, byaStat.data()) ;
    if(byaStat.length()>=20)
    {
        QString qstrLog = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        qstrLog += " analyRequ: error, status string's length is larger than 20";
        globals.writeLog(qstrLog);
        return false ;
    }
    //analyze content type
    QString qstrConType;
    QRegExp qreType("^Content\\-Type:\\s+([^\\r^\\n]+)");
    bool bConType = false;
    for(int i=1; i<qslstHeaders.size(); i++)
    {
        if(qreType.indexIn(qslstHeaders[i])!=-1)
        {
            bConType = true;
            break;
        }
    }
    //*** have content-type header
    if(bConType)
    {
        QByteArray qbyaConType = qreType.cap(1).toAscii();
        psumResp->pcType = new char[qbyaConType.size()+1];
        qstrcpy(psumResp->pcType, qbyaConType.data());
    }
    else
    {
        QString qstrLog = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        qstrLog += " analyRequ: content-type not found";
        globals.writeLog(qstrLog);
    }
    //log it
    QString qstrLog = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrLog += " analyRequ: ok";
    globals.writeLog(qstrLog);
    return true ;
}


////add a new client request to link
//bool WDDatar::mAddNewRequ(PDATANODE pdnode)
//{
//    if(pdnode->pcRequRaw)
//    {
//        //store data about this request
//                //TODO remember the iLenLst
//        //return &mLstVisit[iLenLst] ;
//        ofstream docOut("requ_sum.txt", ios::app) ;
//        docOut<<pdnode->pcRequRaw<<endl ;
//        docOut<<pdnode->pRequSum->caMethod<<" " ;
//        docOut<<pdnode->pRequSum->pcHost<<" " ;
//        docOut<<pdnode->pRequSum->pcUrl<<endl ;
//        docOut.close() ;
//        return true ;
//    }
//    else
//    {
//        return false ;
//    }
//}
//
////add a new server response to link list
//bool WDDatar::mAddNewResp(PDATANODE pdnode)
//{
//    mAnalyResp(pdnode) ;
//    ofstream docOut("response_raw.txt") ;
//    docOut<<pdnode->pcRespRaw<<endl ;
//    docOut.close() ;
//    return true ;
//}

