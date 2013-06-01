#include <QApplication>
#include <QList>
//#include <QRegExp>
#include "wdfilter.h"
#include "wdglobal.h"
#include "wdevents.h"
#include "wdebug.h"

extern WDGlobal globals;
extern wdebug *pwdMainWin;

WDFilter::WDFilter()
{
}

//block specific request url
bool WDFilter::blockUrl(QString &qstrUrl)
{
    globals.mpBlockUrl->qmtxLock.lock();
    bool bBlock = globals.mpBlockUrl->qslstUrl.indexOf(qstrUrl)!=-1;
    globals.mpBlockUrl->qmtxLock.unlock();
    if(bBlock)
    {
        //post message to filter ui to monitor the filtering
        EFHits * pclsEvnt = new EFHits(0, qstrUrl, QString("")) ;
        qApp->postEvent((QObject*)(pwdMainWin->pwinFilter), pclsEvnt);
    }
    return bBlock;
}

//auto response to request
QString WDFilter::autoResponse(QString &qstrUrl)
{
    QList<AUTORNODE> *plstNodes = &(globals.mpAutoResp->qlstNodes);
    QString qstrSrcPath("");
    QString qstrRetData("");
    globals.mpAutoResp->qmtxLock.lock();
    //travel the list to check if qstrUrl in autoresponse list
    for(int i=0; i<plstNodes->size(); i++)
        if(qstrUrl==(*plstNodes)[i].qstrUrl)
            qstrSrcPath = (*plstNodes)[i].qstrSrcPath;
    globals.mpAutoResp->qmtxLock.unlock();
    if(qstrSrcPath!="")
    {
        //return the file's content to browser
        QFile qdocSrc(qstrSrcPath);
        //*** source file can't be found
        //*** return 404 page and post message to filter ui
        if(!qdocSrc.open(QIODevice::ReadOnly))
        {
            EFHits * pclsEvnt = new EFHits(1, qstrUrl, QString("Error! Can't open Source file.")) ;
            qApp->postEvent((QObject*)(pwdMainWin->pwinFilter), pclsEvnt);
            qstrRetData = "HTTP/1.1 404 Page Not Found\r\n\r\n";
        }
        //*** source file exists
        else
        {
            QString qstrRawData(qdocSrc.readAll());
            //****** check if source file are a http message
            //****** if yes, return raw data directly
            //****** else should add template http response header to raw data
            QRegExp qreHttp("^HTTP/\\d\.\\d\\s");
            if(qstrRawData.indexOf(qreHttp)==-1)
            {
                //****** no http response header
                QFile qdocTmpResp(globals.mqstrTmpRespPath);
                if(qdocTmpResp.open(QIODevice::ReadOnly))
                {
                    QString qstrTmpResp(qdocTmpResp.readAll());
                    QString qstrInt2Str;
                    qstrTmpResp.replace("##len##", qstrInt2Str.setNum(qstrRawData.length()));
                    qstrRetData = qstrTmpResp + qstrRawData;
                    //********* post message
                    EFHits * pclsEvnt = new EFHits(1, qstrUrl, qstrSrcPath) ;
                    qApp->postEvent((QObject*)(pwdMainWin->pwinFilter), pclsEvnt);
                    qdocTmpResp.close();
                }
                //********* no template file
                else
                {
                    EFHits * pclsEvnt = new EFHits(1, qstrUrl, QString("Error! Can't open response template file.")) ;
                    qApp->postEvent((QObject*)(pwdMainWin->pwinFilter), pclsEvnt);
                    qstrRetData = "HTTP/1.1 404 Page Not Found\r\n\r\n";
                }
            }
            else
            {
                //****** have http response header
                qstrRetData = qstrRawData;
                EFHits * pclsEvnt = new EFHits(1, qstrUrl, qstrSrcPath) ;
                qApp->postEvent((QObject*)(pwdMainWin->pwinFilter), pclsEvnt);
            }
            qdocSrc.close();
        }
    }
    return qstrRetData;
}

//hide response with some code
bool WDFilter::hideCodes(QString &qstrUrl, QString &qstrCode)
{
    //todos: change the bad coupling
    bool bStatus = false;
    globals.mpHideCodes->qmtxLock.lock();
    if(qstrCode.startsWith('2'))
    {
        //hide 2**
        if(globals.mpHideCodes->b2)
            bStatus = true;
    }
    else
    {
        //hide not 2**
        if(globals.mpHideCodes->bnot2)
            bStatus = true;
        //hide 302
        else if(qstrCode=="302" && globals.mpHideCodes->b302)
            bStatus = true;
        //hide 304
        else if(qstrCode=="304" && globals.mpHideCodes->b304)
            bStatus = true;
    }
    globals.mpHideCodes->qmtxLock.unlock();
    if(bStatus)
    {
        EFHits * pclsEvnt = new EFHits(2, qstrUrl, qstrCode) ;
        qApp->postEvent((QObject*)(pwdMainWin->pwinFilter), pclsEvnt);
        return true;
    }
    else
        return false;
}

//block some content-type
bool WDFilter::blockType(QString &qstrUrl, QString &qstrType)
{
    bool bRes = false;
    QRegExp qreJs("text/javascript");
    QRegExp qreImg("image/");
    QRegExp qreCss("text/css");
    //javascript
    if(qreJs.indexIn(qstrType)!=-1)
    {
        globals.mpBlockType->qmtxLock.lock();
        //block js
        if(globals.mpBlockType->bScript)
        {
            EFHits * pclsEvnt = new EFHits(3, qstrUrl, qstrType) ;
            qApp->postEvent((QObject*)(pwdMainWin->pwinFilter), pclsEvnt);
            bRes = true;
        }
        globals.mpBlockType->qmtxLock.unlock();
    }
    //img
    else if(qreImg.indexIn(qstrType)!=-1)
    {
        globals.mpBlockType->qmtxLock.lock();
        //block img
        if(globals.mpBlockType->bImg)
        {
            EFHits * pclsEvnt = new EFHits(3, qstrUrl, qstrType) ;
            qApp->postEvent((QObject*)(pwdMainWin->pwinFilter), pclsEvnt);
            bRes = true;
        }
        globals.mpBlockType->qmtxLock.unlock();
    }
    //css
    else if(qreCss.indexIn(qstrType)!=-1)
    {
        globals.mpBlockType->qmtxLock.lock();
        //block img
        if(globals.mpBlockType->bCss)
        {
            EFHits * pclsEvnt = new EFHits(3, qstrUrl, qstrType) ;
            qApp->postEvent((QObject*)(pwdMainWin->pwinFilter), pclsEvnt);
            bRes = true;
        }
        globals.mpBlockType->qmtxLock.unlock();
    }
    return bRes;
}
