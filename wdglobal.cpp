#include <QFile>
#include "wdglobal.h"

WDGlobal::WDGlobal(QObject *parent) :
    QObject(parent)
{
    mpLogger = new LOGGER;
    mpBlockUrl = new BLOCKURL;
    mpAutoResp = new AUTOR;
    mpHideCodes = new HIDECODES;
    mpBlockType = new BLOCKTYPE;
    //init mpHideCodes struct
    mpHideCodes->b2 = false; mpHideCodes->b302 = false;
    mpHideCodes->b304 = false; mpHideCodes->bnot2 = false;
    //init mpBlockType struct
    mpBlockType->bScript = false; mpBlockType->bImg = false;
    mpBlockType->bCss = false; mpBlockType->bSwf = false;
}

//write data to log
void WDGlobal::writeLog(QString &qstrLog)
{
    //line tail
    qstrLog += "\n";
    mpLogger->qmtxLock.lock();
    mpLogger->pqdocLog->write(qstrLog.toAscii());
    mpLogger->qmtxLock.unlock();
}

//close logger
void WDGlobal::closeLogger()
{
    mpLogger->pqdocLog->close();
}

//init logger
void WDGlobal::initLogger(QString &qstrFile)
{
    mpLogger->pqdocLog = new QFile(qstrFile);
    mpLogger->pqdocLog->open(QIODevice::ReadWrite|QIODevice::Append);
}

//init block url struct
//read urls from file qstrFile
void WDGlobal::initBlockUrl(QString &qstrFile)
{
    mpBlockUrl->qstrFilePath = qstrFile;
    if(qstrFile!="")
    {
        QFile qdocUrls(qstrFile);
        if(!qdocUrls.open(QIODevice::ReadOnly))
            return;
        QString qstrRawUrls(qdocUrls.readAll());
        QStringList qslstTemp = qstrRawUrls.split('\n');
        for(int i=0; i<qslstTemp.size(); i++)
            mpBlockUrl->qslstUrl<<qslstTemp[i];
        qdocUrls.close();
    }
}

//init auto respoonsing struct
//read urls and specific file path from file qstrFile
void WDGlobal::initAutoResponse(QString &qstrFile)
{
    mpAutoResp->qstrFilePath = qstrFile;
    if(qstrFile!="")
    {
        QFile qdocItems(qstrFile);
        if(!qdocItems.open(QIODevice::ReadOnly))
            return;
        QString qstrRawItems(qdocItems.readAll());
        QStringList qslstItems = qstrRawItems.split('\n');
        for(int i=0; i<qslstItems.size(); i++)
        {
            QStringList qslstOne = qslstItems[i].split('\t');
            AUTORNODE stAutoResp;
            stAutoResp.qstrUrl = qslstOne[0];	//url
            stAutoResp.qstrSrcPath = qslstOne[1];	//specific src file path
            mpAutoResp->qlstNodes.append(stAutoResp);
        }
        qdocItems.close();
    }
}
